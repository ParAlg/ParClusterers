import networkit as nk
import runner_utils
import time
import io
from contextlib import redirect_stdout

# pip3 install --upgrade pip
# pip3 install cmake cython
# pip3 install networkit
# pip3 install tabulate

# Parallel Louvain
def runNetworKitPLM(G, config):
  use_refine = False
  use_gamma = 1.0
  use_par = "balanced"
  use_maxIter = 32
  use_turbo = True
  use_recurse = True
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("refine"):
        use_refine = True if config_split[1].startswith("True") else False
      elif config_split[0].startswith("gamma"):
        use_gamma = float(config_split[1])
      elif config_split[0].startswith("par"):
        use_par = config_split[1]
      elif config_split[0].startswith("maxIter"):
        use_maxIter = int(config_split[1])
      elif config_split[0].startswith("turbo"):
        use_turbo = True if config_split[1].startswith("True") else False
      elif config_split[0].startswith("recurse"):
        use_recurse = True if config_split[1].startswith("True") else False
  f = io.StringIO()
  with redirect_stdout(f):
    print(config)
    start_time = time.time()
    #Communities detected in 0.76547 [s]
    communities = nk.community.detectCommunities(G, algo=nk.community.PLM(G, refine=use_refine, gamma=use_gamma, par=use_par, maxIter=use_maxIter, turbo=use_turbo, recurse=use_recurse))
    end_time = time.time()
    print("Communities detected in %f \n" % (end_time - start_time))
  out = f.getvalue()
  # str(end_time - start_time)
  return out, communities

def runNetworKitPLP(G, config):
  use_updateThreshold = None
  use_maxIterations = None
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("updateThreshold"):
        use_updateThreshold = int(config_split[1])
      elif config_split[0].startswith("maxIterations"):
        use_maxIterations = int(config_split[1])
  f = io.StringIO()
  with redirect_stdout(f):
    print(config)
    start_time = time.time()
    communities = nk.community.detectCommunities(G, algo=nk.community.PLP(G, updateThreshold=use_updateThreshold, maxIterations=use_maxIterations, baseClustering=None))
    end_time = time.time()
    print("Communities detected in %f \n" % (end_time - start_time))
  out = f.getvalue()
  return out, communities

def runNetworKitParallelLeiden(G, config):
  use_randomize = True
  use_gamma = 1.0
  use_iterations = 3
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("randomize"):
        use_randomize = True if config_split[1].startswith("True") else False
      elif config_split[0].startswith("iterations"):
        use_iterations = int(config_split[1])
      elif config_split[0].startswith("gamma"):
        use_gamma = float(config_split[1])
  f = io.StringIO()
  with redirect_stdout(f):
    print(config)
    start_time = time.time()
    communities = nk.community.detectCommunities(G, algo=nk.community.ParallelLeiden(G, randomize=use_randomize, iterations=use_iterations, gamma=use_gamma))
    end_time = time.time()
    print("Communities detected in %f \n" % (end_time - start_time))
  out = f.getvalue()
  return out, communities

def runNetworKitConnectivity(G, config):
  f = io.StringIO()
  if (G.isDirected()):
    raise ValueError("NetworkIt Connected Components can only run for undirected graphs.")
  with redirect_stdout(f):
    print(config)
    start_time = time.time()
    # returns type List[List[int]], each nested list is a cluster, i.e. conencted component
    cc = nk.components.ParallelConnectedComponents(G, False)
    cc.run()
    clusters = cc.getComponents()
    end_time = time.time()
    print("Communities detected in %f \n" % (end_time - start_time))
  out = f.getvalue()
  return out, clusters


def runNetworKitKCore(G, config):
  # The graph may not contain self-loops.
  f = io.StringIO()
  with redirect_stdout(f):
    start_time = time.time()
    coreDec = nk.centrality.CoreDecomposition(G)
    coreDec.run()
    max_core_number = coreDec.maxCoreNumber()
    # cores = coreDec.
    cores = []
    # for i in range(max_core_number+1):
    #   cores.append(list(coreDec.getCover().getMembers(i)))
    end_time = time.time()
    print("Communities detected in %f \n" % (end_time - start_time))
  out = f.getvalue()
  return out, cores


def extractNetworKitTime(out):
  split = [x.strip() for x in out.split('\n')]
  for line in split:
    if line.startswith("Communities detected in"):
      line_split = [x.strip() for x in line.split(' ')]
      return line_split[3]
  return ""

def runNetworKit(clusterer, graph, thread, config, out_prefix):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("NetworKit can only be run using edge list format")
  out_filename = out_prefix + ".out"
  out_clustering = out_prefix + ".cluster"
  use_input_graph = runner_utils.input_directory + graph
  # if(not (use_input_graph.endswith("ungraph.txt") or use_input_graph.endswith("ngraph.txt"))):
  #   raise ValueError("input graph file name must ends with ungraph.txt or ngraph.txt")
  # G = nk.readGraph(use_input_graph, nk.Format.EdgeListTabZero)
  reader = nk.graphio.EdgeListReader('\t', 0, commentPrefix='#', directed=False) #continuous=False, 
  G = reader.read(use_input_graph)
  # print([edge for edge in G.iterEdgesWeights()])
  if (thread != "" and thread != "ALL"):
    nk.setNumberOfThreads(int(thread))
  # This is k-core with a thresholding argument (double-check)
  #nk.community.kCoreCommunityDetection(G, k, algo=None, inspect=False)
  cluster_flag = False
  if (clusterer == "NetworKitPLM"):
    print_time, communities = runNetworKitPLM(G, config)
  elif (clusterer == "NetworKitPLP"):
    print_time, communities = runNetworKitPLP(G, config)
  elif (clusterer == "NetworKitParallelLeiden"):
    print_time, communities = runNetworKitParallelLeiden(G, config)
  elif (clusterer == "NetworKitConnectivity"):
    cluster_flag = True
    print_time, clusters = runNetworKitConnectivity(G, config)
  elif (clusterer == "NetworKitKCore"):
    cluster_flag = True
    print_time, clusters = runNetworKitKCore(G, config)
  else:
    raise ValueError("NetworKit clusterer not supported")
  runner_utils.appendToFile(print_time, out_filename)
  runner_utils.appendToFile("Cluster Time: " + extractNetworKitTime(print_time) + "\n", out_filename)
  if (clusterer == "NetworKitKCore"): # does not produce clustering, can only run k-core decomposition
    return
  if not cluster_flag:
    communities.compact()
    cluster_index = 0
    cluster_list = communities.getMembers(cluster_index)
    while (cluster_list):
      runner_utils.appendToFile("\t".join(str(x) for x in cluster_list) + "\n", out_clustering)
      cluster_index += 1
      cluster_list = communities.getMembers(cluster_index)
  else:
    for cluster_list in clusters:
      runner_utils.appendToFile("\t".join(str(x) for x in cluster_list) + "\n", out_clustering)
