import os
import sys
import signal
import time
import subprocess
import re
import itertools

def signal_handler(signal,frame):
  print "bye\n"
  sys.exit(0)
signal.signal(signal.SIGINT,signal_handler)

def shellGetOutput(str1) :
  process = subprocess.Popen(str1,shell=True,stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
  output, err = process.communicate()
  
  if (len(err) > 0):
    print(str1+"\n"+output+err)
  return output

def appendToFile(out, filename):
  with open(filename, "a+") as out_file:
    out_file.writelines(out)

def makeConfigCombos(current_configs):
  config_combos = itertools.product(*current_configs)
  config_combos_formatted = []
  for config in config_combos:
    config_combos_formatted.append(",".join(config))
  return config_combos_formatted

def readConfig(filename):
  global input_directory, output_directory, clusterers, graphs, num_threads
  global clusterer_configs, num_rounds, timeout, clusterer_config_names
  global gbbs_format
  num_threads = num_rounds = timeout = gbbs_format = None
  clusterers = []
  with open(filename, "r") as in_file:
    for line in in_file:
      line = line.strip()
      split = [x.strip() for x in line.split(':')]
      if split:
        if split[0].startswith("Input directory"):
          input_directory = split[1]
        elif split[0].startswith("Output directory"):
          output_directory = split[1]
        elif split[0].startswith("Clusterers"):
          clusterers = [x.strip() for x in split[1].split(';')]
          clusterer_configs = len(clusterers)*[None]
          clusterer_config_names = len(clusterers)*[None]
        elif split[0].startswith("Graphs"):
          graphs = [x.strip() for x in split[1].split(';')]
        elif split[0].startswith("Number of threads") and len(split) > 1:
          num_threads = [x.strip() for x in split[1].split(';')]
        elif split[0].startswith("Number of rounds") and len(split) > 1:
          num_rounds = 1 if split[1] == "" else int(split[1])
        elif split[0].startswith("Timeout") and len(split) > 1:
          timeout = split[1]
        elif split[0].startswith("GBBS format") and len(split) > 1:
          gbbs_format = split[1]
        else:
          for index, clusterer_name in enumerate(clusterers):
            if split[0].startswith(clusterer_name):
              clusterer_config_names[index] = in_file.next().strip()
              current_configs = []
              next_line = in_file.next().strip()
              while next_line != "":
                arg_name = next_line.split(':', 1)
                arg_name[0] = arg_name[0].strip()
                args = [x.strip() for x in arg_name[1].split(';')]
                current_configs.append([arg_name[0] + ": " + x for x in args])
                try:
                  next_line = in_file.next().strip()
                except StopIteration as err:
                  break
              clusterer_configs[index] = makeConfigCombos(current_configs)
              break
  num_threads = ["ALL"] if num_threads is None or not num_threads else num_threads
  timeout = "" if (timeout is None or timeout == "" or timeout == "NONE") else "timeout " + timeout
  num_rounds = 1 if (num_rounds is None) else num_rounds
  gbbs_format = "false" if (gbbs_format is None or gbbs_format == "") else gbbs_format

def readStatsConfig(filename):
  global communities, stats_config
  communities = []
  stats_config_list = []
  stats_config = ""
  with open(filename, "r") as in_file:
    for line in in_file:
      line = line.strip()
      split = [x.strip() for x in line.split(':')]
      if split:
        if split[0].startswith("Input communities") and len(split) > 1:
          communities = [x.strip() for x in split[1].split(';')]
        elif split[0].startswith("statistics_config"):
          next_line = in_file.next().strip()
          while next_line != "":
            stats_config_list.append(next_line)
            try:
              next_line = in_file.next().strip()
            except StopIteration as err:
              break
          stats_config = ",".join(stats_config_list)

def readGraphConfig(filename):
  global x_axis, x_axis_index, x_axis_modifier
  global y_axis, y_axis_index, y_axis_modifier
  global legend, output_graph_filename
  x_axis = []
  x_axis_index = []
  x_axis_modifier = []
  y_axis = []
  y_axis_index = []
  y_axis_modifier = []
  legend = []
  output_graph_filename = []
  with open(filename, "r") as in_file:
    for line in in_file:
      line = line.strip()
      split = [x.strip() for x in line.split(':')]
      if split:
        if split[0].startswith("x axis"):
          split = [x.strip() for x in split[1].split(' ')]
          x_axis.append(split[0])
          if len(split) > 1:
            x_axis_modifier.append(split[1])
          else:
            x_axis_modifier.append("")
          if len(split) > 2:
            x_axis_index.append(int(split[2]))
          else:
            x_axis_index.append(0)
        elif split[0].startswith("y axis"):
          split = [x.strip() for x in split[1].split(' ')]
          y_axis.append(split[0])
          if len(split) > 1:
            y_axis_modifier.append(split[1])
          else:
            y_axis_modifier.append("")
          if len(split) > 2:
            y_axis_index.append(int(split[2]))
          else:
            y_axis_index.append(0)
        elif split[0].startswith("Legend"):
          legend.append(split[1])
        elif split[0].startswith("Graph filename"):
          output_graph_filename.append(split[1])