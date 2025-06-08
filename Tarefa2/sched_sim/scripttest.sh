#!/bin/bash

# chmod +x scripttest.sh
# Uso: ./scripttest.sh


for j in $(seq 90 10 100)
do
    for i in $(seq 1 10)
    do
        output=$(./main_prio_dynamic_quantum $j | grep TME)
        echo "$output $j"
    done
done


