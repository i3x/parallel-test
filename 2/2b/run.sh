#!/bin/bash

mpicc monte_carlo_pi.c -o monte_carlo_pi
time mpirun -np 2 ./monte_carlo_pi 0.00001
