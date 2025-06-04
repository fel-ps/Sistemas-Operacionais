#include <stdio.h>

#include "queue.h"  // Contém funções úteis para filas
#include "proc.h"   // Possui as funções dos processos
#include "stats.h"  // Possui as funções de estatísticas 
#include "utils.h"  // Possui funções úteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos (não usada aqui)
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados

// Variável global que indica o tempo máximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 

    /*
     *   Tratando o processo que está atualmente executando
     */

    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            case READY:
                enqueue(ready, current);
                count_ready_in(current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                count_blocked_in(current);
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    if (isempty(ready))
    {
        return NULL;
    }

    // Implementação do algoritmo SJF (Shortest Job First)
    struct node *aux = ready->head;
    struct node *min_node = aux;

    while (aux != NULL) {
        if (aux->proc->remaining_time < min_node->proc->remaining_time) {
            min_node = aux;
        }
        aux = aux->next;
    }

    // Remove o processo com menor tempo restante da fila
    selected = removep(ready, min_node->proc->pid);

    // Realizando as estatísticas para o processo que saiu da fila de aptos
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;

    return selected;
}
