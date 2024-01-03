import os
import sys
import signal
import time
import subprocess
import re
import itertools
import cluster_nk
import cluster_tg
import runner_utils
import cluster_neo4j
import traceback
import postprocess_clustering
from pyTigerGraph import TigerGraphConnection
import pandas as pd

# Graph must be in edge format
def runSnap(clusterer, graph, graph_idx, round, runtime_dict):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("SNAP can only be run using edge list format")
  use_input_graph = runner_utils.input_directory + graph
  out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + str(round)
  out_clustering = out_prefix + ".cluster"
  out_filename = out_prefix + ".out"
  snap_binary = "community"
  args = ""
  output_postfix = ""
  print("Compiling snap binaries. This might take a while if it's the first time.")
  if (clusterer == "SnapGirvanNewman"):
    runner_utils.shellGetOutput("(cd external/snap/examples/%s && make all)" % snap_binary)
    alg_number = 1
    args = " -a:" + str(alg_number)
  elif (clusterer == "SnapInfomap"):
    runner_utils.shellGetOutput("(cd external/snap/examples/%s && make all)" % snap_binary)
    alg_number = 3
    args = " -a:" + str(alg_number)
  elif (clusterer == "SnapCNM"):
    runner_utils.shellGetOutput("(cd external/snap/examples/%s && make all)" % snap_binary)
    alg_number = 2
    args = " -a:" + str(alg_number)
  elif (clusterer == "SnapConnectivity"):
    snap_binary = "concomp"
    args = " -wcconly:T"
    output_postfix = ".wcc.txt"
    runner_utils.shellGetOutput("(cd external/snap/examples/%s && make all)" % snap_binary)
  elif (clusterer == "SnapKCore"):
    snap_binary = "kcores"
    args = " -s:F"  # Save the k-core network (for every k) (default:'T')
    runner_utils.shellGetOutput("(cd external/snap/examples/%s && make all)" % snap_binary)
  else:
    raise("Clusterer is not implemented.")
  print("Compilation done.")
  cmds = runner_utils.timeout + " external/snap/examples/%s/%s -i:"  % (snap_binary, snap_binary) + use_input_graph + " -o:" + out_clustering + args
  # print(cmds)
  if runner_utils.postprocess_only != "true":
    runner_utils.appendToFile('Snap: \n', out_filename)
    runner_utils.appendToFile("Input graph: " + graph + "\n", out_filename)
    out_time = runner_utils.shellGetOutput(cmds)
    runner_utils.appendToFile(out_time, out_filename)
    # postprocess to match our clustering format
    if (clusterer == "SnapConnectivity"):
      os.rename(out_clustering + output_postfix, out_clustering)
      postprocess_clustering.snap_connectivity(out_clustering)
  print("postprocessing..." + out_filename)
  with open(out_filename,'r') as f:
    run_info = f.readlines()
    for elem in run_info[1:]:
      if elem.startswith('Wealy Connected Component Time:') or elem.startswith('KCore Time:') or elem.startswith('Cluster Time:'):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()



def runNeo4j(clusterer, graph, thread, config, weighted, out_prefix, runtime_dict):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("Neo4j can only be run using edge list format")
  use_input_graph = runner_utils.input_directory + graph
  out_clustering = out_prefix + ".cluster"
  out_filename = out_prefix + ".out"
  alg_name = clusterer[5:]
  thread = int(thread)
  if runner_utils.postprocess_only != "true":
    out_time = cluster_neo4j.runNeo4j(use_input_graph, graph, alg_name, thread, config, weighted, out_clustering)
    runner_utils.appendToFile(out_time, out_filename)
  print("postprocessing..." + out_filename)
  with open(out_filename,'r') as f:
    run_info = f.readlines()
    for elem in run_info[1:]:
      if elem.startswith("Time:"):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()

