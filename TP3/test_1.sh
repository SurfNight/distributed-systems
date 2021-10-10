#!/bin/bash


for i in 4 8 16 32; do
    for j in $(seq $i); do
        ./cliente 2 10 172.26.86.132 &
    done
    wait
done