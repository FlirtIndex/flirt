# Bash script for running experiments on Queue
for filename in f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse; do
    for windowsize in 10000 1000000 100000000; do
        echo "$filename , 4 , 4 , $windowsize";
        > parameters_queue.h;
        echo "#pragma once" >> parameters_queue.h;
        echo "#define NO_EXECUTION 10000" >> parameters_queue.h;
        echo "#define ERROR 4" >> parameters_queue.h;
        echo "#define FANOUT 4" >> parameters_queue.h;
        echo "#define WINDOWSIZE $windowsize" >> parameters_queue.h;
        g++ test_queue.cpp -std=c++17 -march=native -O3 -w -o z_run_queue && ./z_run_queue $filename >> log_queue.txt
done
done