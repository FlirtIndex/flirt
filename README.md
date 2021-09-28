# FLIRT

## Description

Source Code
- *alex*: contains ALEX from (https://github.com/microsoft/ALEX).
- *flirt*: contains FLIRT and FLIRT Multithread.
- *pgm*: contains the PGM Index from (https://github.com/gvinciguerra/PGM-index).
- *queue*: contains a circular queue implementation.
- *stx*: contains the STX B+ Tree v0.9 from (https://github.com/bingmann/stx-btree).

Test Code
- *test*: contains all the test codes for experiments.
- *test_bash*: conatins the scripts to run the test code.
- *test_parameters*: parameter header file to control the parameters in each experiment (Automatically generated by test scipts).
- *test_pbs*: contains the scripts for running the scripts on a HPC cluster.
- *test_util*: contains utility functions for the experiments (loading, PerfEvent, etc.)

## Running the test files
Run the `move_sub_to_current.sh`script to move all files to main directory. Run any of the test scripts with `./run_xxx.sh`. Alternatively, submit a job with the pbs files on a HPC cluster with `qsub A_XXXX.pbs`. The test dataset is not included.

## Dataset
The dataset used are the SOSD datasets. Specifically the 200M unit64 datasets. Remember to rename the files to the following names and move them to the main directory or change the names in the test scripts.

- books_200M_unit64 -> f_books
- fb_200M_uint64 -> f_fb
- lognormal_200M_unit64 -> f_lognormal
- normal_200M_uint64 -> f_normal
- osm_cellids_200M_uint64 -> f_osm
- uniform_dense_200M_uint64 -> f_udense
- uniform_sparse_200M_uint64 -> f_sparse
