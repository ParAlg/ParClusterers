import os
import sys
import signal
import time
import subprocess
import re
import itertools
import runner_utils

# Graph must be in edge format
def runTectonic(clusterer, graph, graph_idx, round):
  python_ver = "python2"
  gplusplus_ver = "g++-12"
  use_input_graph = runner_utils.input_directory + graph
  out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + str(round)
  out_clustering = out_prefix + ".cluster"
  out_filename = out_prefix + ".out"
  runner_utils.shellGetOutput("(cd external/Tectonic/mace && make)")
  runner_utils.shellGetOutput(gplusplus_ver + " -std=c++11 -o external/Tectonic/tree-clusters external/Tectonic/tree-clusters.cpp")
  # Timing from here
  start_time = time.time()
  num_vert = runner_utils.shellGetOutput(python_ver + " external/Tectonic/relabel-graph-no-comm.py " + use_input_graph + " " + out_prefix + ".mace")
  runner_utils.shellGetOutput("external/Tectonic/mace/mace C -l 3 -u 3 "+ out_prefix + ".mace " + out_prefix + ".triangles")
  runner_utils.shellGetOutput(python_ver + " external/Tectonic/mace-to-list.py " + out_prefix + ".mace " + out_prefix + ".edges")
  runner_utils.shellGetOutput(python_ver + " external/Tectonic/weighted-edges.py " + out_prefix + ".triangles " + out_prefix + ".edges " + out_prefix + ".weighted " + out_prefix + ".mixed " + num_vert)
  cluster = runner_utils.shellGetOutput("external/Tectonic/tree-clusters " + out_prefix + ".weighted " + num_vert)
  end_time = time.time()
  # Output running time to out_filename
  runner_utils.appendToFile(cluster, out_clustering)
  runner_utils.appendToFile("Cluster Time: " + str(end_time - start_time), out_filename)

#cd external/Tectonic/
#cd mace; make
#python2 relabel-graph.py com-amazon.ungraph.txt com-amazon.top5000.cmty.txt amazon.mace amazon.communities
#mace/mace C -l 3 -u 3 amazon.mace amazon.triangles
#python2 mace-to-list.py amazon.mace amazon.edges
#python2 weighted-edges.py amazon.triangles amazon.edges amazon.weighted amazon.mixed $(head -n1 amazon.communities)
#g++-12 -std=c++11 -o tree-clusters tree-clusters.cpp
#./tree-clusters amazon.weighted 334863 > amazon.clusters
#python2 grade-clusters.py amazon.communities amazon.clusters amazon.grading 

def runAll(config_filename):
  runner_utils.readConfig(config_filename)
  for clusterer_idx, clusterer in enumerate(runner_utils.clusterers):
    for graph_idx, graph in enumerate(runner_utils.graphs):
      if clusterer == "Tectonic":
        for i in range(runner_utils.num_rounds):
          runTectonic(clusterer, graph, graph_idx, i)
        continue
      for thread_idx, thread in enumerate(runner_utils.num_threads):
        configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
        config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        for config_idx, config in enumerate(configs):
          for i in range(runner_utils.num_rounds):
            out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
            out_filename = out_prefix + ".out"
            out_clustering = out_prefix + ".cluster"
            use_thread = "" if (thread == "" or thread == "ALL") else "PARLAY_NUM_THREADS=" + thread
            use_input_graph = runner_utils.input_directory + graph
            ss = (use_thread + " " + runner_utils.timeout + " bazel run //clusterers:cluster-in-memory_main -- --"
            "input_graph=" + use_input_graph + " --is_gbbs_format=" + runner_utils.gbbs_format + " --clusterer_name=" + clusterer + " "
            "--clusterer_config='" + config_prefix + config + config_postfix + "' "
            "--output_clustering=" + out_clustering)
            out = runner_utils.shellGetOutput(ss)
            runner_utils.appendToFile(ss + "\n", out_filename)
            runner_utils.appendToFile(out, out_filename)

def main():
  args = sys.argv[1:]
  runAll(args[0])

if __name__ == "__main__":
  main()