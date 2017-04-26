#!/usr/bin/python2
import matplotlib.pyplot as plt
import numpy as np
import math
import fileinput
import sys
import os

def main():
    power = 4
    x, y1, stdev1, e1, y2, stdev2, e2 = [], [], [], [], [], [], []

    title = 'Seq vs. seq without io'

    with open(sys.argv[1]) as f:
        lines1 = f.readlines()

    with open(sys.argv[2]) as f:
        lines2 = f.readlines()

    lines1 = [l.strip() for l in lines1]
    lines2 = [l.strip() for l in lines2]

    for i in range(len(lines1)):
        line1 = lines1[i].split(' ')
        line2 = lines2[i].split(' ')

        y1.append(float(line1[0]))
        y2.append(float(line2[0]))

        stdev1.append(float(line1[1].replace('%', '')) / 100.0)
        stdev2.append(float(line2[1].replace('%', '')) / 100.0)

        x.append(2 ** power)

        power += 1

    log2 = lambda x: math.log(x, 2)
    x = map(log2, x)
    y1 = map(log2, y1)
    y2 = map(log2, y2)

    for i in range(len(y1)):
        e1.append(math.fabs(y1[i] * stdev1[i]))
        e2.append(math.fabs(y2[i] * stdev2[i]))

    # plt.ylim([0,150])

    plt.errorbar(x, y1, e1, linestyle='solid', marker='^')
    plt.errorbar(x, y2, e2, linestyle='solid', marker='^')
    plt.xlabel('lg N')
    plt.ylabel('lg (Time) [s] - AVG of 10 runs')
    plt.title(title)
    plt.show()
    # plt.savefig(os.environ['EP1219'] + '/ploter/pictures/' + title + '.png')

if __name__ == '__main__':
  main()
