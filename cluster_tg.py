import pyTigerGraph as tg
import json
import sys
import csv
import time
import io
from contextlib import redirect_stdout
import runner_utils
import load_tg

SLPA_QUERY_FILE_PATH='../tigergraph-3.9.2-offline/query/tg_slpa.gsql'

def load_tigergraph(conn, filename, input_dir, output_dir, tigergraph_nodes, tigergraph_edges, weighted):

  conn.graphname = 'current_graph'
  nodes = output_dir + filename + '_nodes.csv'
  edges = output_dir + filename + '_edges.csv'

  if tigergraph_edges == None or tigergraph_nodes == None:
    load_tg.convert_to_tigergraph_format(filename, input_dir, output_dir)
  else:
    nodes = input_dir + tigergraph_nodes
    edges = input_dir + tigergraph_edges
  if not weighted:
    print(conn.gsql('''
    CREATE VERTEX Node (id INT PRIMARY KEY)
    CREATE UNDIRECTED EDGE Undirected_Edge (FROM Node, TO Node)
    CREATE GRAPH current_graph (Node, Undirected_Edge)
    USE GRAPH current_graph
    CREATE LOADING JOB job1 FOR GRAPH current_graph {{
      LOAD "{nodes}" to VERTEX Node VALUES ($"ID") USING HEADER = "TRUE";
      LOAD "{edges}" to EDGE Undirected_Edge VALUES ($"Node1", $"Node2") USING HEADER = "TRUE";
    }}
    RUN LOADING JOB job1'''.format(nodes = nodes, edges = edges)))
  else: 
    print(conn.gsql('''
    CREATE VERTEX Node (id INT PRIMARY KEY)
    CREATE UNDIRECTED EDGE Undirected_Weighted_Edge (FROM Node, TO Node, weight FLOAT)
    CREATE GRAPH current_graph (Node, Undirected_Weighted_Edge)
    USE GRAPH current_graph
    CREATE LOADING JOB job1 FOR GRAPH current_graph {{
      LOAD "{nodes}" to VERTEX Node VALUES ($"ID") USING HEADER = "TRUE";
      LOAD "{edges}" to EDGE Undirected_Weighted_Edge VALUES ($"Node1", $"Node2", $"Weight") USING HEADER = "TRUE";
    }}
    RUN LOADING JOB job1'''.format(nodes = nodes, edges = edges)))
  

def run_tigergraph(conn, clusterer, out_clustering, thread, config, weighted):

  feat = conn.gds.featurizer()
  
  
  threshold = -1
  maximum_iteration = 10
  edge = 'Undirected_Weighted_Edge' if weighted else 'Undirected_Edge'
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("threshold"):
        if config_split[1] != "None":
          threshold = float(config_split[1])
      if config_split[0].startswith("maximum_iteration"):
        maximum_iteration = float(config_split[1])

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
        "result_attribute": "cluster",
        "maximum_iteration": maximum_iteration
      }
      if weighted:
        params['weight_attribute'] = 'weight'
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
        "result_attribute": "cluster",
        "maximum_iteration": maximum_iteration,
        "print_limit": -1
      }
      res = feat.runAlgorithm("tg_label_prop", params=params, threadLimit = thread)
    elif clusterer == 'TigerGraphSLLabelProp':
      params = {
        "v_type_set": ["Node"],
        "e_type_set": [edge],
        "maximum_iteration": maximum_iteration,
        "result_attribute": "cluster",
        "print_limit": -1,
        "threshold": threshold
      }
      print(feat.installAlgorithm('tg_slpa', query_path=SLPA_QUERY_FILE_PATH))
      res = feat.runAlgorithm("tg_slpa", params=params, threadLimit = thread, custom_query=True, feat_type = 'INT', schema_name = ["Node"], feat_name= 'cluster', timeout = 10000000)

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
  load_tigergraph(conn, 'com-lj.ungraph.txt', '/home/sy/mount-data/', '/home/sy/ParClusterers/results/', None, None, False)
  remove_tigergraph(conn)