#!/bin/bash

./cliente 2 10 127.0.0.1 &
./cliente 2 10 127.0.0.1 &

wait

cp resultados.txt test_0/resultados.txt
rm -f resultados.txt