# Graph must be in edge format
def runTectonic(clusterer, graph, thread, config, out_prefix, runtime_dict):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("Tectonic can only be run using edge list format")
  runner_utils.readSystemConfig(sys.argv[2])
  use_input_graph = runner_utils.input_directory + graph
  out_clustering_tmp = out_prefix + ".tmpcluster"
  out_clustering = out_prefix + ".cluster"
  out_filename = out_prefix + ".out"
  runner_utils.shellGetOutput("(cd external/Tectonic/mace && make)")
  runner_utils.shellGetOutput("(cd external/Tectonic && make all)")
  threshold = "0.06"
  # no_pruning = True
  split = [x.strip() for x in config.split(',')]
  for config_item in split:
    config_split = [x.strip() for x in config_item.split(':')]
    if config_split:
      if config_split[0].startswith("threshold"):
        if config_split[1]!="":
          threshold = config_split[1]
      # elif config_split[0].startswith("no_pruning"):
      #   no_pruning = True if config_split[1].startswith("True") else False

  if runner_utils.postprocess_only == "true":
    print("postprocessing..." + out_filename)
    with open(out_filename,'r') as f:
      run_info = f.readlines()
      for elem in run_info[1:]:
        if elem.startswith('Cluster Time:'):
          runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
  else:
    # Timing from here
    start_time = time.time()
    # relabel the graph so the node vertices are consecutive. The result format: each line i is the neighbors of i, and each edge only appear once in the smaller id's line.
    num_vert = runner_utils.shellGetOutput(runner_utils.python_ver + " external/Tectonic/relabel-graph-no-comm.py " + use_input_graph + " " + out_prefix + ".mace" + " " + out_prefix + ".pickle")
    num_vert = num_vert.strip()
    runner_utils.shellGetOutput("external/Tectonic/mace/mace C -l 3 -u 3 "+ out_prefix + ".mace " + out_prefix + ".triangles")
    runner_utils.shellGetOutput(runner_utils.python_ver + " external/Tectonic/mace-to-list.py " + out_prefix + ".mace " + out_prefix + ".edges")
    # if (no_pruning):
    runner_utils.shellGetOutput(runner_utils.python_ver + " external/Tectonic/weighted-edges-no-mixed.py " + out_prefix + ".triangles " + out_prefix + ".edges " + out_prefix + ".weighted " + out_prefix + ".mixed " + num_vert)
    cluster = runner_utils.shellGetOutput("external/Tectonic/tree-clusters-parameter-no-mixed " + out_prefix + ".weighted " + num_vert + " " + threshold)
    # else:
    #   runner_utils.shellGetOutput(runner_utils.python_ver + " external/Tectonic/weighted-edges.py " + out_prefix + ".triangles " + out_prefix + ".edges " + out_prefix + ".weighted " + out_prefix + ".mixed " + num_vert)
    #   cluster = runner_utils.shellGetOutput("external/Tectonic/tree-clusters-parameter " + out_prefix + ".weighted " + num_vert + " " + threshold)
    end_time = time.time()
    # Output running time to out_filename
    runner_utils.appendToFile(cluster, out_clustering_tmp)
    runner_utils.shellGetOutput(runner_utils.python_ver + " external/Tectonic/relabel-clusters.py " + use_input_graph + " " + out_clustering_tmp + " " + out_clustering + " " + out_prefix + ".pickle")
    runner_utils.appendToFile("Tectonic: \n", out_filename)
    runner_utils.appendToFile("Input graph: " + graph + "\n", out_filename)
    runner_utils.appendToFile(config + "\n", out_filename)
    runner_utils.appendToFile("Cluster Time: " + str(end_time - start_time) + "\n", out_filename)
    runtime_dict['Cluster Time'] = str(end_time - start_time)

    ## remove intermediate files
    runner_utils.shellGetOutput("rm " + out_prefix + ".triangles ")
    runner_utils.shellGetOutput("rm " + out_prefix + ".mace ")
    runner_utils.shellGetOutput("rm " + out_prefix + ".edges ")
    runner_utils.shellGetOutput("rm " + out_prefix + ".weighted ")
    runner_utils.shellGetOutput("rm " + out_prefix + ".tmpcluster ")
    runner_utils.shellGetOutput("rm " + out_prefix + ".pickle")

