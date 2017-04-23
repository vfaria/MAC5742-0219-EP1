#! /bin/bash

set -o xtrace

N_POWER_THREADS=$2
MEASUREMENTS=10
ITERATIONS=$1
INITIAL_SIZE=16

SIZE=$INITIAL_SIZE

NAMES=('mandelbrot_pth' 'mandelbrot_omp' 'mandelbrot_seq' 'mandelbrot_seq_without_io')

export OMP_NUM_THREADS=$N_THREADS

make
mkdir results

for NAME in ${NAMES[@]}; do
    mkdir results/$NAME

    if [[ $NAME =~ "seq" ]]; then
        for ((i=1; i<=$ITERATIONS; i++)); do
            perf stat -r $MEASUREMENTS ./$NAME -2.5    1.5   -2.0    2.0   $SIZE >> full_$NAME.log          2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.8   -0.7    0.05   0.15  $SIZE >> seahorse_$NAME.log      2>&1
            perf stat -r $MEASUREMENTS ./$NAME  0.175  0.375 -0.1    0.1   $SIZE >> elephant_$NAME.log      2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012  0.554  0.754 $SIZE >> triple_spiral_$NAME.log 2>&1
            SIZE=$(($SIZE * 2))
        done
        SIZE=$INITIAL_SIZE
    else
        N_THREADS=1
        export OMP_NUM_THREADS=$N_THREADS
        for ((t=1; t<=$N_POWER_THREADS; t++)); do
            for ((i=1; i<=$ITERATIONS; i++)); do
                perf stat -r $MEASUREMENTS ./$NAME -2.5    1.5   -2.0    2.0   $SIZE $N_THREADS >> full_$NAME\_$(($N_THREADS))t.log          2>&1
                perf stat -r $MEASUREMENTS ./$NAME -0.8   -0.7    0.05   0.15  $SIZE $N_THREADS >> seahorse_$NAME\_$(($N_THREADS))t.log      2>&1
                perf stat -r $MEASUREMENTS ./$NAME  0.175  0.375 -0.1    0.1   $SIZE $N_THREADS >> elephant_$NAME\_$(($N_THREADS))t.log      2>&1
                perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012  0.554  0.754 $SIZE $N_THREADS >> triple_spiral_$NAME\_$(($N_THREADS))t.log 2>&1
                SIZE=$(($SIZE * 2))
            done
            N_THREADS=$(($N_THREADS * 2))
            export OMP_NUM_THREADS=$N_THREADS
            SIZE=$INITIAL_SIZE
        done
    fi

    mv *.log results/$NAME
    rm output.ppm
done
