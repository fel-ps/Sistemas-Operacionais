#!/bin/bash

# Uso:
# chmod +x script.sh
# ./script.sh > resultados.txt

# Lista de traces
traces=("trace1.trace" "trace2.trace" "trace3.trace" "trace4.trace")

# Lista de algoritmos
algoritmos=("random" "fifo" "lru" "sc" "lfu" "mfu")

# Lista de valores de frames
frames=(2 4 8 16 32 64)

# Caminho do executável
EXECUTAVEL="./simulador"

# Cabeçalho do CSV/tabulado
echo -e "trace\talgoritmo\tframes\tmem_refs\tpage_faults\tpages_written"

# Loop principal
for trace in "${traces[@]}"; do
    for alg in "${algoritmos[@]}"; do
        for frame in "${frames[@]}"; do
            # Executa o simulador e captura as 3 linhas de interesse
            output=$($EXECUTAVEL "$trace" "$frame" "$alg")

            # Extrai os valores usando grep/sed
            mem_refs=$(echo "$output" | grep "memory references" | sed 's/[^0-9]*//g')
            page_faults=$(echo "$output" | grep "page faults" | sed 's/[^0-9]*//g')
            pages_written=$(echo "$output" | grep "pages written" | sed 's/[^0-9]*//g')

            # Imprime linha formatada para CSV ou planilha
            echo -e "$trace\t$alg\t$frame\t$mem_refs\t$page_faults\t$pages_written"
        done
    done
done
