import os
import sys
import signal
import time
import subprocess
import re
import itertools
import runner_utils
import json
import pandas as pd

def runStats(out_prefix, graph, graph_idx, stats_dict):
  out_statistics = out_prefix + ".stats"
  in_clustering = out_prefix + ".cluster"
  if not os.path.exists(in_clustering) or not os.path.getsize(in_clustering) > 0:
    # Either an error or a timeout happened
    runner_utils.appendToFile("ERROR", out_statistics)
    return
  use_input_graph = runner_utils.input_directory + graph
  use_input_communities = "" if not runner_utils.communities else "--input_communities=" + runner_utils.input_directory + runner_utils.communities[graph_idx]
  ss = ("bazel run //clusterers:stats-in-memory_main -- "
  "--input_graph=" + use_input_graph + " "
  "--is_gbbs_format=" + runner_utils.gbbs_format + " "
  "--float_weighted=" + runner_utils.weighted + " "
  "--input_clustering=" + in_clustering + " "
  "--output_statistics=" + out_statistics + " " + use_input_communities + " "
  "--statistics_config='" + runner_utils.stats_config + "'")
  if(runner_utils.postprocess_only == "false"):
    print(ss)
    out = runner_utils.shellGetOutput(ss)

  out_statistics_file = open(out_statistics, "r")
  out_statistics_string = out_statistics_file.read()
  out_statistics_file.close()
  parse_out_statistics = json.loads(out_statistics_string)
  for k in parse_out_statistics:
    v = parse_out_statistics[k]
    if type(v) != dict:
      stats_dict[k] = v
    else:
      for elem2 in v:
        stats_dict[k+'_'+elem2] = v[elem2]



def runAll(config_filename, stats_config_filename):
  runner_utils.readConfig(config_filename)
  runner_utils.readStatsConfig(stats_config_filename)
  stats = []
  for clusterer_idx, clusterer in enumerate(runner_utils.clusterers):
    for graph_idx, graph in enumerate(runner_utils.graphs):
      if clusterer.startswith("Snap"):
        for i in range(runner_utils.num_rounds):
          if runner_utils.deterministic == "true" and i != 0:
            continue
          out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + str(i)
          stats_dict = {}
          stats_dict['Clusterer Name'] = clusterer
          stats_dict["Input Graph"] = graph
          stats_dict["Threads"] = 1
          stats_dict["Config"] = None
          stats_dict["Round"] = i
          runStats(out_prefix, graph, graph_idx, stats_dict)
          stats.append(stats_dict)
        continue
      for thread_idx, thread in enumerate(runner_utils.num_threads):
        if runner_utils.deterministic == "true" and thread != runner_utils.num_threads[0]:
            continue
        configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
        config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        for config_idx, config in enumerate(configs):
          for i in range(runner_utils.num_rounds):
            if runner_utils.deterministic == "true" and i != 0:
              continue
            out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
            stats_dict = {}
            stats_dict['Clusterer Name'] = clusterer
            stats_dict["Input Graph"] = graph
            stats_dict["Threads"] = thread
            stats_dict["Config"] = config
            stats_dict["Round"] = i
            runStats(out_prefix, graph, graph_idx, stats_dict)
            stats_dict["Ground Truth"] = runner_utils.communities[graph_idx]
            stats.append(stats_dict)
  stats_dataframe = pd.DataFrame(stats)
  if not os.path.exists(runner_utils.csv_output_directory):
    os.makedirs(runner_utils.csv_output_directory)
  stats_dataframe.to_csv(runner_utils.csv_output_directory + '/stats.csv', mode='a')

def main():
  args = sys.argv[1:]
  runAll(args[0], args[1])

if __name__ == "__main__":
  main()