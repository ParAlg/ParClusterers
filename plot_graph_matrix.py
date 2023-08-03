import time
import io
import pandas
import numpy as np
import sys
import matplotlib.pyplot as plt

n = int(sys.argv[1])
filename = sys.argv[2]

def readGraph(filename):
  matrix = np.zeros((n,n))
  with open(filename, "r") as in_file:
    for line in in_file:
      line = line.strip()
      if not line:
        continue
      if line[0] == '#':
        continue
      split = [x.strip() for x in line.split('\t')]
      if split:
        a = int(split[0])
        b = int(split[1])
        matrix[a][b] = 1
        matrix[b][a] = 1

    
  plt.imshow(matrix, cmap='hot', interpolation='nearest')
  plt.title('Heat Map')
  plt.xlabel('Column')
  plt.ylabel('Row')
  plt.savefig("/home/ubuntu/outputs/tmp.png")

readGraph("/home/ubuntu/outputs/%s.ungraph.txt" % filename)