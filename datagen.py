#!/usr/bin/python3

import sys
import math
import random
import numpy as np

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print(f"Argument missing: {sys.argv[0]} <size>")
        quit()

    size = int(sys.argv[1])
    A = np.full((size, size), '0')
    x1 = random.randint(0, size/2)
    x2 = random.randint(x1, size-1)
    y1 = random.randint(0, size/2)
    y2 = random.randint(y1, size-1)
    angle = math.atan2(y2 - y1, x2 - x1) * 180 / math.pi

    x = x1
    y = y1
    count = 0
    while(x < x2 and y < y2):
        A[int(x)][int(y)] = '2'
        count += 1
        x += math.cos(angle)
        y += math.sin(angle)

        print(x, y)

    A[size-1][random.randint(0, size-1)] = '1'
 
    with open("map.txt", "w") as f:
            np.savetxt(f, A.astype(int), fmt='%d', delimiter='')
    A[A=='0'] = ' '
    print(A)       
    print(f"{count} counts of 2's")
