# Bash script for running experiments with varying window size with PerfEvent (Output is the size of each data structure)
# Incomplete (Results not in paper)
declare -a file_array
file_array=(f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse)

while IFS="," read -r index filename error windowsize maxseg
do
    echo "Flirt , $filename, $windowsize";
    > parameters_perf.h;
    echo "#pragma once" >> parameters_perf.h;
    echo "#define ERROR $error" >> parameters_perf.h;
    echo "#define NO_EXECUTION 10000" >> parameters_perf.h;
    echo "#define RUNFLAG 0" >> parameters_perf.h;
    echo "#define FANOUT 4" >> parameters_perf.h;
    echo "#define WINDOWSIZE $windowsize" >> parameters_perf.h;
    echo "#define MAXSEG $maxseg" >> parameters_perf.h;
    g++ test_perf_size.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf $filename >> log_flirt_perf_size.txt
done < <(tail -n +2 MaxSeg_Perf.csv)


declare -a fanout_array_bp
fanout_array_bp=(256 256 16 64 256 64 64)

for index in "${!file_array[@]}"; do
    for windowsize in 10000 1000000 100000000; do
        echo "BP TREE , ${file_array[$index]}, $windowsize";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION 10000" >> parameters_perf.h;
        echo "#define RUNFLAG 1" >> parameters_perf.h;
        echo "#define FANOUT ${fanout_array_bp[$index]}" >> parameters_perf.h
        echo "#define WINDOWSIZE $windowsize" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf_size.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_bp_pgm_perf_size.txt
done
done


declare -a fanout_array_pgm
fanout_array_pgm=(16 4 4 4 16 4 16)

for index in "${!file_array[@]}"; do
    for windowsize in 10000 1000000 100000000; do
        echo "PGM , ${file_array[$index]}, $windowsize";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION 10000" >> parameters_perf.h;
        echo "#define RUNFLAG 2" >> parameters_perf.h;
        echo "#define FANOUT ${fanout_array_pgm[$index]}" >> parameters_perf.h
        echo "#define WINDOWSIZE $windowsize" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf_size.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_bp_pgm_perf_size.txt
done
done


for index in "${!file_array[@]}"; do
    for windowsize in 10000 1000000 100000000; do
        echo "Queue , ${file_array[$index]}, $windowsize";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION 10000" >> parameters_perf.h;
        echo "#define RUNFLAG 4" >> parameters_perf.h;
        echo "#define FANOUT 4" >> parameters_perf.h
        echo "#define WINDOWSIZE $windowsize" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf_size.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_queue_perf_size.txt
done
done


for index in "${!file_array[@]}"; do
    for windowsize in 10000 1000000 100000000; do
        echo "ALEX , ${file_array[$index]}, $windowsize";
        > parameters_perf.h;
        echo "#pragma once" >> parameters_perf.h;
        echo "#define ERROR 4" >> parameters_perf.h;
        echo "#define NO_EXECUTION 10000" >> parameters_perf.h;
        echo "#define RUNFLAG 3" >> parameters_perf.h;
        echo "#define FANOUT 4" >> parameters_perf.h
        echo "#define WINDOWSIZE $windowsize" >> parameters_perf.h;
        echo "#define MAXSEG 1" >> parameters_perf.h;
        g++ test_perf_size.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_perf && ./z_run_perf ${file_array[$index]} >> log_alex_perf_size.txt
done
done
