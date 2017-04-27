#!/bin/bash
i=0
while [ $i -le 32 ] 
do
   # cat ../gce_results/mandelbrot_pth/$1\_mandelbrot_pth_"$i"t.log | awk '/elapsed/ {print $1"\t"$7}'
    let i=$($i*2)
    echo
done;
