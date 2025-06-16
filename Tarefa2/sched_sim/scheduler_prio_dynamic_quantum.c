#include <stdio.h>
#include <stdlib.h> // para rand()

#include "queue.h" // contém funções úteis para filas
#include "proc.h"  // contém definição da struct proc
#include "stats.h" // funções de estatísticas
#include "utils.h" // funções auxiliares

// Filas globais
extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

extern int MAX_TIME;
extern int QUANTUM;

struct proc *scheduler(struct proc *current)
{
    struct proc *selected = NULL;

    // Tratamento do processo que saiu da CPU
    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            // PRIO DYNAMIC QUANTUM:
            // Se o processo usou mais de 50% do QUANTUM permitido na rodada,
            // então ele volta para a fila de alta prioridade (ready)
            // Caso contrário, vai para a fila de baixa prioridade (ready2)

            if (current->process_time >= (QUANTUM / 2))
            {
                current->queue = 0; // Fila 1
                enqueue(ready, current);
                count_ready_in(current);
            }
            else
            {
                current->queue = 1; // Fila 2
                enqueue(ready2, current);
                count_ready_in(current);
            }
            break;

        case BLOCKED:
            // Saiu para E/S → volta para fila de alta prioridade
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

    // Sorteio de 80% chance de escolher da fila ready
    int prob = rand() % 100;

    if ((prob < 80 && !isempty(ready)) || isempty(ready2))
    {
        selected = dequeue(ready);
    }
    else if (!isempty(ready2))
    {
        selected = dequeue(ready2);
    }
    else if (!isempty(ready))
    {
        selected = dequeue(ready);
    }
    else
    {
        return NULL;
    }

    if (selected != NULL)
    {
        count_ready_out(selected);
        selected->state = RUNNING;
    }

    return selected;
}
