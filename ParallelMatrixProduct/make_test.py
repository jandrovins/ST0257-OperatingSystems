#!/usr/bin/env python

import random
import numpy as np
import sys
import subprocess as sp
import time
import os

np.set_printoptions(threshold=sys.maxsize)

for i in range(100):
    N = random.randint(2, 5)
    M = random.randint(2, 5)
    K = random.randint(2, 5)
    
    A = np.random.rand(N, M)
    B = np.random.rand(M, K)

    result = np.matmul(A, B)
    buffer = ""
    buffer += f"{A.shape[0]} {A.shape[1]}\n" # A's shape
    for row in A:
        for num in row:
            buffer += f"{num} "
    buffer += f"\n{B.shape[0]} {B.shape[1]}\n"
    for row in B:
        for num in row:
            buffer += f"{num} "
    buffer+="\n"

    print(A)
    print(B)
    print(buffer)

    child = sp.run(["./parameters"], input=buffer, shell=True, text=True, capture_output=True, check=True)

    OurMatMult = [float(i) for i in child.stdout.split()]
    OurMatMult = np.reshape(np.array(OurMatMult), (N,K))

    if np.allclose(OurMatMult, result):
        print(f"SUCCESS ON TEST {i}")
    else:
        print(f"ERROR ON TEST {i}:")
        #print(buffer, end="\n\n")
        #print(child.stdout, end="\n\n")
