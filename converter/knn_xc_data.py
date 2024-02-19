import pynndescent
import numpy as np
import math
from scipy.spatial.distance import cdist


base_dir = "../xc_data"


def write_edges(edges, num_nodes, filename, dataset):
    file = open(filename,'w')
    file.write(
    '''# Undirected weighted graph: {dataset}\n# Nodes: {nodes} Edges: {edges}\n# FromNodeId    ToNodeId    Weight\n'''.format(dataset=dataset,nodes=num_nodes, edges=len(edges)))

    for edge in edges:
        i,j,d = edge
        file.write(str(i) + '\t' + str(j) + '\t'+ str(d) + '\n')
    file.close()

def get_knn_graph_cosine(data):
    index = pynndescent.NNDescent(data, metric="cosine", 
                              n_neighbors=101, 
                              pruning_degree_multiplier=2, 
                              diversify_prob=0)
    index.prepare()
    knns = index.neighbor_graph[0]
    dist = index.neighbor_graph[1] # 1-cos sim distances
    return knns, dist

def get_edges(knns, dist, k):
#     # normalize to only 
#     dist = dist[:,1:k+1]
#     knns = knns[:,1:k+1]
#     max_similarity = dist.max()
#     print(max_similarity)
#     dist /= max_similarity
    edges = []
    for i in range(len(knns)):
        for j in range(k+1): # exclude itself
            if i != knns[i][j]:
                edges.append([i, knns[i][j], 1-dist[i][j]])
    return edges


datasets = [
    "AmazonTitles",
    # "WikiSeeAlsoTItles",
    # "Amazon",
    # "WikiTitles",
]

for dataset in datasets:
  data = np.load(base_dir+f"/{dataset}.npy")
  knns, dist = get_knn_graph_cosine(data)
  num_nodes = len(data)
  for k in [10, 50, 100]:
      edges = get_edges(knns, dist, k)
      write_edges(edges, num_nodes, f'{base_dir}/{dataset}_k{k}.ungraph.txt', f"{dataset}_k{k}")