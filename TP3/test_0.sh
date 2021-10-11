#!/bin/bash

./cliente 2 10 127.0.0.1 &
./cliente 2 10 127.0.0.1 &

wait

cp resultado.txt test_0/resultados.txt
 > resultado.txt

cp coord.txt test_0/coord.txt
 > coord.txt