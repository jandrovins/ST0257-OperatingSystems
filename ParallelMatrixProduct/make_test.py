#!/usr/bin/env python

import random
import numpy as np
import sys
import subprocess as sp
import time
import os

np.set_printoptions(threshold=sys.maxsize)

numTests = 1000
successTests = 0 
for i in range(numTests):
    N = random.randint(100, 110)
    M = random.randint(100, 110)
    K = random.randint(100, 110)
    
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

    #print(A)
    #print(B)
    #print(buffer)

    child = sp.run(["./matrixMultiplication"], input=buffer, shell=True, text=True, capture_output=True, check=True)

    OurMatMult = [float(i) for i in child.stdout.split()]
    OurMatMult = np.reshape(np.array(OurMatMult), (N,K))

    if np.allclose(OurMatMult, result):
        print(f"SUCCESS ON TEST {i}")
        successTests +=1
    else:
        print(f"ERROR ON TEST {i}:")

print("###########################################")
print(f"TESTS SUCCEDED: {successTests}")
print(f"TESTS FAILED: {numTests-successTests}")
print("###########################################")
