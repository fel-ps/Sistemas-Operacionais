#include <stdio.h>
#include <stdlib.h> // para rand()

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas
#include "utils.h" // possui funções uteis

// Utilizando as variáveis globais definidas no 'main'
extern struct queue *ready;     // fila de aptos
extern struct queue *ready2;    // segunda fila de aptos
extern struct queue *blocked;   // fila de bloqueados
extern struct queue *finished;  // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;
// Variavel global QUANTUM, necessária para a nova regra
extern int QUANTUM;

struct proc *scheduler(struct proc *current)
{
    struct proc *selected = NULL;

    // Tratamento do processo que acabou de sair da CPU
    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            // Regra PRIO DYNAMIC QUANTUM para retorno às filas:
            // Se utilizou mais de 50% do QUANTUM, volta para Fila 1 (ready)
            // Se utilizou menos de 50% do QUANTUM, volta para Fila 2 (ready2)
            if (current->quantum_used > (QUANTUM / 2))
            {
                current->queue = 0; // Fila 1 (ready)
                enqueue(ready, current);
                count_ready_in(current);
            }
            else
            {
                current->queue = 1; // Fila 2 (ready2)
                enqueue(ready2, current);
                count_ready_in(current);
            }
            break;

        case BLOCKED:
            // Saiu por E/S -> volta para fila de alta prioridade (Fila 1)
            current->queue = 0;
            enqueue(ready, current);
            count_blocked_in(current);
            break;

        case FINISHED:
            enqueue(finished, current);
            count_finished_in(current);
            break;

        default:
            printf("@@ ERRO no estado do processo %d\n", current->pid);
        }
    }

    // A inclusão de novos processos na fila 'ready' (Fila 1)
    // precisa ser tratada ANTES do escalonador, geralmente na função que
    // simula a chegada de processos (e.g., uma função `new_proc_arrival` ou `create_process`).
    // No seu código original PRIO DYNAMIC, a lógica de inserção na fila
    // era mais simples. Para o PRIO DYNAMIC QUANTUM, todos os processos
    // *começam* na Fila 1. Certifique-se de que quando um novo processo é criado,
    // ele seja inicializado com `current->queue = 0` e inserido em `ready`.

    // Seleção do próximo processo com probabilidade 80%/20%
    int prob = rand() % 100;
    // printf("Probabilidade sorteada: %d\n", prob);

    if ((prob < 80 && !isempty(ready)) || isempty(ready2))
    {
        // Seleciona da fila de alta prioridade (Fila 1)
        selected = dequeue(ready);
    }
    else if (!isempty(ready2))
    {
        // Seleciona da fila de baixa prioridade (Fila 2)
        selected = dequeue(ready2);
    }
    else if (!isempty(ready))
    {
        // Se ready2 estiver vazia, tenta ready
        selected = dequeue(ready);
    }
    else
    {
        // Não há processo disponível
        return NULL;
    }

    // Se achou algum processo
    if (selected != NULL)
    {
        count_ready_out(selected);
        selected->state = RUNNING;

        // printf("Selecionado processo %d da fila %d\n", selected->pid, selected->queue);
    }

    return selected;
}