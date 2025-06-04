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

    /*
     * Tratando o processo que acabou de sair da CPU (current)
     */
    if (current != NULL)
    {
        switch (current->state)
        {
            case READY: 
                // Processou e voltou para pronto (preempção)
                // Inserir na fila correta conforme remaining_time
                
                if (current->remaining_time <= 0.2 * MAX_TIME) {
                    current->queue = 0;
                    enqueue(ready, current);
                    count_ready_in(current);
                } else {
                    current->queue = 1;
                    enqueue(ready2, current);
                    count_ready_in(current); // Pode ter contagem separada se quiser
                }
                break;

            case BLOCKED:
                // Saiu para I/O, vai para bloqueados
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

    /*
     * Seleção do próximo processo para executar
     * Probabilidade: 80% fila ready (0), 20% fila ready2 (1)
     */

    int prob = rand() % 100;

    if ((prob < 80 && !isempty(ready)) || isempty(ready2))
    {
        // Seleciona da fila de prioridade alta
        selected = dequeue(ready);
    }
    else if (!isempty(ready2))
    {
        // Seleciona da fila de prioridade baixa
        selected = dequeue(ready2);
    }
    else if (!isempty(ready))
    {
        // Caso fila 2 esteja vazia, tenta fila 1
        selected = dequeue(ready);
    }
    else
    {
        // Não há processo para executar
        return NULL;
    }

    if (selected != NULL)
    {
        // Atualiza estatísticas: saiu da fila correta
        count_ready_out(selected);

        // printf("Selecionado processo %d da fila %d\n", selected->pid, selected->queue);

        // Define estado para RUNNING
        selected->state = RUNNING;
    }

    return selected;
}
