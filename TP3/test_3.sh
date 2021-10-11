#!/bin/bash

for i in 4 8 16 32 64 128; do
    for j in $(seq $i); do
        ./cliente 0 3 127.0.0.1 &
    done
    wait
    cp resultado.txt test_3/resultados/$i.txt
     > resultado.txt
done

cp coord.txt test_3/coord.txt
 > coord.txt
