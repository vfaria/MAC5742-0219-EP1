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
    t_sem = []
    t_com = []

    for line in infile:
        t_sem.append(float(line.strip()) * 1000)
        t_com.append(float(next(infile).strip()) * 1000)

    ind = np.arange(N)  # the x locations for the groups
    width = 0.25        # the width of the bars

    fig, ax = plt.subplots()

    rects1 = ax.bar(ind, t_sem , width, color='r')
    rects2 = ax.bar(ind + width, t_com, width, color='yellow')

    # add some text for labels, title and axes ticks
    ax.set_ylabel('Time [ks]')
    ax.set_title('')
    ax.set_xticks(ind + width)
    ax.set_xticklabels(('Elephant', 'Seahorse', 'Triple Spiral', 'Full'))

    ax.legend((rects1[0], rects2[0]), ('t_sem', 't_com'))

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

    plt.ylim([0, 140000])
    plt.show()

if __name__ == '__main__':
  main()
