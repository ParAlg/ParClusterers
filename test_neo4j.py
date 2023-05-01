import time
import io
import pandas
import sys
from collections import defaultdict
from graphdatascience import GraphDataScience

def appendToFile(out, filename):
  with open(filename, "a+") as out_file:
    out_file.writelines(out)

def readGraph(filename, gds):
  nodes = set()
  edges_from = []
  edges_to = []
  with open(filename, "r") as in_file:
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

  #nodes_set, edges_from, edges_to = readGraph(graph_path, gds)
  #nodes_dict = {}
  #nodes_dict["nodeId"] = list(nodes_set)
  #relationships_dict = {}
  #relationships_dict["sourceNodeId"] = edges_from
  #relationships_dict["targetNodeId"] = edges_to
  #rel_type_list = ["KNOWS"] * len(edges_from)
  #rel_weight_list = [1.0] * len(edges_from)
  #relationships_dict["relationshipType"] = rel_type_list
  #relationships_dict["weight"] = rel_weight_list
  #nodes = pandas.DataFrame(nodes_dict)
  #relationships = pandas.DataFrame(relationships_dict)
  #G = gds.alpha.graph.construct( #G_dir
  #  graph_name,      # Graph name
  #  nodes,           # One or more dataframes containing node data
  #  relationships    # One or more dataframes containing relationship data
  #)

  c_commands = defaultdict(list)
  c_nodes = []
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
        nodes_set.add(int(a))
        nodes_set.add(int(b))
  #      cypher_commands_list.append("(A" + a + ": A)")
  #      cypher_commands_list.append("(A" + b + ": A)")
        cypher_commands_list.append("(A" + str(a) + ")-[:KNOWS]->(A" + str(b) + ")")
        #c_commands[int(a)].append({"start" : a, "end" : b})
  cypher_node_commands_list = []
  for node in nodes_set:
    cypher_node_commands_list.append("(A" + str(node) + ": A )") #{id: "+ str(node)+"}
    #c_nodes.append(node)

  print("Finished loading in memory")
  sys.stdout.flush()

  # Use Neo4j URI and credentials according to your setup
  gds = GraphDataScience("bolt://localhost:7687", auth=None)
  _ = gds.run_cypher("MATCH (n) DETACH DELETE n")
  #_ = gds.run_cypher("CALL apoc.schema.assert({},{},true) YIELD label, key RETURN *")

  print("Cleared db")
  sys.stdout.flush()

  #batch_size = 10000
  #current_index = 0
  #start_time = time.time()
  #while current_index < len(c_nodes):
  #  print("Index: " + str(current_index))
  #  sys.stdout.flush()
  #  end_index = current_index + batch_size
  #  if len(c_nodes) < end_index:
  #    end_index = len(c_nodes)
  #  c_commands_act = ["{start:" + str(a) +"}" for a in c_nodes[current_index : end_index]]
  #  cypher_command = "CALL apoc.periodic.iterate(\'UNWIND ["  + ", ".join(c_commands_act) + "] AS row RETURN row\', \'CREATE (head:A{id:row.start})\',{batchSize:10000, retries: 3, iterateList:true, parallel:true})"
  #  gds.run_cypher(cypher_command)
  #  current_index = current_index + batch_size
  #current_index = 0
  #end_time = time.time()
  #print("Node Reading Time: " + str(end_time - start_time))

  #cypher_command = "CREATE CONSTRAINT node_id ON (n:A) ASSERT n.id IS UNIQUE"
  #gds.run_cypher(cypher_command)
  #print("Constraint")
  #sys.stdout.flush()

  cypher_command = "CREATE " + ', '.join(cypher_node_commands_list)
  start_time = time.time()
  gds.run_cypher(cypher_command)
  end_time = time.time()
  print("Node Reading Time: " + str(end_time - start_time))
  sys.stdout.flush()

  
  
  print("Finished loading nodes")
  sys.stdout.flush()

  #start_time = time.time()
  #for c in c_commands:
  #  c_commands_tmp = c_commands[c]
  #  c_commands_act = ["{start:" + a["start"] + ", end:" + a["end"] + "}" for a in c_commands_tmp]
  #  cypher_command = "CALL apoc.periodic.iterate(\'UNWIND ["  + ", ".join(c_commands_act) + "] AS row RETURN row\', \'MATCH (head:A{id:row.start}) USING INDEX head:A(id) MATCH (tail:A{id:row.end}) USING INDEX tail:A(id) CREATE (head)-[:KNOWS]->(tail)\', {batchSize:10000, retries: 3, iterateList:true, parallel:true})"
  #  gds.run_cypher(cypher_command)
  #end_time = time.time()
  #print("Edge Reading Time: " + str(end_time - start_time))

  #while current_index < len(c_commands):
  #  print("C Index: " + str(current_index))
  #  sys.stdout.flush()
  #  end_index = current_index + batch_size
  #  if len(c_commands) < end_index:
  #    end_index = len(c_commands)
  #  c_commands_act = ["{start:" + a["start"] + ", end:" + a["end"] + "}" for a in c_commands[current_index : end_index]]
  #  cypher_command = "CALL apoc.periodic.iterate(\'UNWIND ["  + ", ".join(c_commands_act) + "] AS row RETURN row\', \'MATCH (head:A{id:row.start}) MATCH (tail:A{id:row.end}) CREATE (head)-[:KNOWS]->(tail)\', {batchSize:10000, retries: 3, iterateList:true, parallel:true})"
  #  gds.run_cypher(cypher_command)
  #  current_index = current_index + batch_size

  # Working unwind merge method
  #batch_size = 10000
  #current_index = 0
  #while current_index < len(cypher_commands_list):
  #  print("Index: " + str(current_index))
  #  sys.stdout.flush()
  #  end_index = current_index + batch_size
  #  if len(cypher_commands_list) < end_index:
  #    end_index = len(cypher_commands_list)
  #  c_commands_act = ["{start:" + a["start"] + ", end:" + a["end"] + "}" for a in c_commands[current_index : end_index]]
  #  cypher_command = "UNWIND ["  + ", ".join(c_commands_act) + "] AS row MERGE (head:A{id:row.start}) MERGE (tail:A{id:row.end}) CREATE (head)-[:KNOWS]->(tail)"
  #  gds.run_cypher(cypher_command)
  #  current_index = current_index + batch_size

  cypher_command = "CREATE " + ', '.join(cypher_commands_list)
  start_time = time.time()
  gds.run_cypher(cypher_command)
  end_time = time.time()
  print("Edge Reading Time: " + str(end_time - start_time))

  print("Finished cypher")
  sys.stdout.flush()

  #gds.run_cypher("MATCH (n) SET n:A")

  gds.run_cypher("CALL gds.graph.project(\'" + graph_name + "undir\', \'*\', {KNOWS: {orientation: \'UNDIRECTED\'}})")
  G = gds.graph.get(graph_name + "undir")

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
  graphs = ["com-dblp.ungraph.txt", "com-youtube.ungraph.txt"] #"com-amazon.ungraph.txt" , "com-lj.ungraph.txt", "com-orkut.ungraph.txt"] #
  graph_pres = ["dblp", "youtube"] #, ,"amazon" "lj", "orkut"] #"youtube", 
  for graph_idx, graph in enumerate(graphs):
    graph_pre = graph_pres[graph_idx]
    graph_name = directory + "snap/" + graph
    algs = ["leiden"] #["triangle", "louvain", "modularity", "pagerank"]
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

