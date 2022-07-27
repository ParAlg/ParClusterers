import os
import sys
import signal
import time
import subprocess
import re
import itertools
import runner_utils

def runAll(config_filename, stats_config_filename):
  runner_utils.readConfig(config_filename)
  runner_utils.readStatsConfig(stats_config_filename)
  for clusterer_idx, clusterer in enumerate(runner_utils.clusterers):
    for graph_idx, graph in enumerate(runner_utils.graphs):
      for thread_idx, thread in enumerate(runner_utils.num_threads):
        configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
        config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        for config_idx, config in enumerate(configs):
          for i in range(runner_utils.num_rounds):
            out_prefix = runner_utils.output_directory + runner_utils.clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
            out_statistics = out_prefix + ".stats"
            in_clustering = out_prefix + ".cluster"
            use_input_graph = runner_utils.input_directory + runner_utils.graph
            use_input_communities = "" if not runner_utils.communities else "--input_communities=" + runner_utils.input_communities[graph_idx]
            ss = ("bazel run //clusterers:stats-in-memory_main -- "
            "--input_graph=" + use_input_graph + " "
            "--is_gbbs_format=" + runner_utils.gbbs_format + " "
            "--input_clustering=" + in_clustering + " "
            "--output_statistics=" + out_statistics + " " + use_input_communities + " "
            "--statistics_config='{" + stats_config + "}'")
            out = runner_utils.shellGetOutput(ss)
            # runner_utils.appendToFile(ss + "\n", out_filename)
            # runner_utils.appendToFile(out, out_filename)

def main():
  args = sys.argv[1:]
  runAll(args[0])

if __name__ == "__main__":
  main()