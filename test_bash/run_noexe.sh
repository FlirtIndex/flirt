# Bash script for running experiments with varying number of executions
declare -a file_array
file_array=(f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse)

declare -a error_array_fq
declare -a max_seg_array_fq
error_array_fq=(1024 4096 256 64 1024 64 1024)
max_seg_array_fq=(110 3000 400 690 22000 10 100)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "Flirt , ${file_array[$index]}, $no_execution";
        > parameters_noexe.h;
        echo "#pragma once" >> parameters_noexe.h;
        echo "#define ERROR ${error_array_fq[$index]}" >> parameters_noexe.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_noexe.h;
        echo "#define RUNFLAG 0" >> parameters_noexe.h;
        echo "#define FANOUT 4" >> parameters_noexe.h
        echo "#define WINDOWSIZE 100000000" >> parameters_noexe.h;
        echo "#define MAXSEG ${max_seg_array_fq[$index]}" >> parameters_noexe.h;
        g++ test_noexe.cpp -march=native -std=c++17 -O3 -w -o z_run_noexe && ./z_run_noexe ${file_array[$index]} >> log_flirt_no_execution.txt
done
done


declare -a fanout_array_bp
fanout_array_bp=(256 256 16 64 256 64 64)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "BP TREE , ${file_array[$index]}, $no_execution";
        > parameters_noexe.h;
        echo "#pragma once" >> parameters_noexe.h;
        echo "#define ERROR 4" >> parameters_noexe.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_noexe.h;
        echo "#define RUNFLAG 1" >> parameters_noexe.h;
        echo "#define FANOUT ${fanout_array_bp[$index]}" >> parameters_noexe.h
        echo "#define WINDOWSIZE 100000000" >> parameters_noexe.h;
        echo "#define MAXSEG 1" >> parameters_noexe.h;
        g++ test_noexe.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_noexe && ./z_run_noexe ${file_array[$index]} >> log_bp_pgm_no_execution.txt
done
done


declare -a fanout_array_pgm
fanout_array_pgm=(16 4 4 4 16 4 16)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "PGM , ${file_array[$index]}, $no_execution";
        > parameters_noexe.h;
        echo "#pragma once" >> parameters_noexe.h;
        echo "#define ERROR 4" >> parameters_noexe.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_noexe.h;
        echo "#define RUNFLAG 2" >> parameters_noexe.h;
        echo "#define FANOUT ${fanout_array_pgm[$index]}" >> parameters_noexe.h
        echo "#define WINDOWSIZE 100000000" >> parameters_noexe.h;
        echo "#define MAXSEG 1" >> parameters_noexe.h;
        g++ test_noexe.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_noexe && ./z_run_noexe ${file_array[$index]} >> log_bp_pgm_no_execution.txt
done
done


for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "Queue , ${file_array[$index]}, $no_execution";
        > parameters_noexe.h;
        echo "#pragma once" >> parameters_noexe.h;
        echo "#define ERROR 4" >> parameters_noexe.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_noexe.h;
        echo "#define RUNFLAG 4" >> parameters_noexe.h;
        echo "#define FANOUT 4" >> parameters_noexe.h
        echo "#define WINDOWSIZE 100000000" >> parameters_noexe.h;
        echo "#define MAXSEG 1" >> parameters_noexe.h;
        g++ test_noexe.cpp -march=native -std=c++17 -O3 -w -o z_run_noexe && ./z_run_noexe ${file_array[$index]} >> log_queue_no_execution.txt
done
done


for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "ALEX , ${file_array[$index]}, $no_execution";
        > parameters_noexe.h;
        echo "#pragma once" >> parameters_noexe.h;
        echo "#define ERROR 4" >> parameters_noexe.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_noexe.h;
        echo "#define RUNFLAG 3" >> parameters_noexe.h;
        echo "#define FANOUT 4" >> parameters_noexe.h
        echo "#define WINDOWSIZE 100000000" >> parameters_noexe.h;
        echo "#define MAXSEG 1" >> parameters_noexe.h;
        g++ test_noexe.cpp -march=native -std=c++17 -O3 -w -o z_run_noexe && ./z_run_noexe ${file_array[$index]} >> log_alex_no_execution.txt
done
done