#cd external/Tectonic/
#cd mace; make
#python2 relabel-graph.py com-amazon.ungraph.txt com-amazon.top5000.cmty.txt amazon.mace amazon.communities
#mace/mace C -l 3 -u 3 amazon.mace amazon.triangles
#python2 mace-to-list.py amazon.mace amazon.edges
#python2 weighted-edges.py amazon.triangles amazon.edges amazon.weighted amazon.mixed $(head -n1 amazon.communities)
#g++-12 -std=c++11 -o tree-clusters tree-clusters.cpp
#./tree-clusters amazon.weighted 334863 > amazon.clusters
#python2 grade-clusters.py amazon.communities amazon.clusters amazon.grading 

def run_tigergraph(conn, clusterer, graph, thread, config, weighted, out_prefix, runtime_dict):
  if (runner_utils.gbbs_format == "true"):
    raise ValueError("Tigergraph can only be run using edge list format")
  use_input_graph = runner_utils.input_directory + graph
  out_clustering = out_prefix + ".cluster"
  out_filename = out_prefix + ".out"
  if runner_utils.postprocess_only != "true":
    out_time = cluster_tg.run_tigergraph(conn, clusterer, out_clustering, thread, config, weighted)
    runner_utils.appendToFile("Tigergraph: \n", out_filename)
    runner_utils.appendToFile("Clusterer: " + clusterer + "\n", out_filename)
    runner_utils.appendToFile("Input graph: " + graph + "\n", out_filename)
    runner_utils.appendToFile("Threads: " + str(thread) + "\n", out_filename)
    runner_utils.appendToFile("Config: " + config + "\n", out_filename)
    runner_utils.appendToFile(out_time, out_filename)
  print("postprocessing..." + out_filename)
  with open(out_filename,'r') as f:
    run_info = f.readlines()
    for elem in run_info[1:]:
      if elem.startswith('Total Time:'):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()

