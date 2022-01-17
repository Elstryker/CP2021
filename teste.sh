#!/bin/bash
#SBATCH --time=1:00 
#SBATCH --partition=cpar

MY=/home/pg47576/CP2021

module load papi/5.4.1
export OMP_NUM_THREADS=40

srun --partition=cpar --nodelist=compute-134-102 papi_component_avail
srun --partition=cpar papi_mem_info compute-134-102

srun --partition=cpar --nodelist=compute-134-102 $MY/test 8000 40
srun --partition=cpar --nodelist=compute-134-102 $MY/test 64000 40
srun --partition=cpar --nodelist=compute-134-102 $MY/test 2048000 40
