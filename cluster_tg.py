import pyTigerGraph as tg
import json
import sys
import csv
import time
import io
from contextlib import redirect_stdout
import runner_utils



def readGraph(filename):
  print_index = 1000000
  index_track = 0
  nodes = set()
  edges_from = []
  edges_to = []
  weights = []
  with open(filename, "r") as in_file:
    for line in in_file:
      if index_track % print_index == 0:
        print("We're at: " + str(index_track))
        sys.stdout.flush()
      index_track = index_track + 1
      line = line.strip()
      if not line:
        continue
      if line[0] == '#':
        continue
      split = [x.strip() for x in line.split('\t')]
      if split:
        a = split[0]
        b = split[1]
        w = 1
        if len(split) == 3:
          w = split[2]
          weights.append(float(w))
          weights.append(float(w))
        nodes.add(int(a))
        nodes.add(int(b))
        edges_from.append(int(a))
        edges_to.append(int(b))
        edges_from.append(int(b))
        edges_to.append(int(a))
        
  return nodes, edges_from, edges_to, weights

def load_tigergraph(conn, filename, input_dir, output_dir):

  conn.graphname = 'current_graph'

  nodes, edges_from, edges_to, weights = readGraph(input_dir + filename)

  node_list = list(nodes)
  with open(output_dir + filename + '_nodes.csv', 'w') as f:
      
      writer = csv.writer(f)
      writer.writerow(['Number', 'ID'])

      for i in range(len(node_list)):
        writer.writerow([i, node_list[i]])

  with open(output_dir + filename + '_edges.csv', 'w') as f:
      
      writer = csv.writer(f)
      if len(weights) == 0:
        writer.writerow(['Number', 'Node1', 'Node2'])

        for i in range(0, len(edges_to), 2):
          writer.writerow([i/2, edges_to[i], edges_to[i+1]])
        print(conn.gsql('''
        CREATE VERTEX Node (id INT PRIMARY KEY)
        CREATE UNDIRECTED EDGE Undirected_Edge (FROM Node, TO Node)
        CREATE GRAPH current_graph (Node, Undirected_Edge)
        USE GRAPH current_graph
        CREATE LOADING JOB job1 FOR GRAPH current_graph {{
          LOAD "{nodes}" to VERTEX Node VALUES ($"ID") USING HEADER = "TRUE";
          LOAD "{edges}" to EDGE Undirected_Edge VALUES ($"Node1", $"Node2") USING HEADER = "TRUE";
        }}
        RUN LOADING JOB job1'''.format(nodes = output_dir + filename + '_nodes.csv', edges = output_dir + filename + '_edges.csv')))
      else: 
        writer.writerow(['Number', 'Node1', 'Node2', 'Weight'])

        for i in range(0, len(edges_to), 2):
          writer.writerow([i/2, edges_to[i], edges_to[i+1], weights[int(i/2)]])
        print(conn.gsql('''
        CREATE VERTEX Node (id INT PRIMARY KEY)
        CREATE UNDIRECTED EDGE Undirected_Weighted_Edge (FROM Node, TO Node, weight FLOAT)
        CREATE GRAPH current_graph (Node, Undirected_Weighted_Edge)
        USE GRAPH current_graph
        CREATE LOADING JOB job1 FOR GRAPH current_graph {{
          LOAD "{nodes}" to VERTEX Node VALUES ($"ID") USING HEADER = "TRUE";
          LOAD "{edges}" to EDGE Undirected_Weighted_Edge VALUES ($"Node1", $"Node2", $"Weight") USING HEADER = "TRUE";
        }}
        RUN LOADING JOB job1'''.format(nodes = output_dir + filename + '_nodes.csv', edges = output_dir + filename + '_edges.csv')))
  

def run_tigergraph(conn, clusterer, out_clustering, thread, config, weighted):

  feat = conn.gds.featurizer()
  
  
  threshold = -1
  edge = 'Undirected_Weighted_Edge' if weighted else 'Undirected_Edge'
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("threshold"):
        if config_split[1] != "None":
          threshold = float(config_split[1])

  f = io.StringIO()
  with redirect_stdout(f):
    start_time = time.time()
    if clusterer == 'TigerGraphKCore':
      params = {
        "v_type": "Node",
        "e_type": edge,
        "result_attribute": "cluster",
        "k_max": threshold
      }
      res = feat.runAlgorithm("tg_kcore", params=params, threadLimit = thread)
    elif clusterer == 'TigerGraphLouvain':
      params = {
        "v_type_set": ["Node"],
        "e_type_set": [edge],
        "result_attribute": "cluster"
      }
      if weighted:
        params['wt_attr'] = 'weight'
      res = feat.runAlgorithm("tg_louvain", params=params, threadLimit = thread)
    elif clusterer == 'TigerGraphWCC':
      params = {
        "v_type_set": ["Node"],
        "e_type_set": [edge],
        "result_attribute": "cluster"
      }
      res = feat.runAlgorithm("tg_wcc", params=params, threadLimit = thread)
    elif clusterer == 'TigerGraphLabelProp':
      params = {
        "v_type_set": ["Node"],
        "e_type_set": [edge],
        "result_attribute": "cluster"
      }
      res = feat.runAlgorithm("tg_label_prop", params=params, threadLimit = thread)

    end_time = time.time()
    
    print("Cluster Time: " + str(end_time - start_time))

    df = conn.getVertexDataFrame("Node")
    result = df.groupby('cluster')['id'].apply(list).tolist()
    if not (result is None):
      for cluster_list in result:
        runner_utils.appendToFile("\t".join(str(x) for x in cluster_list) + "\n", out_clustering)

    end_time = time.time()
    
    print("Total Time: " + str(end_time - start_time))
  out = f.getvalue()
  return out

def remove_tigergraph(conn):
  print(conn.gsql("DROP ALL"))



if __name__ == "__main__":
  conn = tg.TigerGraphConnection(
      host='http://127.0.0.1',
      username='tigergraph',
      password='tigergraph',
  )
  load_tigergraph(conn, 'iris_graph.txt', '/home/jamisonmeindl/ParClusterers/', '/home/jamisonmeindl/ParClusterers/')
  remove_tigergraph(conn)