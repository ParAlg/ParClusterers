#!/usr/bin/env python3

import os
from pathlib import Path
import sys
import numpy as np

import dpc_ann

# Change to DPC-ANN folder and add to path
abspath = Path(__file__).resolve().parent.parent
os.chdir(abspath)
sys.path.append(str(abspath))

def convert_to_cmty_format_helper(numpy_array, ground_truth_output):
  # Initialize an empty dictionary to store grouped indices
  grouped_indices = {}

  # Enumerate through the array and group indices by their values
  for idx, value in enumerate(numpy_array):
      if value in grouped_indices:
          grouped_indices[value].append(idx)
      else:
          grouped_indices[value] = [idx]

  # Convert the dictionary values (lists of grouped indices) into a list of groups
  groups = list(grouped_indices.values())
  
  file = open(ground_truth_output,'w')
  for cluster_list in groups:
    file.write("\t".join(str(x) for x in cluster_list) + "\n")
  file.close()

def convert_to_cmty_format(ground_truth_input, ground_truth_output):
  numpy_array = np.load(ground_truth_input).flatten()
  convert_to_cmty_format_helper(numpy_array, ground_truth_output)

# digits = datasets.load_digits()
# n_samples = len(digits.images)
# data = digits.images.reshape((n_samples, -1)).astype("float32")
# knn_graph_path = "results/knn_graphs/digits.graph.txt"

########### KNN Graph for MNIST ####################
# input_file_path = "/home/sy/ParDPC/data/mnist/mnist.npy"
# ground_truth_input = "/home/sy/ParDPC/data/mnist/mnist_gt.npy"
# data_name = "mnist"
# ground_truth_output = "knn_graphs/%s/%s.cmty" % (data_name, data_name)
# data = np.load(input_file_path)
# print("data loaded")
# ks = [10, 50]
# for k in ks:
#   knn_graph_path = "knn_graphs/%s/%s_k%s.graph.txt" % (data_name, data_name, k)
#   dpc_ann.dpc_numpy(
#               graph_type="BruteForce",
#               knn_graph_path=knn_graph_path,
#               data=data,
#               K=k,
#               center_finder=dpc_ann.ProductCenterFinder(num_clusters=1),
#           )
# result = convert_to_cmty_format(ground_truth_input, ground_truth_output)

# ########### KNN Graph for ImageNet ####################
# input_file_path = "/home/sy/mount-data/imagenet/imagenet.npy"
# data_name = "imagenet"
# ground_truth_input = "/home/sy/mount-data/imagenet/imagenet_gt.npy"
# ground_truth_output = "knn_graphs/%s.cmty" % (data_name)

# data = np.load(input_file_path)
# print("data loaded")
# ks = [10, 50, 100, 500]
# for k in ks:
#   knn_graph_path = "knn_graphs/%s_k%s.graph.txt" % (data_name, k)
#   params = {
#             "max_degree": 256,
#             "alpha": 1.1,
#             "Lbuild": 256,
#             "L": 256,
#             "Lnn": 256,
#           }
#   dpc_ann.dpc_numpy(
#             graph_type="Vamana",
#             knn_graph_path=knn_graph_path,
#             data=data,
#             K=k,
#             center_finder=dpc_ann.ProductCenterFinder(num_clusters=1),
#             **params
#         )

# result = convert_to_cmty_format(ground_truth_input, ground_truth_output)

########### KNN Graph for others ####################

for data_name in ["amazon_polarity", "arxiv-clustering-p2p", 
                  "arxiv-clustering-s2s", "reddit-clustering", 
                  "stackexchange-clustering", "wikipedia"]:
    
    input_file_path = "/home/sy/embeddings/%s/%s.npy" % (data_name, data_name)
    ground_truth_input = "/home/sy/embeddings/%s/%s.gt" % (data_name, data_name)
    ground_truth_output = "knn_graphs/%s.cmty" % (data_name)
    numpy_array = np.loadtxt(ground_truth_input)
    print(numpy_array)

    data = np.load(input_file_path)
    print("data loaded")
    ks = [10, 50, 100]
    for k in ks:
      knn_graph_path = "knn_graphs/%s_k%s.graph.txt" % (data_name, k)
      params = {
                "max_degree": 256,
                "alpha": 1.1,
                "Lbuild": 256,
                "L": 256,
                "Lnn": 256,
              }
      dpc_ann.dpc_numpy(
                graph_type="Vamana",
                knn_graph_path=knn_graph_path,
                data=data,
                K=k,
                center_finder=dpc_ann.ProductCenterFinder(num_clusters=1),
                **params
            )

    result = convert_to_cmty_format_helper(numpy_array, ground_truth_output)


# bazel run //utils:snap_converter -- -s -i /home/sy/ParClusterers/knn_graphs/mnist_k10.graph.txt -o /home/sy/ParClusterers/knn_graphs/mnist_k10.gbbs.txt
# bazel run //utils:snap_converter -- -s -i /home/sy/ParClusterers/knn_graphs/mnist_k50.graph.txt -o /home/sy/ParClusterers/knn_graphs/mnist_k50.gbbs.txt

# bazel run //utils:snap_converter -- -s -i /home/sy/ParClusterers/knn_graphs/imagenet_k10.graph.txt -o /home/sy/ParClusterers/knn_graphs/imagenet_k10.gbbs.txt
# bazel run //utils:snap_converter -- -s -i /home/sy/ParClusterers/knn_graphs/imagenet_k50.graph.txt -o /home/sy/ParClusterers/knn_graphs/imagenet_k50.gbbs.txt
# bazel run //utils:snap_converter -- -s -i /home/sy/ParClusterers/knn_graphs/imagenet_k100.graph.txt -o /home/sy/ParClusterers/knn_graphs/imagenet_k100.gbbs.txt
