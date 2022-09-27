import snap
import runner_utils
import time
import io
from contextlib import redirect_stdout

def runSnap(use_input_graph):#clusterer, graph, thread, config, out_prefix):
  #if (runner_utils.gbbs_format == "true"):
  #  raise ValueError("SNAP can only be run using edge list format")
  #out_filename = out_prefix + ".out"
  #out_clustering = out_prefix + ".cluster"
  #use_input_graph = runner_utils.input_directory + graph
  # Undirected graph
  G = snap.LoadEdgeList(snap.TUNGraph, use_input_graph, 0, 1, '\t')

  modularity, CmtyV = G.CommunityCNM()
  modularity, CmtyV = G.CommunityGirvanNewman()
  # CmtyV: A vector of all the communities that are detected by the CNM method. Each community is represented as a vector of node ids.
  #for Cmty in CmtyV:
  #  print("Community: ")
  #  for NI in Cmty:
  #      print(NI)

def main():
  args = sys.argv[1:]
  runSnap(args[0])

if __name__ == "__main__":
  main()