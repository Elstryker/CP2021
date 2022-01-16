#!/bin/bash
#SBATCH --time=1:00 
#SBATCH --partition=cpar

MY=/home/pg47576/CP2021

module load papi/5.4.1
export OMP_NUM_THREADS=16
$MY/test 1000000 100
time  $MY/test 1000000 100


# perf record  ./test
# perf report -n > perfreport 
