#!/bin/bash

# Permitir: chmod +x script.sh

# Uso: ./script.sh nome_do_executavel
# Exemplo: ./script.sh main_ljf

# Para salvar a saída:
# ./scriptljf.sh main_ljf > resultados_ljf.txt

# Verifica se um argumento foi passado
if [ -z "$1" ]; then
    echo "Uso: $0 nome_do_executavel"
    exit 1
fi

EXECUTAVEL=$1

for j in $(seq 10 10 100)
do
    for i in $(seq 1 10)
    do
        output=$("./$EXECUTAVEL" $j | grep TME)
        echo "$output $j"
    done
done
