// Copyright 2020 The Google Research Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"

#include "clusterers/affinity/parallel-affinity.h"
#include "clusterers/connectivity_clusterer/connectivity-clusterer.h"
#include "clusterers/example_clusterer/example-clusterer.h"
#include "clusterers/kcore_clusterer/kcore-clusterer.h"
#include "clusterers/ldd_clusterer/ldd-clusterer.h"

#include "clusterers/clustering_stats.h"
#include "clusterers/clustering_stats.pb.h"
#include "google/protobuf/text_format.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

ABSL_FLAG(std::string, clusterer_name, "",
          "Name of a clusterer (e.g., ParallelAffinityClusterer).");

ABSL_FLAG(std::string, clusterer_config, "",
          "Text-format research_graph.in_memory.ClustererConfig proto.");

ABSL_FLAG(std::string, input_graph, "",
          "Input file pattern of a graph. Should be in edge list format "
          "(SNAP format).");

ABSL_FLAG(std::string, output_clustering, "",
          "Output filename of a clustering.");

ABSL_FLAG(std::string, output_statistics, "",
          "Output filename for clustering statistics.");

ABSL_FLAG(bool, is_symmetric_graph, true,
          "Without this flag, the program expects the edge list to represent "
          "an undirected graph (each edge needs to be given in both "
          "directions). With this flag, the program symmetrizes the graph.");

ABSL_FLAG(bool, float_weighted, false,
          "Use this flag if the edge list is weighted with 32-bit floats. If "
          "this flag is not set, then the graph is assumed to be unweighted, "
          "and edge weights are automatically set to 1.");

ABSL_FLAG(std::string, input_communities, "",
          "Input file pattern of a list of communities; tab separated nodes, "
          "lines separating communities.");

ABSL_FLAG(std::string, statistics_config, "",
          "Text-format research_graph.in_memory.ClusteringStatsConfig proto.");

namespace research_graph {
namespace in_memory {
namespace {

void PrintTime(std::chrono::steady_clock::time_point begin,
               std::chrono::steady_clock::time_point end,
               const std::string& input) {
  std::cout << input << " Time: "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                      begin)
                    .count()) /
                   1000000.0
            << std::endl;
}

double DoubleFromWeight(gbbs::empty weight) { return static_cast<double>(1); }
double DoubleFromWeight(double weight) { return weight; }

float FloatFromWeight(float weight) { return weight; }
float FloatFromWeight(gbbs::empty weight) { return static_cast<float>(1); }

template <class Graph>
absl::Status GbbsGraphToInMemoryClustererGraph(InMemoryClusterer::Graph* graph,
                                               Graph& gbbs_graph) {
  using weight_type = typename Graph::weight_type;
  for (std::size_t i = 0; i < gbbs_graph.n; i++) {
    auto vertex = gbbs_graph.get_vertex(i);
    std::vector<std::pair<gbbs::uintE, double>> outgoing_edges(
        vertex.out_degree());
    gbbs::uintE index = 0;
    auto add_outgoing_edge = [&](gbbs::uintE, const gbbs::uintE neighbor,
                                 weight_type wgh) {
      outgoing_edges[index] = std::make_pair(static_cast<gbbs::uintE>(neighbor),
                                             DoubleFromWeight(wgh));
      index++;
    };
    vertex.out_neighbors().map(add_outgoing_edge, false);
    InMemoryClusterer::Graph::AdjacencyList adjacency_list{
        static_cast<InMemoryClusterer::NodeId>(i), 1,
        std::move(outgoing_edges)};
    RETURN_IF_ERROR(graph->Import(adjacency_list));
  }
  RETURN_IF_ERROR(graph->FinishImport());
  return absl::OkStatus();
}

template <typename Weight>
absl::StatusOr<std::size_t> WriteEdgeListAsGraph(
    InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<Weight>>& edge_list,
    bool is_symmetric_graph) {
  std::size_t n = 0;
  if (is_symmetric_graph) {
    auto gbbs_graph{gbbs::gbbs_io::edge_list_to_symmetric_graph(edge_list)};
    n = gbbs_graph.n;
    auto status = GbbsGraphToInMemoryClustererGraph<
        gbbs::symmetric_graph<gbbs::symmetric_vertex, Weight>>(graph,
                                                               gbbs_graph);
    RETURN_IF_ERROR(status);
  } else {
    auto gbbs_graph{gbbs::gbbs_io::edge_list_to_asymmetric_graph(edge_list)};
    n = gbbs_graph.n;
    auto status = GbbsGraphToInMemoryClustererGraph<
        gbbs::asymmetric_graph<gbbs::asymmetric_vertex, Weight>>(graph,
                                                                 gbbs_graph);
    RETURN_IF_ERROR(status);
  }
  return n;
}

absl::Status WriteClustering(const char* filename,
                             InMemoryClusterer::Clustering clustering) {
  std::ofstream file{filename};
  if (!file.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  for (gbbs::uintE i = 0; i < clustering.size(); i++) {
    for (auto node_id : clustering[i]) {
      file << node_id << "\t";
    }
    file << std::endl;
  }
  return absl::OkStatus();
}

void split(const std::string& s, char delim, std::vector<gbbs::uintE>& elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::stoi(item));
  }
}

struct FakeGraph {
  std::size_t n;
};

