import pandas as pd
import sys
from sklearn.neighbors import kneighbors_graph
from sklearn import datasets, metrics, svm
from sklearn.datasets import fetch_olivetti_faces
from numpy.random import RandomState
import os
import glob
import cv2

def convert_to_weighted(input, config):
    config = open(config,'r')
    config_info = config.readlines()
    col = -1
    remove = []
    for line in config_info:
        if line.startswith('label'):
            col = int(line.split(' ')[-1].strip())
        elif line.startswith('remove'):
           for elem in line.split(' ')[-1].strip().split(';'):
              remove.append(int(elem))
    data = pd.DataFrame()

    print(remove)
    faces = None
    digits = None
    faces_targets = None

    if input == "digits":
      digits = datasets.load_digits()
      # flatten the images
      n_samples = len(digits.images)
      data = digits.images.reshape((n_samples, -1))
      data = pd.DataFrame(data)
    elif input == "faces":
      # random number used for data shuffling
      rng = RandomState(0)
      faces, faces_targets = fetch_olivetti_faces(return_X_y=True, shuffle=True, random_state=rng)
      n_samples, n_features = faces.shape

      # Global centering (focus on one feature, centering all samples)
      faces_centered = faces - faces.mean(axis=0)

      # Local centering (focus on one sample, centering all features)
      faces_centered -= faces_centered.mean(axis=1).reshape(n_samples, -1)
      data = faces_centered
      data = pd.DataFrame(data)

    else:
      data = pd.read_csv(input, header = None)
      print("ground truth col: ", col)
      print(data.head()[col])

      
    if input in ["digits", "faces"]:
       sub_data = data
    else:
      sub_data = data.drop(data.columns[[col]+remove], axis =1)
    print("data head: ")
    print(sub_data.head())

    res = kneighbors_graph(sub_data, 10, mode='distance', include_self='auto')
    max_similarity = res.max()
    # Divide each non-zero similarity in the matrix by this maximum value.
    res /= max_similarity

    name = input.split('/')[-1].split('.')[0]
    file = open(name + '.graph.txt','w')
    file.write(
'''# Undirected weighted graph: {input}
# {name}
# Nodes: {nodes} Edges: {edges}
# FromNodeId	ToNodeId    Weight
'''.format(input=input,name=name,nodes=data.shape[0], edges=res.count_nonzero()))

    for i in range(res.shape[0]):
        for j in res[i].nonzero()[1]:
            file.write(str(i) + '\t' + str(j) + '\t'+ str(1/ (1 + res[i,j])) + '\n')
    file.close()

    file = open(name + '.cmty.txt','w')
    if input == "digits":
      result = data.reset_index().groupby(digits.target)['index'].apply(list).tolist()
    elif input == "faces":
      result = data.reset_index().groupby(faces_targets)['index'].apply(list).tolist()
    else:
      result = data.reset_index().groupby(data[data.columns[col]])['index'].apply(list).tolist()
    if not (result is None):
      for cluster_list in result:
        file.write("\t".join(str(x) for x in cluster_list) + "\n")
    file.close()



def main():
  args = sys.argv[1:]
  convert_to_weighted(args[0], args[1])

if __name__ == "__main__":
  main()