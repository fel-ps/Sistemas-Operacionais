#include <stdio.h>

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
    struct proc * selected;

    // Se existe um processo atualmente executando, verificar seu estado
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state)
        {
        case READY: // Processo ainda está apto, coloca ele de volta na fila de prontos
            enqueue(ready, current);
            count_ready_in(current);
            break;
        case BLOCKED: // Processo bloqueado, coloca ele na fila de bloqueados
            enqueue(blocked, current);
            count_blocked_in(current);
            break;
        case FINISHED: // terminou sua execução, coloca ele na fila de finalizados
            enqueue(finished, current);
            count_finished_in(current);
            break;
        default:
            // Caso o estado do processo seja desconhecido ou inválido, imprime erro
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Se não há processos prontos para executar, retorna NULL (não tem processo selecionado)
    if (isempty(ready))
        return NULL;

    // Procurar o processo com maior remaining_time (tempo restante)
    int max_time = -1;  // Começamos com -1 pois os tempos são positivos
    struct proc *max_proc = NULL;
    int size = countqueue(ready);

    for (int i = 0; i < size; i++)
    {
        struct proc *p = dequeue(ready);

        if (p->remaining_time > max_time)
        {
            // Se já existia um processo com maior tempo, reencolá-lo
            if (max_proc != NULL)
                enqueue(ready, max_proc);

            max_time = p->remaining_time;
            max_proc = p;
        }
        else
        {
            enqueue(ready, p);
        }
    }

    selected = max_proc;

    // Contabiliza a saída da fila de aptos para estatísticas
    count_ready_out(selected);

    // Altera estado do processo selecionado para executando
    selected->state = RUNNING;

    return selected;
}
