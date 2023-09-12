import networkit as nk
import time 
input_dir = "/home/sy/mount-data/"

path = input_dir + "com-amazon.bin"

start_time = time.time()
G = nk.readGraph(path, nk.Format.NetworkitBinary)
end_time = time.time()
print("Read Graph in %f \n" % (end_time - start_time))

G = nk.Graph(4, weighted=False, directed=False)
G.addEdge(0, 1)
G.addEdge(1, 2)
G.addEdge(2, 3)
G.addEdge(3, 0)


edge_list = [(u, v) for u, v in G.iterEdges()]

nodes = set()
edges_from = []
edges_to = []

for (u,v) in edge_list:
  nodes.add(u)
  nodes.add(v)
  edges_from.append(u)
  edges_to.append(v)
  edges_from.append(v)
  edges_to.append(u)

print(len(edge_list))

print(nodes)
print(edges_from)
print(edges_to)

# print(edge_list)
# def has_duplicate_edges(edge_list):
#     seen_edges = set()
#     for edge in edge_list:
#         # Convert each edge to a frozenset to be able to add it to a set
#         edge_set = frozenset(edge)
#         if edge_set in seen_edges:
#             return True
#         seen_edges.add(edge_set)
#     return False

# print(has_duplicate_edges(edge_list))  