#!/usr/bin/python2
import matplotlib.pyplot as plt
import numpy as np
import math
import fileinput
import sys
import os

def main():
    xi    = 1
    x, y1, y2, stdev1, stdev2, e1, e2     = [], [], [], [], [], [], []
     
    title = '#threads x CPU utilized (N=8192, fig='+ os.environ['FIGNAME'] +')' 
    
    infile = fileinput.input()

    for line in infile:
        pth_measure = line.strip('\n').split('\t')
        omp_measure = next(infile).strip('\n').split('\t')

        y1.append(float(pth_measure[0]))
        y2.append(float(omp_measure[0]))

        stdev1.append(float(pth_measure[1].replace('%', '')) / 100.0)
        stdev2.append(float(omp_measure[1].replace('%', '')) / 100.0)

        x.append(xi)
        xi *= 2

    for i in range(len(y1)):
        e1.append(math.fabs(y1[i] * stdev1[i]))
        e2.append(math.fabs(y2[i] * stdev2[i]))

    fn1 = plt.errorbar(x, y1, e1, linestyle='solid', marker='^')
    fn2 = plt.errorbar(x, y2, e2, linestyle='solid', marker='^')

    plt.legend([fn1, fn2], ['pth', 'omp'])

    plt.xlabel('#threads')
    plt.ylabel('CPU utilized')
    plt.title(title)
    plt.show()
    # plt.savefig(os.environ['EP1219'] + '/ploter/pictures/' + title + '.png')

if __name__ == '__main__':
  main()
