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
  global input_directory, output_directory, clusterers, graphs, num_threads, clusterer_configs, num_rounds, timeout, clusterer_config_names
  clusterers = []
  with open(filename, "r") as in_file:
    for line in filename:
      line = line.strip()
      split = [x.strip() for x in line.split(':')]
      print(split)
      if split:
        if split[0].startswith("Input directory"):
          input_directory = split[1]
        elif split[0].startswith("Output directory"):
          output_directory = split[1]
        elif split[0].startswith("Clusterers"):
          clusterers = [x.strip() for x in line[1].split(';')]
          clusterer_configs = len(clusterers)*[None]
          clusterer_config_names = len(clusterers)*[None]
        elif split[0].startswith("Graphs"):
          graphs = [x.strip() for x in line[1].split(';')]
        elif split[0].startswith("Number of threads"):
          num_threads = [x.strip() for x in line[1].split(';')]
        elif split[0].startswith("Number of rounds"):
          num_rounds = int(split[1])
        elif split[0].startswith("Timeout"):
          timeout = split[1]
        else:
          for index, clusterer_name in enumerate(clusterers):
            if split[0].startswith(clusterer_name):
              clusterer_config_names[index] = filename.next().strip()
              current_configs = []
              next_line = filename.next().strip()
              while next_line:
                arg_name = next_line.split(':', 1)
                arg_name[0] = arg_name[0].strip()
                args = [x.strip() for x in arg_name[1].split(';')]
                current_configs.append([arg_name[0] + ": " + x for x in args])
                next_line = filename.next().strip()
              clusterer_configs[index] = makeConfigCombos(current_configs)
              break

def printAll():
  print(input_directory)

def runAll(config_filename):
  readConfig(config_filename)
  printAll()
  for clusterer_idx, clusterer in enumerate(clusterers):
    for graph_idx, graph in enumerate(graphs):
      for thread_idx, thread in enumerate(num_threads):
        for config_idx, config in enumerate(clusterer_configs[clusterer_idx]):
          out_filename = output_directory + clusterer + "_" + graph_idx + "_" + thread + "_" + config_idx + ".out"
          for i in range(num_rounds):
            out_clustering = output_directory + clusterer + "_" + graph_idx + "_" + thread + "_" + config_idx + "_" + str(i) + ".cluster"
            use_thread = "" if (thread == "ALL") else "NUM_THREADS=" + thread
            use_timeout = "" if (timeout == "NONE") else "timeout " + timeout
            use_input_graph = input_directory + graph
            ss = (use_thread + " " + use_timeout + " bazel run //clustering:cluster-in-memory_main -- --"
            "input_graph=" + use_input_graph + " --clusterer_name=" + clusterer + " "
            "--clusterer_config='" + clusterer_config_names[clusterer_idx] + "{" + config + "}' "
            "--output_clustering=" + out_clustering)
            out = shellGetOutput(ss)
            appendToFile(out, out_filename)

def main():
  args = sys.argv[1:]
  runAll(args[0])

if __name__ == "__main__":
  main()