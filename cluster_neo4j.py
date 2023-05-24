import time
import io
import pandas
import sys
from collections import defaultdict
from graphdatascience import GraphDataScience
import math


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

def runNeo4j(graph_path, graph_pre, algorithm_name, thread, config, out_clustering):
  ## load configs
  threshold = math.inf
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("threshold"):
        threshold = float(config_split[1])

  # Use Neo4j URI and credentials according to your setup
  gds = GraphDataScience("bolt://localhost:7687", auth=None)
  print("GDS version: ", gds.version())

  graph_name = graph_pre #+ "undir"
  # graph_exists = gds.graph.exists(graph_name=graph_name)
  # if graph_exists[1]: 
  #   gds.graph.drop(gds.graph.get(graph_name))
  graph_exists = gds.graph.exists(graph_name=graph_name)
  if not graph_exists[1]:
    cypher_commands_list, cypher_node_commands_list = getLoadGraphCommand(graph_path)

    print("Finished loading in memory")
    sys.stdout.flush()

    # print("Num. of threads: ", gds.nthreads())

    _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
    #_ = gds.run_cypher("CALL apoc.schema.assert({},{},true) YIELD label, key RETURN *")xw
    print("Cleared db")
    sys.stdout.flush()

    cypher_command = "CREATE " + ', '.join(cypher_node_commands_list) +", "+ ', '.join(cypher_commands_list)
    # print(cypher_command)

    start_time = time.time()
    gds.run_cypher(cypher_command)
    end_time = time.time()
    print("Node and Edge Reading Time: " + str(end_time - start_time))
    sys.stdout.flush()

    print("Finished cypher")
    sys.stdout.flush()


    gds.run_cypher("CALL gds.graph.project(\'" + graph_name + "\', \'*\', {EDGE: {orientation: \'UNDIRECTED\', properties: ['weight']}})")

  G = gds.graph.get(graph_name)
  print("database: ", G.database())
  # print(G.node_count())

  #G, result = gds.graph.project(graph_name + "undir", '*', {"EDGE" : {"orientation": "UNDIRECTED"}})

  print("Finished loading graph")
  print("Relationship count: " + str(G.relationship_count()))

  community_flag = False
  component_flag = False
  print("Graph: ", graph_pre,  ", Alg.: ", algorithm_name)
  sys.stdout.flush()
  start_time = time.time()
  if (algorithm_name.startswith("louvain")):
    community_flag = True
    #use_cypher = "CALL gds.louvain.mutate(\'" + graph_name + "undir\', {mutateProperty: \'louvaincommunity\'}) YIELD preProcessingMillis, computeMillis, postProcessingMillis, communityCount, modularity"
    #res = gds.run_cypher(use_cypher)
    res = gds.louvain.mutate(G, mutateProperty="louvaincommunity") #relationshipWeightProperty="weight", 
  elif (algorithm_name.startswith("leiden")):
    community_flag = True
    res = gds.beta.leiden.mutate(G, mutateProperty="leidencommunity")
  elif algorithm_name.startswith("connectivity"):
    component_flag = True
    res = gds.wcc.mutate(G, mutateProperty="connectivitycommunity", threshold = threshold, relationshipWeightProperty="weight", concurrency = thread)
  else:
    print("The algorithm ", algorithm_name, " is not available")
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
  if (component_flag):
    print("Community count: " + str(res["componentCount"]))
    # print(G.node_properties())
    # result = gds.graph.nodeProperty.stream(G, node_properties="connectivitycommunity")
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
  G.drop()
  # _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  #G_undir = gds.graph.get(graph_name + "undir")
  #G_undir.drop()