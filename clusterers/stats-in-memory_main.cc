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

#include "clusterers/clustering_stats.h"
#include "clusterers/clustering_stats.pb.h"
#include "clusterers/gbbs_graph_io.h"
#include "google/protobuf/text_format.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

ABSL_FLAG(std::string, input_graph, "",
          "Input file pattern of a graph. Should be in edge list format "
          "(SNAP format).");

ABSL_FLAG(bool, is_gbbs_format, false,
          "Use this flag if the input file format is in the GBBS format."
          "Otherwise, the expectation is that the input file format is in"
          "an edge list format (or SNAP format).");

ABSL_FLAG(std::string, input_clustering, "",
          "Input filename of a clustering.");

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

void split(const std::string& s, char delim, std::vector<InMemoryClusterer::NodeId>& elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::stoi(item));
  }
}

absl::StatusOr<InMemoryClusterer::Clustering> ReadClustering(const char* filename){
  InMemoryClusterer::Clustering clustering;
  std::ifstream file{filename};
  if (!file.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  std::string line;
  while (std::getline(file, line)) {
    std::vector<InMemoryClusterer::NodeId> cluster;
    split(line, '\t', cluster);
    clustering.push_back(std::move(cluster));
  }
  return clustering;
}

absl::Status Main() {
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
  bool is_gbbs_format = absl::GetFlag(FLAGS_is_gbbs_format);

  std::size_t n = 0;
  // TODO(jeshi): This is assuming we will always call stats
  if (!is_gbbs_format) {
    ASSIGN_OR_RETURN(n, ReadEdgeListGraphFormat(
      input_file, stats.MutableGraph(), float_weighted, is_symmetric_graph));
  } else {
    ASSIGN_OR_RETURN(n, ReadGbbsGraphFormat(
      input_file, stats.MutableGraph(), float_weighted));
  }

  auto end_read = std::chrono::steady_clock::now();
  PrintTime(begin_read, end_read, "Read");

  std::cout << "Num workers: " << parlay::num_workers() << std::endl;
  std::cout << "Graph: " << input_file << std::endl;
  std::cout << "Num vertices: " << n << std::endl;

  InMemoryClusterer::Clustering clustering;
  std::string input_clustering = absl::GetFlag(FLAGS_input_clustering);
  ASSIGN_OR_RETURN(clustering, ReadClustering(input_clustering.c_str()));

  std::string input_communities = absl::GetFlag(FLAGS_input_communities);
  std::string output_stats_file = absl::GetFlag(FLAGS_output_statistics);
  auto clustering_stats = stats.GetStats(clustering,
    absl::GetFlag(FLAGS_input_graph), stats_config);
  // TODO(jeshi): Properly write stats to file
  std::cout << "Graph name from stats: " << clustering_stats.filename() << std::endl;

  return absl::OkStatus();
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
