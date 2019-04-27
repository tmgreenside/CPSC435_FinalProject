#!/bin/bash
#PBS -l nodes=4:ppn=2:physical
#PBS -l walltime=00:10:00
#PBS -o /home/Students/tgreenside/Assignment#6_Greenside/job_out
#PBS -j oe
#PBS -N lab3

date
export PROGRAM="finalProject"
mpicc -o $PROGRAM main.c
mpirun -machinefile $PBS_NODEFILE $PROGRAM 512
date
exit 0
