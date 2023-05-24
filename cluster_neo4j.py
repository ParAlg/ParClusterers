import time
import io
import pandas
import sys
from collections import defaultdict
from graphdatascience import GraphDataScience
import math
from neo4j import GraphDatabase

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

def appendToFile(out, filename):
  with open(filename, "a+") as out_file:
    out_file.writelines(out)

def getLoadGraphCommand(graph_path):
  nodes_set = set()
  cypher_commands_list = []
  with open(graph_path, "r") as in_file:
    for line in in_file:
      line = line.strip()
      if not line:
        continue
      if line[0] == '#':
        continue
      split = [x.strip() for x in line.split('\t')]
      if split:
        a = split[0]
        b = split[1]
        w = 0
        if len(split) == 3:
          w = split[2]
        nodes_set.add(int(a))
        nodes_set.add(int(b))
        cypher_commands_list.append("(A" + str(a) + ")-[:EDGE { weight:" + str(w) + " }]->(A" + str(b) + ")")
  cypher_node_commands_list = []
  for node in nodes_set:
    cypher_node_commands_list.append("(A" + str(node) + ": A {id: "+ str(node)+" })") #
  return cypher_commands_list, cypher_node_commands_list

# first argument is input graph
# second argument is louvain, modularity, or leiden; triangle
# third argument is output clustering
# default weight is unweighted

def runNeo4j(graph_path, graph_name, algorithm_name, thread, config, out_clustering):
  ## load configs
  threshold = None
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("threshold"):
        if config_split[1] != "None":
          threshold = float(config_split[1])

  # Use Neo4j URI and credentials according to your setup
  gds = GraphDataScience("bolt://localhost:7687", auth=None)
  print("GDS version: ", gds.version())

  # graph_name = graph_pre #+ "undir"
  # graph_exists = gds.graph.exists(graph_name=graph_name)
  # if graph_exists[1]: 
  #   gds.graph.drop(gds.graph.get(graph_name))
  graph_exists = gds.graph.exists(graph_name=graph_name)
  if not graph_exists[1]:
    print("error, graph does not exist")
    return "error, graph does not exist"

  G = gds.graph.get(graph_name)
  print("database: ", G.database())
  # print(G.node_count())

  print("Finished loading graph")
  print("Relationship count: " + str(G.relationship_count()))

  community_flag = False
  component_flag = False
  mutateProperty = ""
  print("Graph: ", graph_name,  ", Alg.: ", algorithm_name)
  sys.stdout.flush()
  start_time = time.time()
  if (algorithm_name.startswith("louvain")):
    community_flag = True
    res = gds.louvain.mutate(G, mutateProperty="louvaincommunity") #relationshipWeightProperty="weight", 
  elif (algorithm_name.startswith("leiden")):
    community_flag = True
    res = gds.beta.leiden.mutate(G, mutateProperty="leidencommunity")
  elif algorithm_name.startswith("connectivity"):
    component_flag = True #
    mutateProperty = "connectivitycommunity"
    res = gds.wcc.mutate(G, mutateProperty=mutateProperty, threshold = threshold, relationshipWeightProperty="weight", concurrency = thread)
  else:
    print("The algorithm ", algorithm_name, " is not available")
  end_time = time.time()
  # print(res)
  # node1 = gds.find_node_id(["A"], {"id": 0})
  # node2 = gds.find_node_id(["A"], {"id": 1})
  # print(node1, node2)
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
  if (component_flag):
    # pass
    print("Community count: " + str(res["componentCount"]))
    # print(G.node_properties())
    start_time = time.time()
    result = gds.graph.nodeProperty.stream(G, node_properties=mutateProperty)
    result.to_csv("tmp.csv")
    end_time = time.time()
    print("Gather result Time: " + str(end_time - start_time))
    # print(result)


  sys.stdout.flush()
  # cluster_dict = defaultdict(list)
  # for node in nodes_set:
  #   community_id = gds.util.nodeProperty(G, node, algorithm_name + "community")
  #   cluster_dict[community_id].append(node)
  # for index, row in pandas_res.iterrows():
  #   cluster_dict[row["communityId"]].append(row["nodeId"])
  # with open(out_clustering, "a+") as out_file:
  #   for key, value in cluster_dict.items():
  #     out_file.write('\t'.join(map(str, value)) + '\n')

  #gds.graph.drop(G)
  #G_dir.drop()
  # G.drop()
  # _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  #G_undir = gds.graph.get(graph_name + "undir")
  #G_undir.drop()
  # gds.run_cypher("MATCH (n) REMOVE n." + "connectivitycommunity")
  gds.close()


