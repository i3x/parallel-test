#!/bin/bash

mpicc tree.c
mpirun a.out -np 8
rm a.out
