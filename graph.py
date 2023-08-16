import os
import sys
import signal
import time
import subprocess
import re
import math
import itertools
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import json
import runner_utils

def readClusterTime(filename):
  with open(filename, "r") as in_file:
    for line in in_file:
      line = line.strip()
      split = [x.strip() for x in line.split(':')]
      if split:
        if split[0].startswith("Cluster Time"):
          return float(split[1])
  return math.nan

def plotAll(xes, yes, labels, x_label, y_label, graph_name):
  colors = cm.rainbow(np.linspace(0, 1, len(labels)))
  fig, ax = plt.subplots()
  for label_idx, label_name in enumerate(labels):
    ax.scatter(xes[label_idx], yes[label_idx], color=colors[label_idx], label=label_name)
  ax.legend()
  ax.set_xlabel(x_label)
  ax.set_ylabel(y_label)
  plt.savefig(graph_name)

def isNumber(s, modifier_index):
  try:
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
      try:
        float(s[int(modifier_index)])
        return float(s[int(modifier_index)])
      except TypeError:
        return math.nan

def plotAllHelper(xes, yes, index, out_prefix, x_axis, x_axis_modifier, y_axis, y_axis_modifier, thread):
  out_statistics = out_prefix + ".stats"
  out_statistics_file = open(out_statistics, "r")
  out_statistics_string = out_statistics_file.read()
  out_statistics_file.close()
  parse_out_statistics = json.loads(out_statistics_string)
  if (x_axis == "threads"):
    xes[index].append(thread)
  elif (x_axis != "clusterTime"):
    xes[index].append(isNumber(parse_out_statistics[x_axis], x_axis_modifier))
  else:
    xes[index].append(readClusterTime(out_prefix + ".out"))
  if (y_axis == "threads"):
    yes[index].append(thread)
  elif (y_axis != "clusterTime"):
    yes[index].append(isNumber(parse_out_statistics[y_axis], y_axis_modifier))
  else:
    yes[index].append(readClusterTime(out_prefix + ".out"))

def configPlotAll(
  x_axis, x_axis_modifier, y_axis, y_axis_modifier, legend, graph_name):
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
      index = 0
      if legend == "Graphs":
        index = graph_idx
      elif legend == "Clusterers":
        index = clusterer_idx
      for i in range(runner_utils.num_rounds):
        if clusterer.startswith("Snap"):
          out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + str(i)
          plotAllHelper(xes, yes, index, out_prefix, x_axis, x_axis_modifier, y_axis, y_axis_modifier)
        else:
          configs = runner_utils.clusterer_configs[clusterer_idx] if runner_utils.clusterer_configs[clusterer_idx] is not None else [""]
          config_prefix = runner_utils.clusterer_config_names[clusterer_idx] + "{" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
          config_postfix = "}" if runner_utils.clusterer_configs[clusterer_idx] is not None else ""
          for config_idx, config in enumerate(configs):
            for thread_idx, thread in enumerate(runner_utils.num_threads):
              if legend == "Number of threads":
                index = thread_idx
              elif legend == "Config":
                index = config_idx
              out_prefix = runner_utils.output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
              plotAllHelper(xes, yes, index, out_prefix, x_axis, x_axis_modifier, y_axis, y_axis_modifier, thread)
  plotAll(xes, yes, labels, x_axis + " " + x_axis_modifier, y_axis + " " + y_axis_modifier, runner_utils.output_directory + graph_name)

def runAll(config_filename, stats_config_filename, graph_config_filename):
  runner_utils.readConfig(config_filename)
  runner_utils.readStatsConfig(stats_config_filename)
  runner_utils.readGraphConfig(graph_config_filename)
  for i in range(len(runner_utils.output_graph_filename)):
    configPlotAll(
      runner_utils.x_axis[i], runner_utils.x_axis_modifier[i],
      runner_utils.y_axis[i], runner_utils.y_axis_modifier[i], 
      runner_utils.legend[i], runner_utils.output_graph_filename[i])

def main():
  args = sys.argv[1:]
  runAll(args[0], args[1], args[2])

if __name__ == "__main__":
  main()