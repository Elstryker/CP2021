#!/bin/bash
#SBATCH --time=1:00 
#SBATCH --partition=cpar

MY=/home/pg47576/CP2021

module load papi/5.4.1
export OMP_NUM_THREADS=16
$MY/test 10000 16 
time  $MY/test 10000 16


perf record  ./test
perf report -n > perfreport 