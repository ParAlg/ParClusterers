import time
import io
import pandas
import sys
from collections import defaultdict
from neo4j import GraphDatabase
from graphdatascience import GraphDataScience

def appendToFile(out, filename):
  with open(filename, "a+") as out_file:
    out_file.writelines(out)

def readGraph(filename):
  print_index = 1000000
  index_track = 0
  nodes = set()
  edges_from = []
  edges_to = []
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
        nodes.add(int(a))
        nodes.add(int(b))
        edges_from.append(int(a))
        edges_to.append(int(b))
        edges_from.append(int(b))
        edges_to.append(int(a))
  return nodes, edges_from, edges_to



#  nodes_set = set()
#  edges_from = []
#  edges_to = []
#  with open(graph_path, "r") as in_file:
#    for line in in_file:
#      line = line.strip()
#      if not line:
#        continue
#      if line[0] == '#':
#        continue
#      split = [x.strip() for x in line.split('\t')]
#      if split:
#        a = split[0]
#        b = split[1]
#        gds.run_cypher("CREATE (A" + str(a) + ": A)")
#        gds.run_cypher("CREATE (A" + str(b) + ": A)")
#        gds.run_cypher("CREATE (A" + str(a) + ": A)-[:KNOWS]->(A" + str(b) + ":A)")

# first argument is input graph
# second argument is louvain, modularity, or leiden; triangle
# third argument is output clustering
# default weight is unweighted

def run_algs(graph_path, algs, out_dir, graph_pre):
  graph_name = "my-graph"

  nodes_set, edges_from, edges_to = readGraph(graph_path)
  nodes_dict = {}
  nodes_dict["nodeId"] = list(nodes_set)
  relationships_dict = {}
  relationships_dict["sourceNodeId"] = edges_from
  relationships_dict["targetNodeId"] = edges_to
  rel_type_list = ["KNOWS"] * len(edges_from)
  #rel_weight_list = [1.0] * len(edges_from)
  relationships_dict["relationshipType"] = rel_type_list
  #relationships_dict["weight"] = rel_weight_list
  nodes = pandas.DataFrame(nodes_dict)
  relationships = pandas.DataFrame(relationships_dict)

  print("Finish loading in memory")
  sys.stdout.flush()

  # Use Neo4j URI and credentials according to your setup
  neo4j_url = "bolt://localhost:7687"
  neo4j_client = GraphDatabase.driver(neo4j_url, auth=None, max_connection_lifetime=7200)
  gds = GraphDataScience(neo4j_client, auth=None) #"bolt://localhost:7687"

  print("Starting gds")
  sys.stdout.flush()

  _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  #_ = gds.run_cypher("CALL apoc.schema.assert({},{},true) YIELD label, key RETURN *")

  print("Cleared db")
  sys.stdout.flush()

  start_time = time.time()
  G = gds.alpha.graph.construct( #G_dir
    graph_name,      # Graph name
    nodes,           # One or more dataframes containing node data
    relationships    # One or more dataframes containing relationship data
  )
  end_time = time.time()
  print("Reading Time: " + str(end_time - start_time))


  print("Finished cypher")
  sys.stdout.flush()

  #gds.run_cypher("MATCH (n) SET n:A")

  #gds.run_cypher("CALL gds.graph.project(\'" + graph_name + "undir\', \'*\', {KNOWS: {orientation: \'UNDIRECTED\'}})")
  #G = gds.graph.get(graph_name + "undir")

  #G, result = gds.graph.project(graph_name + "undir", '*', {"KNOWS" : {"orientation": "UNDIRECTED"}})

  print("Finished loading graph")
  print("Relationship count: " + str(G.relationship_count()))

  for algorithm_name in algs:
    community_flag = False
    print(graph_pre + " " + algorithm_name)
    sys.stdout.flush()
    start_time = time.time()
    #pandas_res = gds.louvain.stream(G) #relationshipWeightProperty="weight", 
    if (algorithm_name.startswith("modularity")):
      community_flag = True
      res = gds.beta.modularityOptimization.mutate(G, mutateProperty="modularitycommunity") #relationshipWeightProperty="weight",
    elif (algorithm_name.startswith("louvain")):
      community_flag = True
      #use_cypher = "CALL gds.louvain.mutate(\'" + graph_name + "undir\', {mutateProperty: \'louvaincommunity\'}) YIELD preProcessingMillis, computeMillis, postProcessingMillis, communityCount, modularity"
      #res = gds.run_cypher(use_cypher)
      res = gds.louvain.mutate(G, mutateProperty="louvaincommunity") #relationshipWeightProperty="weight", 
    elif (algorithm_name.startswith("leiden")):
      community_flag = True
      res = gds.alpha.leiden.mutate(G, mutateProperty="leidencommunity")
    elif algorithm_name.startswith("triangle"):
      #use_cypher = "CALL gds.triangleCount.mutate(\'" + graph_name + "undir\', {mutateProperty: \'triangle\'}) YIELD preProcessingMillis, computeMillis, postProcessingMillis, nodeCount, globalTriangleCount"
      #res = gds.run_cypher(use_cypher)
      res = gds.triangleCount.mutate(G, mutateProperty="triangle")
    elif algorithm_name.startswith("pagerank"):
      res = gds.pageRank.mutate(G, mutateProperty="pagerank")
    end_time = time.time()
    print("Time: " + str(end_time - start_time))
    print("Preprocessing millis: " + str(res["preProcessingMillis"]))
    print("Compute millis: " + str(res["computeMillis"]))
    print("Postprocessing millis: " + str(res["postProcessingMillis"]))
    sys.stdout.flush()

    if algorithm_name.startswith("triangle"):
      print("Triangle count: " + str(res["globalTriangleCount"]))
      print("Node count: " + str(res["nodeCount"]))
      sys.stdout.flush()
    if (community_flag):
      print("Community count: " + str(res["communityCount"]))
      print("Modularity: " + str(res["modularity"]))
      sys.stdout.flush()
