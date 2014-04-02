#!/bin/bash

mpicc pipe.c -o pipe
mpirun -np 16 ./pipe 10
