# Bash script for running experiments with varying read-to-write ratios
declare -a file_array
file_array=(f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse)

declare -a rw_ratio_array
declare -a update_array
declare -a search_array
declare -a execution_array
rw_ratio_array=(100 10 1)
update_array=(3 15 30)
search_array=(60 30 6)
execution_array=(35000 7000 3500)

declare -a fanout_array_bp
fanout_array_bp=(256 256 16 64 256 64 64)

declare -a error_array_fq
declare -a max_seg_array_fq
error_array_fq=(1024 4096 256 64 1024 64 1024)
max_seg_array_fq=(110 3000 400 690 22000 10 100)

for index in "${!file_array[@]}"; do
    for num in "${!update_array[@]}"; do
        echo "Flirt , ${file_array[$index]} , ${rw_ratio_array[$num]}";
        > parameters_rw.h;
        echo "#pragma once" >> parameters_rw.h;
        echo "#define ERROR ${error_array_fq[$index]}" >> parameters_rw.h;
        echo "#define NUM_UPDATE ${update_array[$num]}" >> parameters_rw.h;
        echo "#define NUM_SEARCH ${search_array[$num]}" >> parameters_rw.h;
        echo "#define NO_EXECUTION ${execution_array[$num]}" >> parameters_rw.h;
        echo "#define RUNFLAG 0" >> parameters_rw.h;
        echo "#define FANOUT 4" >> parameters_rw.h
        echo "#define WINDOWSIZE 100000000" >> parameters_rw.h;
        echo "#define MAXSEG ${max_seg_array_fq[$index]}" >> parameters_rw.h;
        g++ test_rw_ratio.cpp -march=native -std=c++17 -O3 -w -o z_run_ratio && ./z_run_ratio ${file_array[$index]} >> log_flirt_rw_${rw_ratio_array[$num]}.txt
done
done

declare -a fanout_array_bp
fanout_array_bp=(256 256 16 64 256 64 64)

for index in "${!file_array[@]}"; do
    for num in "${!update_array[@]}"; do
        echo "BP TREE , ${file_array[$index]} , ${rw_ratio_array[$num]}";
        > parameters_rw.h;
        echo "#pragma once" >> parameters_rw.h;
        echo "#define ERROR 4" >> parameters_rw.h;
        echo "#define NUM_UPDATE ${update_array[$num]}" >> parameters_rw.h;
        echo "#define NUM_SEARCH ${search_array[$num]}" >> parameters_rw.h;
        echo "#define NO_EXECUTION ${execution_array[$num]}" >> parameters_rw.h;
        echo "#define RUNFLAG 1" >> parameters_rw.h;
        echo "#define FANOUT ${fanout_array_bp[$index]}" >> parameters_rw.h
        echo "#define WINDOWSIZE 100000000" >> parameters_rw.h;
        echo "#define MAXSEG 1" >> parameters_rw.h;
        g++ test_rw_ratio.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_ratio && ./z_run_ratio ${file_array[$index]} >> log_bp_pgm_rw_${rw_ratio_array[$num]}.txt
done
done


declare -a fanout_array_pgm
fanout_array_pgm=(16 4 4 4 16 4 16)

for index in "${!file_array[@]}"; do
    for num in "${!update_array[@]}"; do
        echo "PGM , ${file_array[$index]} , ${rw_ratio_array[$num]}";
        > parameters_rw.h;
        echo "#pragma once" >> parameters_rw.h;
        echo "#define ERROR 4" >> parameters_rw.h;
        echo "#define NUM_UPDATE ${update_array[$num]}" >> parameters_rw.h;
        echo "#define NUM_SEARCH ${search_array[$num]}" >> parameters_rw.h;
        echo "#define NO_EXECUTION ${execution_array[$num]}" >> parameters_rw.h;
        echo "#define RUNFLAG 2" >> parameters_rw.h;
        echo "#define FANOUT ${fanout_array_pgm[$index]}" >> parameters_rw.h
        echo "#define WINDOWSIZE 100000000" >> parameters_rw.h;
        echo "#define MAXSEG 1" >> parameters_rw.h;
        g++ test_rw_ratio.cpp -march=native -fopenmp -std=c++17 -O3 -w -o z_run_ratio && ./z_run_ratio ${file_array[$index]} >> log_bp_pgm_rw_${rw_ratio_array[$num]}.txt
done
done


for index in "${!file_array[@]}"; do
    for num in "${!update_array[@]}"; do
        echo "Queue , ${file_array[$index]} , ${rw_ratio_array[$num]}";
        > parameters_rw.h;
        echo "#pragma once" >> parameters_rw.h;
        echo "#define ERROR 4" >> parameters_rw.h;
        echo "#define NUM_UPDATE ${update_array[$num]}" >> parameters_rw.h;
        echo "#define NUM_SEARCH ${search_array[$num]}" >> parameters_rw.h;
        echo "#define NO_EXECUTION ${execution_array[$num]}" >> parameters_rw.h;
        echo "#define RUNFLAG 4" >> parameters_rw.h;
        echo "#define FANOUT 4" >> parameters_rw.h
        echo "#define WINDOWSIZE 100000000" >> parameters_rw.h;
        echo "#define MAXSEG 1" >> parameters_rw.h;
        g++ test_rw_ratio.cpp -march=native -std=c++17 -O3 -w -o z_run_ratio && ./z_run_ratio ${file_array[$index]} >> log_queue_rw_${rw_ratio_array[$num]}.txt
done
done


for index in "${!file_array[@]}"; do
    for num in "${!update_array[@]}"; do
        echo "ALEX , ${file_array[$index]} , ${rw_ratio_array[$num]}";
        > parameters_rw.h;
        echo "#pragma once" >> parameters_rw.h;
        echo "#define ERROR 4" >> parameters_rw.h;
        echo "#define NUM_UPDATE ${update_array[$num]}" >> parameters_rw.h;
        echo "#define NUM_SEARCH ${search_array[$num]}" >> parameters_rw.h;
        echo "#define NO_EXECUTION ${execution_array[$num]}" >> parameters_rw.h;
        echo "#define RUNFLAG 3" >> parameters_rw.h;
        echo "#define FANOUT 4" >> parameters_rw.h
        echo "#define WINDOWSIZE 100000000" >> parameters_rw.h;
        echo "#define MAXSEG 1" >> parameters_rw.h;
        g++ test_rw_ratio.cpp -march=native -std=c++17 -O3 -w -o z_run_ratio && ./z_run_ratio ${file_array[$index]} >> log_alex_rw_${rw_ratio_array[$num]}.txt
done
done