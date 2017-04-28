#!/bin/bash
i=1
while [[ $i -le 32 ]]
do
    # PTH BIGGEST SIZE MEASUREMENTS
    cat ../gce_results/mandelbrot_pth/$1\_mandelbrot_pth_"$i"t.log | awk '/elapsed/ {print $1"\t"$7}' | tail -n 1


    # OMP BIGGEST SIZE MEASUREMENTS
    cat ../gce_results/mandelbrot_omp/$1\_mandelbrot_omp_"$i"t.log | awk '/elapsed/ {print $1"\t"$7}' | tail -n 1

    i=$(($i *2))
done
