#!/usr/bin/python2
import matplotlib.pyplot as plt
import numpy as np
import math
import fileinput
import sys
import os

def main():

    N = 4
    infile = fileinput.input()
    pth_means = []
    omp_means = []
    seq_means = []

    for line in infile:
        pth_means.append(float(line.strip()))
        omp_means.append(float(next(infile).strip()))
        seq_means.append(float(next(infile).strip()))

    ind = np.arange(N)  # the x locations for the groups
    width = 0.25        # the width of the bars

    fig, ax = plt.subplots()

    rects1 = ax.bar(ind, pth_means , width, color='r')
    rects2 = ax.bar(ind + width, omp_means, width, color='yellow')
    rects3 = ax.bar(ind + width*2, seq_means, width, color='b')

    # add some text for labels, title and axes ticks
    ax.set_ylabel('Time [s]')
    ax.set_title('Regions x Time')
    ax.set_xticks(ind + width*1.5 )
    ax.set_xticklabels(('Elephant', 'Seahorse', 'Triple Spiral', 'Full'))

    ax.legend((rects1[0], rects2[0], rects3[0]), ('pth 2t', 'omp 2t', 'seq w/o io'))

    def autolabel(rects):
        """
        Attach a text label above each bar displaying its height
        """
        for rect in rects:
            height = rect.get_height()
            ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                    '%d' % int(height),
                    ha='center', va='bottom')

    autolabel(rects1)
    autolabel(rects2)
    autolabel(rects3)

    plt.ylim([0, 130])
    plt.show()

if __name__ == '__main__':
  main()
