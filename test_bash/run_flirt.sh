# Bash script for running experiments on Flirt
while IFS="," read -r index filename error windowsize maxseg
do
    echo "$filename , $err , $fanout , $windowsize, $maxseg";
    > parameters_flirt.h;
    echo "#pragma once" >> parameters_flirt.h;
    echo "#define NO_EXECUTION 10000" >> parameters_flirt.h;
    echo "#define ERROR $error" >> parameters_flirt.h;
    echo "#define FANOUT 4" >> parameters_flirt.h;
    echo "#define WINDOWSIZE $windowsize" >> parameters_flirt.h;
    echo "#define MAXSEG $maxseg" >> parameters_flirt.h;
    g++ test_flirt.cpp -std=c++17 -march=native -O3 -w -o z_run_flirt && ./z_run_flirt $filename >> log_flirt.txt
done < <(tail -n +2 MaxSeg.csv)