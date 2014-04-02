#!/bin/bash

mpicc tree.c
mpirun -np 8  a.out  && rm a.out
