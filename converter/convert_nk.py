import networkit as nk
import sys
import time

input_dir = "/home/sy/mount-data/"

# convert from snap format to networkit binary format. Work for unweighted graphs.
for g in ["amazon", "youtube", "dblp", "lj", "orkut", "friendster"]:
  graph = "com-%s.ungraph.txt" % g
  path = "com-%s.bin" % g
  print(graph)

  start_time = time.time()
  reader = nk.graphio.EdgeListReader('\t', 0, commentPrefix='#', directed=False) #continuous=False, 
  G = reader.read(input_dir+graph)
  print(G.numberOfNodes(), G.numberOfEdges())
  end_time = time.time()
  print("Read Graph in %f \n" % (end_time - start_time))
  # Write the graph to a binary file
  # weightsType can be any of the following options: - none = 0, // The graph is not weighted - unsignedFormat = 1, //The weights are unsigned integers - signedFormat = 2, //The weights are signed integers - doubleFormat = 3, //The weights are doubles - floatFormat = 4, //The weights are floats - autoDetect
  nk.writeGraph(G,path, nk.Format.NetworkitBinary, chunks=16, NetworkitBinaryWeights=0)
  print("done")


# for g in ["amazon", "youtube", "dblp", "lj", "orkut", "friendster"]:
#     path = "com-%s.bin" % g
#     start_time = time.time()
#     # Read the graph back from the binary file
#     G_read = nk.readGraph(path, nk.Format.NetworkitBinary)
#     end_time = time.time()
#     print("Read Graph in %f \n" % (end_time - start_time))
#     print(G_read.numberOfNodes(), G_read.numberOfEdges())


# networkitBinaryReader = nk.graphio.NetworkitBinaryReader()
# G = networkitBinaryReader.read("../input/foodweb-baydry.networkit")