template <class Graph>
gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float> CopyGraph(
    Graph& graph) {
  using vertex_data = gbbs::symmetric_vertex<float>;
  using edge_type = std::tuple<gbbs::uintE, float>;
  auto vd = gbbs::new_array_no_init<vertex_data>(graph.n);
  auto ed = gbbs::new_array_no_init<edge_type>(graph.m);
  parlay::parallel_for(0, graph.n, [&](size_t i) {
    vd[i].degree = graph.v_data[i].degree;
    vd[i].neighbors = ed + graph.v_data[i].offset;
    vd[i].id = i;
  });
  parlay::parallel_for(0, graph.m, [&](size_t i) {
    ed[i] = std::make_tuple(
        std::get<0>(graph.e0[i]),
        FloatFromWeight(std::get<1>(graph.e0[i])));  // graph.e0[i];
  });
  size_t n = graph.n;
  size_t m = graph.m;
  auto g = gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>(
      graph.n, graph.m, vd, [vd, ed, n, m]() {
        gbbs::free_array(vd, n);
        gbbs::free_array(ed, m);
      });
  return g;
}

absl::Status Main() {
  std::string clusterer_name = absl::GetFlag(FLAGS_clusterer_name);

  ClustererConfig config;
  std::string clusterer_config = absl::GetFlag(FLAGS_clusterer_config);
  if (!google::protobuf::TextFormat::ParseFromString(clusterer_config,
                                                     &config)) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Cannot parse --clusterer_config as a text-format "
                        "research_graph.in_memory.ClustererConfig proto: %s",
                        clusterer_config));
  }

  std::unique_ptr<InMemoryClusterer> clusterer;
  bool is_hierarchical = false;
  if (clusterer_name == "ParallelAffinityClusterer") {
    clusterer.reset(new ParallelAffinityClusterer);
  } else if (clusterer_name == "ExampleClusterer") {
    clusterer.reset(new ExampleClusterer);
  } else if (clusterer_name == "LDDClusterer") {
    clusterer.reset(new LDDClusterer);
  }  else if (clusterer_name == "ConnectivityClusterer") {
    clusterer.reset(new ConnectivityClusterer);
  }  else if (clusterer_name == "KCoreClusterer") {
    clusterer.reset(new KCoreClusterer);
  }
  else {
    std::cerr << "Clusterer name = " << clusterer_name << std::endl;
    return absl::UnimplementedError("Unknown clusterer.");
  }

  ClusteringStats stats;
  ClusteringStatsConfig stats_config;
  std::string clusterer_stats_config = absl::GetFlag(FLAGS_statistics_config);
  if (!google::protobuf::TextFormat::ParseFromString(clusterer_stats_config,
                                                     &stats_config)) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Cannot parse --statistics_config as a text-format "
                        "research_graph.in_memory.ClusteringStatsConfig proto: %s",
                        clusterer_stats_config));
  }

  auto begin_read = std::chrono::steady_clock::now();
  std::string input_file = absl::GetFlag(FLAGS_input_graph);
  bool is_symmetric_graph = absl::GetFlag(FLAGS_is_symmetric_graph);
  bool float_weighted = absl::GetFlag(FLAGS_float_weighted);

  std::size_t n = 0;
  if (float_weighted) {
    const auto edge_list{
        gbbs::gbbs_io::read_weighted_edge_list<float>(input_file.c_str())};
    ASSIGN_OR_RETURN(n, WriteEdgeListAsGraph(clusterer->MutableGraph(),
                                             edge_list, is_symmetric_graph));
  } else {
    const auto edge_list{
        gbbs::gbbs_io::read_unweighted_edge_list(input_file.c_str())};
    ASSIGN_OR_RETURN(n, WriteEdgeListAsGraph(clusterer->MutableGraph(),
                                             edge_list, is_symmetric_graph));
  }
  // Must initialize the list allocator for GBBS, to support parallelism.
  // The list allocator seeds using the number of vertices in the input graph.
  FakeGraph fake_graph{n};
  gbbs::alloc_init(fake_graph);

  // TODO(jeshi): This is assuming we will always call stats
  stats.MutableGraph()->graph_ = clusterer->MutableGraph()->graph_;

  auto end_read = std::chrono::steady_clock::now();
  PrintTime(begin_read, end_read, "Read");

  std::cout << "Num workers: " << parlay::num_workers() << std::endl;
  std::cout << "Graph: " << input_file << std::endl;

  std::vector<InMemoryClusterer::Clustering> clusterings;

  auto begin_cluster = std::chrono::steady_clock::now();
  std::cout << "Calling clustering." << std::endl;
  if (is_hierarchical) {
    // TODO(jeshi): Not implemented
  } else {
    InMemoryClusterer::Clustering clustering;
    ASSIGN_OR_RETURN(clustering, clusterer->Cluster(config));
    clusterings.push_back(std::move(clustering));
  }
  auto end_cluster = std::chrono::steady_clock::now();
  PrintTime(begin_cluster, end_cluster, "Cluster");

  std::string input_communities = absl::GetFlag(FLAGS_input_communities);
  std::string output_stats_file = absl::GetFlag(FLAGS_output_statistics);
  auto clustering_stats = stats.GetStats(clusterings[0],
    absl::GetFlag(FLAGS_input_graph), stats_config);
  // TODO(jeshi): Properly write stats to file
  std::cout << "Graph name from stats: " << clustering_stats.filename() << std::endl;

  std::string output_file = absl::GetFlag(FLAGS_output_clustering);
  // TODO(laxmand): Fix status warnings here (and potentially elsewhere).
  // TODO(jeshi): Support writing entire dendrogram to output file
  return WriteClustering(output_file.c_str(), clusterings[0]);

  // return absl::OkStatus();
}

}  // namespace
}  // namespace in_memory
}  // namespace research_graph

int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);
  auto status = research_graph::in_memory::Main();
  if (!status.ok()) {
    std::cerr << status << std::endl;
    return EXIT_FAILURE;
  }
}
