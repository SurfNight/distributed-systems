#!/bin/bash


for i in 4 8 16 32; do
    for j in $(seq $i); do
        ./cliente 2 10 127.0.0.1 &
    done
    wait
    cp resultado.txt test_1/resultados/$i.txt
     > resultado.txt
done
