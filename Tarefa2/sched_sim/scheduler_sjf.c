#include <stdio.h>

#include "queue.h"  // Contém struct queue, struct proc, e protótipos de funções de fila como dequeue_bypid
#include "proc.h"   // Possui a definição de struct proc e ProcState
#include "stats.h"  // Possui as funções de estatísticas (ex: count_ready_in, count_ready_out)
#include "utils.h"  // Possui funções úteis (se houver alguma usada aqui)

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos (não usada neste SJF simples)
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados

// Variável global que indica o tempo máximo que um processo pode executar ao todo
extern int MAX_TIME; // Não diretamente usada no SJF, mas importante para o sistema

/*
 * scheduler: Função principal do escalonador SJF (Shortest Job First).
 * Seleciona o processo com o menor 'remaining_time' da fila de prontos.
 *
 * @param current: Ponteiro para o processo que estava em execução na rodada anterior.
 * Pode ser NULL se nenhum processo estava executando.
 *
 * @return: Ponteiro para o processo selecionado para execução nesta rodada.
 * Retorna NULL se a fila de aptos estiver vazia.
 */
struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 

    /*
     * 1. Tratamento do Processo Atualmente em Execução (current)
     * Verifica o estado do processo 'current' e o realoca na fila apropriada
     * (prontos, bloqueados ou finalizados).
     */
    if (current != NULL)
    {
        // Verifica o estado em que o processo executando está e o enfileira adequadamente
        switch (current->state) 
        {
            case READY:
                enqueue(ready, current);      // Retorna à fila de aptos
                count_ready_in(current);      // Atualiza estatísticas de entrada na fila de aptos
                break;

            case BLOCKED:
                enqueue(blocked, current);    // Move para a fila de bloqueados
                count_blocked_in(current);    // Atualiza estatísticas de entrada na fila de bloqueados
                break;

            case FINISHED:
                enqueue(finished, current);   // Move para a fila de finalizados
                count_finished_in(current);   // Atualiza estatísticas de entrada na fila de finalizados
                break;

            default:
                // Em caso de estado inesperado, imprime um erro
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     * 2. Estratégia de Seleção de um Novo Processo para Executar (SJF)
     * Procura o processo com o menor 'remaining_time' na fila 'ready'.
     */

    // Se a fila de aptos estiver vazia, não há processos para escalonar
    if (isempty(ready)) // Assume que 'isempty' verifica se 'ready->head' é NULL
    {
        return NULL;
    }

    // Implementação do algoritmo SJF (Shortest Job First)
    // Inicializa 'min_proc' com o primeiro processo da fila.
    // Usamos 'struct proc *' diretamente, pois os processos são os nós da lista.
    struct proc *aux = ready->head;
    struct proc *min_proc = aux; 

    // Itera por todos os processos na fila 'ready' para encontrar o menor 'remaining_time'
    while (aux != NULL) {
        // Se o tempo restante do processo atual ('aux') for menor que o do 'min_proc' encontrado até agora
        if (aux->remaining_time < min_proc->remaining_time) {
            min_proc = aux; // Atualiza 'min_proc' para este processo
        }
        aux = aux->next; // Move para o próximo processo na fila (usando o ponteiro 'next' de 'struct proc')
    }

    // Remove o processo selecionado (com menor tempo restante) da fila 'ready'.
    // Usamos 'dequeue_bypid' com o PID do processo encontrado.
    selected = dequeue_bypid(ready, min_proc->pid);

    // Realiza as estatísticas para o processo que saiu da fila de aptos
    count_ready_out(selected);

    // Altera o estado do processo selecionado para RUNNING (em execução)
    selected->state = RUNNING;

    return selected; // Retorna o processo que irá executar nesta rodada
}