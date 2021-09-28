# Bash script for running experiments on Flirt configured to compare with multithreaded workload
for filename in f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse; do
    for err in 4 16 64 256 1024 4096; do
        echo "$filename , $err , 4 , 50000000, 50000000";
        > parameters_flirt.h;
        echo "#pragma once" >> parameters_flirt.h;
        echo "#define NO_EXECUTION 50000000" >> parameters_flirt.h;
        echo "#define ERROR $err" >> parameters_flirt.h;
        echo "#define FANOUT 4" >> parameters_flirt.h;
        echo "#define WINDOWSIZE 50000000" >> parameters_flirt.h;
        echo "#define MAXSEG 50000000" >> parameters_flirt.h;
        g++ test_flirt_mt.cpp -std=c++17 -march=native -O3 -w -o z_run_flirt && ./z_run_flirt $filename >> log_flirt_mt.txt
done
done