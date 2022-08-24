import os
import sys
import signal
import time
import subprocess
import re
import itertools
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import json
import runner_utils

def plotAll(xes, yes, labels, x_label, y_label, graph_name):
  colors = cm.rainbow(np.linspace(0, 1, len(labels)))
  fig, ax = plt.subplots()
  for label_idx, label_name in enumerate(labels):
    ax.scatter(xes[label_idx], yes[label_idx], color=colors[label_idx], label=label_name)
  ax.legend()
  ax.set_xlabel(x_label)
  ax.set_ylabel(y_label)
  plt.savefig(graph_name)

def isNumber(s, modifier_index, index):
  try:
    print(s)
    float(s)
    return float(s)
  except TypeError:
    try:
      float(s[modifier_index])
      return float(s[modifier_index])
    except TypeError:
      #s = s.replace("{", "")
      #s = s.replace("}", "")
      #s_list = [x.strip() for x in s.split(',')]
      return float(s[modifier_index][index])

def configPlotAll(
  x_axis, x_axis_modifier, x_axis_index, y_axis, y_axis_modifier, y_axis_index,
  legend, graph_name):
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
            xes[index].append(isNumber(parse_out_statistics[x_axis], x_axis_modifier, x_axis_index))
            yes[index].append(isNumber(parse_out_statistics[y_axis], y_axis_modifier, y_axis_index))
  plotAll(xes, yes, labels, x_axis + " " + x_axis_modifier + " " + x_axis_index, y_axis + " " + y_axis_modifier + " " + y_axis_index, runner_utils.output_directory + graph_name)

def runAll(config_filename, stats_config_filename, graph_config_filename):
  runner_utils.readConfig(config_filename)
  runner_utils.readStatsConfig(stats_config_filename)
  runner_utils.readGraphConfig(graph_config_filename)
  for i in range(len(runner_utils.output_graph_filename)):
    configPlotAll(
      runner_utils.x_axis[i], runner_utils.x_axis_modifier[i],
      runner_utils.x_axis_index[i], runner_utils.y_axis[i],
      runner_utils.y_axis_modifier[i], runner_utils.y_axis_index[i],
      runner_utils.legend[i], runner_utils.output_graph_filename[i])

def main():
  args = sys.argv[1:]
  runAll(args[0], args[1], args[2])

if __name__ == "__main__":
  main()