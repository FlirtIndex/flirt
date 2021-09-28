# Bash script for running experiments on Flirt Multithread
# Testing different multithreading configurations (Results not in paper)
for filename in f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse; do
    for err in 4 16 64 256 1024 4096; do
        for no_threads in 4 6 8 10 12 14 16 18 20 22 24; do
            echo "$filename , $err , 4 , $no_threads";
            > parameters_flirt_mt.h;
            echo "#pragma once" >> parameters_flirt_mt.h;
            echo "#define NO_EXECUTION 50000000" >> parameters_flirt_mt.h;
            echo "#define ERROR $err" >> parameters_flirt_mt.h;
            echo "#define FANOUT 4" >> parameters_flirt_mt.h;
            echo "#define WINDOWSIZE 50000000" >> parameters_flirt_mt.h;
            echo "#define SEGMENT_LIMIT 1000000" >> parameters_flirt_mt.h;
            echo "#define NO_THREADS $no_threads" >> parameters_flirt_mt.h;
            g++ test_flirt_multithread_ssu.cpp -march=native -lboost_thread -lboost_system -std=c++17 -O2 -pthread -w -o z_run_flirt_multi && ./z_run_flirt_multi $filename >> log_flirt_multithread_ssu.txt
done
done
done