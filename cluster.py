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
  global input_directory, output_directory, clusterers, graphs, num_threads, clusterer_configs, num_rounds, timeout, clusterer_config_names, gbbs_format
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
  num_threads = [""] if num_threads is None or not num_threads else num_threads
  timeout = "" if (timeout is None or timeout == "" or timeout == "NONE") else "timeout " + timeout
  num_rounds = 1 if (num_rounds is None) else num_rounds
  gbbs_format = "false" if (gbbs_format is None or gbbs_format == "") else gbbs_format

def printAll():
  print(clusterers)
  print(clusterer_config_names)
  print(clusterer_configs)

def runAll(config_filename):
  readConfig(config_filename)
  printAll()
  for clusterer_idx, clusterer in enumerate(clusterers):
    for graph_idx, graph in enumerate(graphs):
      for thread_idx, thread in enumerate(num_threads):
        configs = clusterer_configs[clusterer_idx] if clusterer_configs[clusterer_idx] is not None else [""]
        config_prefix = clusterer_config_names[clusterer_idx] + "{" if clusterer_configs[clusterer_idx] is not None else ""
        config_postfix = "}" if clusterer_configs[clusterer_idx] is not None else ""
        for config_idx, config in enumerate(configs):
          for i in range(num_rounds):
            out_prefix = output_directory + clusterer + "_" + str(graph_idx) + "_" + thread + "_" + str(config_idx) + "_" + str(i)
            out_filename = out_prefix + ".out"
            out_clustering = out_prefix + ".cluster"
            use_thread = "" if (thread == "" or thread == "ALL") else "PARLAY_NUM_THREADS=" + thread
            use_input_graph = input_directory + graph
            ss = (use_thread + " " + timeout + " bazel run //clusterers:cluster-in-memory_main -- --"
            "input_graph=" + use_input_graph + " --is_gbbs_format=" + gbbs_format + " --clusterer_name=" + clusterer + " "
            "--clusterer_config='" + config_prefix + config + config_postfix + "' "
            "--output_clustering=" + out_clustering)
            out = shellGetOutput(ss)
            appendToFile(ss + "\n", out_filename)
            appendToFile(out, out_filename)

def main():
  args = sys.argv[1:]
  runAll(args[0])

if __name__ == "__main__":
  main()