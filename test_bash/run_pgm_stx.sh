# Bash script for running experiments on PGM and STX B+ Tree
for filename in f_books f_fb f_lognormal f_normal f_osm f_udense f_usparse; do
    for err in 4; do
    # for err in 4 16 64 256 1024 4096; do
        for fanout in 4 16 64 256 1024 4096; do
            for windowsize in 10000 1000000 100000000; do
                echo "$filename , $err , $fanout , $windowsize";
		        > parameters_bl.h;
                echo "#pragma once" >> parameters_bl.h;
                echo "#define NO_EXECUTION 10000" >> parameters_bl.h;
                echo "#define ERROR $err" >> parameters_bl.h;
                echo "#define FANOUT $fanout" >> parameters_bl.h;
                echo "#define WINDOWSIZE $windowsize" >> parameters_bl.h;
                g++ test_pgm_stx.cpp -std=c++17 -march=native -fopenmp -O3 -w -o z_run_pgm_stx && ./z_run_pgm_stx $filename >> log_pgm_stx.txt
done
done
done
done