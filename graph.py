import os
import sys
import signal
import time
import subprocess
import re
import itertools
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import json
import runner_utils

def plotAll(xes, yes, labels):
  colors = cm.rainbow(np.linspace(0, 1, len(labels)))
  fig, ax = plt.subplots()
  for label_idx, label_name in enumerate(labels):
    ax.scatter(xes[label_idx], yes[label_idx], color=colors[label_idx], label=label_name)
  ax.legend()
  plt.savefig('tmp.png')

def runAll(config_filename, stats_config_filename):
  x_axis = "communityPrecision"
  x_axis_modifier = "mean"
  y_axis = "communityRecall"
  y_axis_modifier = "mean"
  legend = "Graphs"
  runner_utils.readConfig(config_filename)
  runner_utils.readStatsConfig(stats_config_filename)
  labels = []
  if legend == "Graphs":
    labels = runner_utils.graphs
  elif legend == "Clusterers":
    labels = runner_utils.clusterers
  elif legend == "Number of threads":
    labels = runner_utils.num_threads
  elif legend == "Config":
    labels = ["Config " + str(x) for x in range(len(configs))]
  xes = [[] for x in range(len(labels))]
  yes = [[] for x in range(len(labels))]
  for clusterer_idx, clusterer in enumerate(runner_utils.clusterers):
    for graph_idx, graph in enumerate(runner_utils.graphs):
      for thread_idx, thread in enumerate(runner_utils.num_threads):
        configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
        config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
        for config_idx, config in enumerate(configs):
          index = 0
          if legend == "Graphs":
            index = graph_idx
          elif legend == "Clusterers":
            index = clusterer_idx
          elif legend == "Number of threads":
            index = thread_idx
          elif legend == "Config":
            index = config_idx
          for i in range(runner_utils.num_rounds):
            out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
            out_statistics = out_prefix + ".stats"
            out_statistics_file = open(out_statistics, "r")
            out_statistics_string = out_statistics_file.read()
            out_statistics_file.close()
            parse_out_statistics = json.loads(out_statistics_string)
            xes[index].append(float(parse_out_statistics[x_axis][x_axis_modifier]))
            yes[index].append(float(parse_out_statistics[y_axis][y_axis_modifier]))
  plotAll(xes, yes, labels)

def main():
  args = sys.argv[1:]
  runAll(args[0], args[1])

if __name__ == "__main__":
  main()