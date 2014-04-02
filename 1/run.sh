#!/bin/bash

mpicc global_sum2.c -o gsum
mpirun -np 4 ./gsum
