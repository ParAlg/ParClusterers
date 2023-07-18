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

# # Read in DB configs
# with open('../config.json', "r") as config_file:
#     config = json.load(config_file)


def load_tigergraph(conn, filename, input_dir, output_dir):

  conn.graphname = 'current_graph'

  conn.gsql('CREATE VERTEX Node (id INT PRIMARY KEY)')
  conn.gsql('CREATE UNDIRECTED EDGE Undirected_Edge (FROM Node, TO Node)')
  conn.gsql("CREATE GRAPH current_graph (Node, Undirected_Edge)")

  nodes, edges_from, edges_to, weights = readGraph(input_dir + filename)

  node_list = list(nodes)
  with open(output_dir + filename + '_nodes.csv', 'w') as f:
      
      writer = csv.writer(f)
      writer.writerow(['Number', 'ID'])

      for i in range(len(node_list)):
        writer.writerow([i, node_list[i]])

  with open(output_dir + filename + '_edges.csv', 'w') as f:
      
      writer = csv.writer(f)

      writer.writerow(['Number', 'Node1', 'Node2'])

      for i in range(0, len(edges_to), 2):
        writer.writerow([i/2, edges_to[i], edges_to[i+1]])
  print(conn.gsql('''
        USE GRAPH current_graph
        CREATE LOADING JOB job1 FOR GRAPH current_graph {{
          LOAD "{nodes}" to VERTEX Node VALUES ($"ID") USING HEADER = "TRUE";
          LOAD "{edges}" to EDGE Undirected_Edge VALUES ($"Node1", $"Node2") USING HEADER = "TRUE";
        }}
        RUN LOADING JOB job1'''.format(nodes = output_dir + filename + '_nodes.csv', edges = output_dir + filename + '_edges.csv')))

def run_tigergraph(conn, clusterer, out_clustering, thread, config):

  feat = conn.gds.featurizer()
  
  
  threshold = -1
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
      # feat.installAlgorithm("tg_kcore")
      # conn.runInstalledQuery("tg_kcore", params=params, threadLimit = thread)
      params = {
        "v_type": "Node",
        "e_type": "Undirected_Edge",
        "result_attribute": "cluster",
        "k_max": threshold
      }
      res = feat.runAlgorithm("tg_kcore", params=params, threadLimit = thread)
      df = conn.getVertexDataFrame("Node")
      result = df.groupby('cluster')['id'].apply(list).tolist()
      print(df["cluster"].value_counts())

    if not (result is None):
      for cluster_list in result:
        runner_utils.appendToFile("\t".join(str(x) for x in cluster_list) + "\n", out_clustering)
    end_time = time.time()
    print("Time: " + str(end_time - start_time))
  out = f.getvalue()
  return out

def remove_tigergraph(conn):
  print(conn.gsql("DROP ALL"))



# if __name__ == "__main__":
#   main()