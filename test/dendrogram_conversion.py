import numpy as np
import os
# import requests
import tempfile
import time
import argparse
import math
import struct
import sklearn
# from sklearn import datasets

data_dir = "dendrograms"
os.makedirs(data_dir, exist_ok=True)

parser = argparse.ArgumentParser(description='Convert from sklearn UCI datasets to fvecs format.')
parser.add_argument('-input', type=str, help='input file in pbbs format')
parser.add_argument('-output', type=str, help='ouput file name')
parser.add_argument('-n', type=str, help='number of input points')

args = parser.parse_args()

n = int(args.n)

with open(args.input) as infile:
  lines = infile.readlines()
  with open(args.output, "w") as f:
    parents = [(-1, -1)]*(2*n-1)
    cur_id = n
    min_nonzero_dist = 100000000000
    for line in lines:
      l = line.strip().split(" ")
      fst = int(l[0])
      snd = int(l[1])
      wgh = float(l[2])
      parents[fst] = (cur_id, wgh)
      parents[snd] = (cur_id, wgh)
      if (wgh != 0):
        min_nonzero_dist = min(min_nonzero_dist, wgh)
      cur_id += 1

    print(min_nonzero_dist)
    for i in range(2*n-2):
      dist = parents[i][1]
      if (dist == 0):
        dist = min_nonzero_dist
      sim = 1 / (1 + dist)
      st = str(i) + " " + str(parents[i][0]) + " " + str(sim)
      if (i < 2*n-3):
        st += "\n"
      f.write(st)
      assert(parents[i][0] != -1)
    print("done")