def clearDB(graph_name):
  gds = GraphDataScience("bolt://localhost:7687", auth=None)
  _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  graph_exists = gds.graph.exists(graph_name=graph_name)
  if graph_exists[1]: 
    gds.graph.drop(gds.graph.get(graph_name))
  gds.close()

def projectGraph(graph_name, graph_path):
    # Use Neo4j URI and credentials according to your setup
  neo4j_url = "bolt://localhost:7687"
  neo4j_client = GraphDatabase.driver(neo4j_url, auth=None, max_connection_lifetime=7200)
  gds = GraphDataScience(neo4j_client, auth=None) #"bolt://localhost:7687"
  graph_exists = gds.graph.exists(graph_name=graph_name)
  if not graph_exists[1]:
    # cypher_commands_list, cypher_node_commands_list = getLoadGraphCommand(graph_path)
    # print("Finished loading in memory")
    # sys.stdout.flush()
    # # _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
    # cypher_command = "CREATE " + ', '.join(cypher_node_commands_list) +", "+ ', '.join(cypher_commands_list)
    # # print(cypher_command)
    # start_time = time.time()
    # gds.run_cypher(cypher_command)
    # end_time = time.time()
    # print("Node and Edge Reading Time: " + str(end_time - start_time))
    # # sys.stdout.flush()

    # print("Finished cypher")
    # sys.stdout.flush()

    # gds.run_cypher("CALL gds.graph.project(\'" + graph_name + "\', \'*\', {EDGE: {orientation: \'UNDIRECTED\', properties: ['weight']}})")
    nodes_set, edges_from, edges_to, weights = readGraph(graph_path)
    nodes_dict = {}
    nodes_dict["nodeId"] = list(nodes_set)
    relationships_dict = {}
    relationships_dict["sourceNodeId"] = edges_from
    relationships_dict["targetNodeId"] = edges_to
    rel_type_list = ["EDGE"] * len(edges_from)
    relationships_dict["relationshipType"] = rel_type_list
    if len(weights) > 0:
      relationships_dict["weight"] = weights
    nodes = pandas.DataFrame(nodes_dict)
    relationships = pandas.DataFrame(relationships_dict)

    print("Finish loading in memory")
    sys.stdout.flush()

    print("Starting gds")
    sys.stdout.flush()

    # _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
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
    print("Node Count: ", G.node_count())

    print("Finished cypher")
    sys.stdout.flush()
    gds.close()
    return True
  gds.close()
  return False


def main():
  args = sys.argv[1:]
  directory = "/home/ubuntu/"
  graphs = ["com-dblp.ungraph.txt"]#, "edge.txt" #"com-dblp.ungraph.txt","com-youtube.ungraph.txt", "com-amazon.ungraph.txt",
  # graph_pres = ["edge"] # "dblp","youtube", "amazon",
  config = "threashold: None"
  for graph_idx, graph in enumerate(graphs):
    # graph_pre = graph_pres[graph_idx]
    graph_name = directory + "snap/" + graph
    alg = "connectivity" 
    # out_dir = directory + "neo4j_out/" + graph_pre + "_"
    if args[0] == "run":
      runNeo4j(graph_name, graph, alg, 4, config, "")
    elif args[0] == "load":
      projectGraph(graph, graph_name)
    elif args[0] == "delete":
      clearDB(graph)
  #for graph_idx, graph in enumerate(graphs):
  #  graph_pre = graph_pres[graph_idx]
  #  graph_name = directory + "snap/" + graph
  #  algs = ["leiden"]
  #  out_dir = directory + "neo4j_out/" + graph_pre + "_"
  #  run_algs(graph_name, algs, out_dir, graph_pre)

if __name__ == "__main__":
  main()