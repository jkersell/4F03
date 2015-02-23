#!/bin/bash

for b in 1000000000 100000000 1000000000; do
        for p in 1 2 4 8 12 16 20 24 28 32; do
                sqsub -q mpi -n $p -r 4h -o q2-proc$p-10$b.log ./primegap 2 $b
        done
done
