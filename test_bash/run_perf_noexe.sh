# Bash script for running experiments with varying number of exection with PerfEvent
declare -a file_array
file_array=(f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse)

declare -a error_array_fq
declare -a max_seg_array_fq
error_array_fq=(1024 4096 256 64 1024 64 1024)
max_seg_array_fq=(110 3000 400 690 22000 10 100)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "Flirt , ${file_array[$index]}, $no_execution";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR ${error_array_fq[$index]}" >> parameters_perf.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_perf.h;
        echo "#define RUNFLAG 0" >> parameters_perf.h;
        echo "#define FANOUT 4" >> parameters_perf.h
        echo "#define WINDOWSIZE 100000000" >> parameters_perf.h;
        echo "#define MAXSEG ${max_seg_array_fq[$index]}" >> parameters_perf.h;
        g++ test_perf.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_flirt_perf_noexe.txt
done
done


declare -a fanout_array_bp
fanout_array_bp=(256 256 16 64 256 64 64)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "BP TREE , ${file_array[$index]}, $no_execution";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_perf.h;
        echo "#define RUNFLAG 1" >> parameters_perf.h;
        echo "#define FANOUT ${fanout_array_bp[$index]}" >> parameters_perf.h
        echo "#define WINDOWSIZE 100000000" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_bp_pgm_perf_noexe.txt
done
done


declare -a fanout_array_pgm
fanout_array_pgm=(16 4 4 4 16 4 16)

for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "PGM , ${file_array[$index]}, $no_execution";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_perf.h;
        echo "#define RUNFLAG 2" >> parameters_perf.h;
        echo "#define FANOUT ${fanout_array_pgm[$index]}" >> parameters_perf.h
        echo "#define WINDOWSIZE 100000000" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_bp_pgm_perf_noexe.txt
done
done


for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "Queue , ${file_array[$index]}, $no_execution";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_perf.h;
        echo "#define RUNFLAG 4" >> parameters_perf.h;
        echo "#define FANOUT 4" >> parameters_perf.h
        echo "#define WINDOWSIZE 100000000" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_queue_perf_noexe.txt
done
done


for index in "${!file_array[@]}"; do
    for no_execution in 1000 10000 100000 1000000; do
        echo "ALEX , ${file_array[$index]}, $no_execution";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION $no_execution" >> parameters_perf.h;
        echo "#define RUNFLAG 3" >> parameters_perf.h;
        echo "#define FANOUT 4" >> parameters_perf.h
        echo "#define WINDOWSIZE 100000000" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_alex_perf_noexe.txt
done
done
