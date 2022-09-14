import networkit as nk
import runner_utils

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
  start_time = time.time()
  communities = nk.community.detectCommunities(G, algo=nk.community.PLM(G, refine=use_refine, gamma=use_gamma, par=use_par, maxIter=use_maxIter, turbo=use_turbo, recurse=use_recurse))
  end_time = time.time()
  return str(end_time - start_time), communities

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
  start_time = time.time()
  communities = nk.community.detectCommunities(G, algo=nk.community.PLP(G, updateThreshold=use_updateThreshold, maxIterations=use_maxIterations, baseClustering=None))
  end_time = time.time()
  return str(end_time - start_time), communities

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
  start_time = time.time()
  communities = nk.community.detectCommunities(G, algo=nk.community.ParallelLeiden(G, randomize=use_randomize, iterations=use_iterations, gamma=use_gamma))
  end_time = time.time()
  return str(end_time - start_time), communities

def runNetworKit(clusterer, graph, thread, config, out_prefix):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("NetworKit can only be run using edge list format")
  out_filename = out_prefix + ".out"
  out_clustering = out_prefix + ".cluster"
  use_input_graph = runner_utils.input_directory + graph
  G = nk.readGraph(use_input_graph, nk.Format.SNAP)
  if (thread != "" and thread != "ALL"):
    nk.setNumberOfThreads(int(thread))
  # This is k-core with a thresholding argument (double-check)
  #nk.community.kCoreCommunityDetection(G, k, algo=None, inspect=False)
  if (clusterer == "NetworKitPLM"):
    print_time, communities = runNetworKitPLM(G, config)
  elif (clusterer == "NetworKitPLP"):
    print_time, communities = runNetworKitPLP(G, config)
  elif (clusterer == "NetworKitParallelLeiden"):
    print_time, communities = runNetworKitParallelLeiden(G, config)
  else:
    raise ValueError("NetworKit clusterer not supported")
  runner_utils.appendToFile("Cluster Time: " + print_time, out_filename)
  communities.compact()
  cluster_index = 0
  cluster_list = communities.getMembers(cluster_index)
  while (cluster_list):
    runner_utils.appendToFile("\t".join(str(x) for x in cluster_list) + "\n", out_clustering)
    cluster_index += 1
    cluster_list = communities.getMembers(cluster_index)