#      cluster_dict = defaultdict(list)
#      for node in nodes_set:
#        community_id = gds.util.nodeProperty(G, node, algorithm_name + "community")
#        cluster_dict[community_id].append(node)
      #for index, row in pandas_res.iterrows():
      #  cluster_dict[row["communityId"]].append(row["nodeId"])
#      with open(out_dir + algorithm_name + ".cluster", "a+") as out_file:
#        for key, value in cluster_dict.items():
#          out_file.write('\t'.join(map(str, value)) + '\n')

  #gds.graph.drop(G)
  #G_dir.drop()
  G.drop()
  _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  #G_undir = gds.graph.get(graph_name + "undir")
  #G_undir.drop()

  #res = gds.pageRank.mutate(G, tolerance=0.5, mutateProperty="pagerank")
  #print(str(res["nodePropertiesWritten"]))
  #assert res["nodePropertiesWritten"] == G.node_count()

def main():
  args = sys.argv[1:]
  directory = "/home/ubuntu/"
  graphs = ["com-dblp.ungraph.txt"] #"com-lj.ungraph.txt","com-youtube.ungraph.txt", "com-amazon.ungraph.txt",
  graph_pres = ["dblp"] # "dblp","youtube", "amazon",
  for graph_idx, graph in enumerate(graphs):
    graph_pre = graph_pres[graph_idx]
    graph_name = directory + "snap/" + graph
    algs = [] #"triangle", "leiden", "pagerank"]
    out_dir = directory + "neo4j_out/" + graph_pre + "_"
    run_algs(graph_name, algs, out_dir, graph_pre)
  #for graph_idx, graph in enumerate(graphs):
  #  graph_pre = graph_pres[graph_idx]
  #  graph_name = directory + "snap/" + graph
  #  algs = ["leiden"]
  #  out_dir = directory + "neo4j_out/" + graph_pre + "_"
  #  run_algs(graph_name, algs, out_dir, graph_pre)

if __name__ == "__main__":
  main()

#nodes = pandas.DataFrame(
#    {
#        "nodeId": [0, 1, 2, 3],
#        "labels":  ["A", "B", "C", "A"],
#        "prop1": [42, 1337, 8, 0],
#        "otherProperty": [0.1, 0.2, 0.3, 0.4]
#    }
#)

#relationships = pandas.DataFrame(
#    {
#        "sourceNodeId": [0, 1, 2, 3],
#        "targetNodeId": [1, 2, 3, 0],
#        "relationshipType": ["REL", "REL", "REL", "REL"],
#        "weight": [0.0, 0.0, 0.1, 42.0]
#    }
#)

