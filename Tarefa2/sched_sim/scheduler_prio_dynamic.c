#include <stdio.h>
#include <stdlib.h> // para rand()

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Tratamento do processo que acabou de sair da CPU
    if (current != NULL)
    {
        switch (current->state)
        {
            case READY:
                // Preempção -> vai para fila de baixa prioridade
                current->queue = 1;
                enqueue(ready2, current);
                count_ready_in(current);
                break;

            case BLOCKED:
                // Saiu por E/S -> volta para fila de alta prioridade
                current->queue = 0;
                enqueue(blocked, current);
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

    // Seleção do próximo processo com probabilidade 80%/20%
    int prob = rand() % 100;
    // printf("Probabilidade sorteada: %d\n", prob);

    if ((prob < 80 && !isempty(ready)) || isempty(ready2))
    {
        // Seleciona da fila de alta prioridade
        selected = dequeue(ready);
    }
    else if (!isempty(ready2))
    {
        // Seleciona da fila de baixa prioridade
        selected = dequeue(ready2);
    }
    else if (!isempty(ready))
    {
        // Se ready2 vazia, tenta ready
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