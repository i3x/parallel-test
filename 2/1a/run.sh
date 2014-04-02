#!/bin/bash

mpicc rectangle_integrate.c -o rec_int
time ./rec_int -100000 100000 100000000