def runAll(config_filename):
  runner_utils.readConfig(config_filename)
  
  runtimes = []
  for graph_idx, graph in enumerate(runner_utils.graphs):
    if graph == "SKIP":
      continue
    neo4j_graph_loaded = False
    tigergraph_loaded = False
    conn = None
    for clusterer_idx, clusterer in enumerate(runner_utils.clusterers):
      if clusterer == "SKIP":
        continue
      try:
        if clusterer.startswith("Snap"):
          if not os.path.exists(runner_utils.output_directory):
            os.makedirs(runner_utils.output_directory)
          for i in range(runner_utils.num_rounds):
            runtime_dict = {}
            runtime_dict['Clusterer Name'] = clusterer
            runtime_dict["Input Graph"] = graph
            runtime_dict["Threads"] = 1
            runtime_dict["Config"] = ""
            runtime_dict["Round"] = i
            runtime_dict['Num Iterations'] = -1
            runSnap(clusterer, graph, graph_idx, i, runtime_dict)
            runtimes.append(runtime_dict)
          continue
        for thread_idx, thread in enumerate(runner_utils.num_threads):
          configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
          config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
          config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
          for config_idx, config in enumerate(configs):
            for i in range(runner_utils.num_rounds):
              runtime_dict = {}
              runtime_dict['Clusterer Name'] = clusterer
              runtime_dict["Input Graph"] = graph
              runtime_dict["Threads"] = thread
              runtime_dict["Config"] = config
              runtime_dict["Round"] = i
              runtime_dict['Num Iterations'] = -1
              out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
              if not os.path.exists(runner_utils.output_directory):
                os.makedirs(runner_utils.output_directory)
              if clusterer.startswith("NetworKit"):
                cluster_nk.runNetworKit(clusterer, graph, thread, config, out_prefix, runtime_dict)
              elif clusterer == "Tectonic":
                runTectonic(clusterer, graph, thread, config, out_prefix, runtime_dict)
              elif clusterer.startswith("Neo4j"):
                if int(thread) > 4:
                  print("neo4j only run up to 4 threads")
                  continue
                if (not neo4j_graph_loaded) and (runner_utils.postprocess_only != "true"):
                  use_input_graph = runner_utils.input_directory + graph
                  cluster_neo4j.projectGraph(graph, use_input_graph)
                  neo4j_graph_loaded = True
                weighted = runner_utils.weighted == "true"
                runNeo4j(clusterer, graph, thread, config + ', num_rounds: ' + str(i), weighted, out_prefix, runtime_dict)
              elif clusterer.startswith("TigerGraph"):
                weighted = runner_utils.weighted == "true"
                if (not tigergraph_loaded) and (runner_utils.postprocess_only != "true"):
                  conn = TigerGraphConnection(
                      host='http://127.0.0.1',
                      username='tigergraph',
                      password='tigergraph',
                  )
                  print("connected")
                  cluster_tg.remove_tigergraph(conn)
                  cluster_tg.load_tigergraph(conn, graph, runner_utils.input_directory, runner_utils.output_directory, runner_utils.tigergraph_nodes, runner_utils.tigergraph_edges, weighted)
                  tigergraph_loaded = True
                run_tigergraph(conn, clusterer, graph, thread, config, weighted, out_prefix, runtime_dict)
              else:
                out_filename = out_prefix + ".out"
                out_clustering = out_prefix + ".cluster"
                use_thread = "" if (thread == "" or thread == "ALL") else "PARLAY_NUM_THREADS=" + thread
                use_input_graph = runner_utils.input_directory + graph
                if (runner_utils.gbbs_format == "true" and "ungraph" in graph):
                  print("warning: use gbbs format is true, but seems like snap format is used from graph file name")
                ss = (use_thread + " " + runner_utils.timeout + " bazel run //clusterers:cluster-in-memory_main -- --"
                "input_graph=" + use_input_graph + " --is_gbbs_format=" + runner_utils.gbbs_format + " --float_weighted=" + runner_utils.weighted + " --clusterer_name=" + clusterer + " "
                "--clusterer_config='" + config_prefix + config + config_postfix + "' "
                "--output_clustering=" + out_clustering)
                if runner_utils.postprocess_only.lower() != "true":
                  print(ss)
                  out = runner_utils.shellGetOutput(ss)
                  runner_utils.appendToFile(ss + "\n", out_filename)
                  runner_utils.appendToFile(out, out_filename)
                print("postprocessing... " + out_filename)
                with open(out_filename,'r') as f:
                  run_info = f.readlines()
                  for elem in run_info[1:]:
                    if elem.startswith('Cluster Time:'):
                      runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
                    if elem.startswith('Num iterations:'):
                      runtime_dict['Num Iterations'] = elem.split(' ')[-1].strip()
              runtimes.append(runtime_dict)
      except Exception as e:
          # Print the stack trace
          traceback.print_exc()
    if neo4j_graph_loaded:
      cluster_neo4j.clearDB(graph)
    if tigergraph_loaded:
      cluster_tg.remove_tigergraph(conn)
    
    runtime_dataframe = pd.DataFrame(runtimes)
    if not os.path.exists(runner_utils.csv_output_directory):
      os.makedirs(runner_utils.csv_output_directory)
  runtime_dataframe.to_csv(runner_utils.csv_output_directory + '/runtimes.csv', mode='a',
                             columns=["Clusterer Name","Input Graph","Threads","Config","Round","Cluster Time","Num Iterations"])



def main():
  args = sys.argv[1:]
  runAll(args[0])

if __name__ == "__main__":
  main()