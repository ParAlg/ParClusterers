import sys
import time
import load_tg

input_dir = "/home/sy/mount-data/"

## convert from snap format to tigergraph format.
for g in ["friendster"]: # "youtube","amazon" ,  "dblp", "lj", "orkut", 
  graph = "com-%s.bin" % g
  print(graph)


  start_time = time.time()
  load_tg.convert_to_tigergraph_format(graph, '/home/sy/mount-data/', '/home/sy/ParClusterers/results/', True)
  end_time = time.time()
  print("Converted Graph in %f \n" % (end_time - start_time))
