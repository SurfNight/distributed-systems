#!/bin/bash

for i in 4 8 16 32 64; do
    for j in $(seq $i); do
        ./client 1 5 127.0.0.1 &
    done
    wait
    cp resultado.txt test_2/resultados/$i.txt
     > resultado.txt
done

cp coord.txt test_2/coord.txt
 > coord.txt
