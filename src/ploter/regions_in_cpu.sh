#!/bin/bash
REGIONS=(elephant seahorse triple_spiral full)
for i in "${REGIONS[@]}"
do
    cat ../gce_results/mandelbrot_pth/$i\_mandelbrot_pth_32t.log | awk '/CPU/ {print $1}' | tail -n 1
    cat ../gce_results/mandelbrot_omp/$i\_mandelbrot_omp_32t.log | awk '/CPU/ {print $1}' | tail -n 1
    cat ../gce_results/mandelbrot_seq_without_io/$i\_mandelbrot_seq_without_io.log | awk '/CPU/ {print $1}' | tail -n 1
done