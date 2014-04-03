#!/bin/bash

mpicc parallel_heat.c -o heated_plate 
mpirun -np 8 heated_plate 0.01 output_tmp
./grid_to_bmp output_tmp output.bmp
rm output_tmp
