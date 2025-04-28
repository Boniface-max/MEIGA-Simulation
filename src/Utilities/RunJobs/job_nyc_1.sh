#!/bin/bash

# Script para obtener los jobs de MEIGA :
# Solo cambiar el inputfile y la carpeta base y jobs

#Registro del tiempo de inicio del trabajo
start_time=$(date +"%Y-%m-%d %H:%M:%S")

host=`/bin/hostname`
date=`/bin/date`
echo "Test JOB ">> Registro1cm.txt
echo "Machine: "$host>> Registro1cm.txt 
echo "Date: "$date>> Registro1cm.txt 
echo "Inicio del trabajo: $start_time">> Registro1cm.txt 

path="/opt/data"
inputfile="$path/CorsikaFlujos/1hora/S3_bga_003600_fluka.shw"
basefile="$path/Articulo/plomo/1cm/base"
jobsfile="$path/Articulo/plomo/1cm/jobs"
num_evts=416521
num_jobs=10

python3 /opt/meiga/src/Utilities/RunJobs/createjobs.py -i "$inputfile" -n "$num_evts" -b "$basefile" -j "$jobsfile"

python3 /opt/meiga/src/Utilities/RunJobs/runjobs.py -j "$jobsfile" -n "$num_jobs"

# Registro del tiempo de finalización del trabajo
end_time=$(date +"%Y-%m-%d %H:%M:%S")

# Guardar el tiempo de cómputo en un archivo
echo "Inputfile : $inputfile " >> Registro1cm.txt 
echo "srun -p GIRG -n 10 --qos=girg --pty /bin/bash" >> Registro1cm.txt 
echo "Job: meiga_Hodo__app  - Inicio: $start_time - Fin: $end_time" >> Registro1cm.txt 
echo "Fin del trabajo: $end_time" >> Registro1cm.txt 


