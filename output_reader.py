import sys
import os
import csv
import json
import pandas as pd

'''
Reads .out file for run information and runtime.
Input: 
filename: type: string, name of file to read
directory: type: string, directory of file

Output:
dictionary with relevant info
'''
def read_out(filename, directory):
  runtime_file = open(directory + '/' + filename,'r')
  run_info = runtime_file.readlines()
  runtime_dict = {}
  # Internal Clusterer
  if run_info[0].startswith('PARLAY_NUM_THREADS'):
    threads = run_info[0].split(' ')[0].split('=')[-1]
    runtime_dict['Threads'] = threads.strip()
    run_info_arr = run_info[0].split(' --')
    for elem in run_info_arr:
      if elem.startswith('input_graph'):
        runtime_dict['Input Graph'] = elem.split('/')[-1].strip()
      elif elem.startswith('clusterer_name'):
        runtime_dict['Clusterer Name'] = elem.split('=')[-1].strip()
      elif elem.startswith('clusterer_config'):
        runtime_dict['Config'] = elem.split('{')[-1][:-2].strip()
    for elem in run_info[1:]:
      if elem.startswith('Num vertices'):
        runtime_dict['Num Vertices'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Num clusters'):
        runtime_dict['Num clusters'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Cluster Time'):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Read Time'):
        runtime_dict['Read Time'] = elem.split(' ')[-1].strip()
  # Neo4j Clusterer
  elif run_info[0].startswith('GDS version:'):
    for elem in run_info[1:]:
      if elem.startswith('Graph:'):
        runtime_dict['Input Graph'] = elem.split(',')[0][8:].strip()
        runtime_dict['Clusterer Name'] = 'Neo4j' + elem.split(',')[1][8:].strip()
      elif elem.startswith("{'concurrency'"):
        runtime_dict['Threads'] = elem.split(',')[0].split(' ')[-1].strip()
        runtime_dict['Config'] = elem.split(',', 1)[1][:-2].strip()
      elif elem.startswith("Time:"):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
  # NetworKit Clusterer
  elif run_info[0].startswith('NetworKit:'):
    for elem in run_info[1:]:
      if elem.startswith('Clusterer:'):
        runtime_dict['Clusterer Name'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Threads:'):
        runtime_dict['Threads'] = elem.split(' ')[-1].strip()
      elif elem.startswith("Cluster Time:"):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Graph:'):
        runtime_dict['Input Graph'] = elem.split(' ')[-1].strip()
  # Snap Clusterer
  elif run_info[0].startswith('Snap:'):
    runtime_dict['Threads'] = 1
    runtime_dict['Config'] = ''
    for elem in run_info[1:]:
      if elem.startswith('Wealy Connected Component Time:') or elem.startswith('KCore Time:'):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Input graph:'):
        runtime_dict['Input Graph'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Output file'):
        runtime_dict['Clusterer Name'] = elem.split('/')[-1].split('_')[0].strip()
  # Tectonic Clusterer
  elif run_info[0].startswith('Tectonic:'):
    runtime_dict['Threads'] = 1
    runtime_dict['Config'] = run_info[2].strip()
    runtime_dict['Clusterer Name'] = 'Tectonic'
    for elem in run_info[1:]:
      if elem.startswith('Cluster Time:'):
        runtime_dict['Cluster Time'] = elem.split(' ')[-1].strip()
      elif elem.startswith('Input graph:'):
        runtime_dict['Input Graph'] = elem.split(' ')[-1].strip()
  
  runtime_file.close()
  return runtime_dict

'''
Reads .stats file for calculated stats.
Input: 
filename: type: string, name of file to read
directory: type: string, directory of file

Output:
dictionary with relevant info
'''
def read_stats(filename, directory):
  stats_file = open(directory + '/' + filename,'r')
  stats_dict = json.loads(stats_file.readline())
  flattened_dict = {}
  for elem in stats_dict:
    if type(stats_dict[elem]) != dict:
      flattened_dict[elem] = stats_dict[elem]
    else:
      for elem2 in stats_dict[elem]:
        flattened_dict[elem+'_'+elem2] = stats_dict[elem][elem2]
  stats_file.close()
  return flattened_dict

'''
Reads through given dictionary for run information and stats.
Input: 
directory: type: string, directory of files

Output:
creates csv files, 'runtimes.csv' and 'stats.csv' in given directory
'''
def read_files(directory):
  encode_directory = os.fsencode(directory)
  runtime_dataframe = pd.DataFrame()
  stats_dataframe = pd.DataFrame()

  for file in os.listdir(encode_directory):
    filename = os.fsdecode(file)
    # Read .out file for runtime
    if filename.endswith(".out"): 
      runtime_dataframe = pd.concat([runtime_dataframe, pd.DataFrame([read_out(filename, directory)])], ignore_index=True)    
    # Read .stats file   
    elif filename.endswith(".stats"): 
      stats_dict = read_stats(filename, directory)
      # Take run info from .out file
      try:
        runtime_dict = read_out(filename.split('.')[0] + '.out', directory)
        for col in ['Clusterer Name', 'Threads', 'Input Graph', 'Config']:
          stats_dict[col] = runtime_dict[col]
      except:
        pass
      stats_dataframe = pd.concat([stats_dataframe, pd.DataFrame([stats_dict])], ignore_index=True)
      
  stats_dataframe.to_csv(directory + '/stats.csv')
  runtime_dataframe.to_csv(directory + '/runtimes.csv')



def main():
  args = sys.argv[1:]
  read_files(args[0])

if __name__ == "__main__":
  main()