#!/bin/bash
#
#SBATCH --job-name=NOMAD-1
#SBATCH --output=res.txt
#
#SBATCH --mail-user=esteban.foncoux@student.unamur.be
#SBATCH --mail-type=ALL
#
#SBATCH --time=02:30:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=8000
#
#SBATCH --array=0-99

cd $HOME/MMMI/build1
module load GSL

srun ./Main_classe_1 $SLURM_ARRAY_TASK_ID

