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

#include "clusterers/gbbs_graph_io.h"
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

ABSL_FLAG(bool, is_gbbs_format, false,
          "Use this flag if the input file format is in the GBBS format."
          "Otherwise, the expectation is that the input file format is in"
          "an edge list format (or SNAP format).");

ABSL_FLAG(std::string, output_clustering, "",
          "Output filename of a clustering.");

ABSL_FLAG(bool, is_symmetric_graph, true,
          "Without this flag, the program expects the edge list to represent "
          "an undirected graph (each edge needs to be given in both "
          "directions). With this flag, the program symmetrizes the graph.");

ABSL_FLAG(bool, float_weighted, false,
          "Use this flag if the edge list is weighted with 32-bit floats. If "
          "this flag is not set, then the graph is assumed to be unweighted, "
          "and edge weights are automatically set to 1.");

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

bool IsAnyProto(const std::string& clusterer_name){
  return (clusterer_name == "ExampleClusterer");
}

std::string FormatClustererConfig(const std::string& clusterer_name, const std::string& clusterer_config){
  if (!IsAnyProto(clusterer_name)) return clusterer_config;
  std::size_t index_left_brace = clusterer_config.find('{');
  std::size_t index_right_brace = clusterer_config.rfind('}');
  if (index_left_brace == std::string::npos || index_right_brace == std::string::npos) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Cannot find left or right brace in --clusterer_config: %s",
                        clusterer_config));
  } else if (index_right_brace < index_left_brace) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Last right brace cannot be before first left brace --clusterer_config: %s",
                        clusterer_config));
  }
  std::string clusterer_config_formatted = "any_config {[type.googleapis.com/research_graph.in_memory.";
  clusterer_config_formatted.append(clusterer_name);
  clusterer_config_formatted.append("Config]");
  clusterer_config_formatted.append(clusterer_config, index_left_brace, index_right_brace - index_left_brace + 1);
  clusterer_config_formatted.append("}");
  return clusterer_config_formatted;
}

absl::Status Main() {
  std::string clusterer_name = absl::GetFlag(FLAGS_clusterer_name);

  ClustererConfig config;
  // "any_config {[type.googleapis.com/research_graph.in_memory.ExampleClustererConfig] { ... }}"
  std::string clusterer_config = absl::GetFlag(FLAGS_clusterer_config);

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

  std::string formatted_clusterer_config = FormatClustererConfig(clusterer_name, clusterer_config);
  if (!google::protobuf::TextFormat::ParseFromString(formatted_clusterer_config,
                                                     &config)) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Cannot parse --clusterer_config as a text-format "
                        "research_graph.in_memory.ClustererConfig proto: %s",
                        formatted_clusterer_config));
  }

  auto begin_read = std::chrono::steady_clock::now();
  std::string input_file = absl::GetFlag(FLAGS_input_graph);
  bool is_symmetric_graph = absl::GetFlag(FLAGS_is_symmetric_graph);
  bool float_weighted = absl::GetFlag(FLAGS_float_weighted);
  bool is_gbbs_format = absl::GetFlag(FLAGS_is_gbbs_format);

  std::size_t n = 0;
  if (!is_gbbs_format) {
    ASSIGN_OR_RETURN(n, ReadEdgeListGraphFormat(
      input_file, clusterer->MutableGraph(), float_weighted, is_symmetric_graph));
  } else {
    ASSIGN_OR_RETURN(n, ReadGbbsGraphFormat(
      input_file, clusterer->MutableGraph(), float_weighted));
  }

  auto end_read = std::chrono::steady_clock::now();
  PrintTime(begin_read, end_read, "Read");

  std::cout << "Num workers: " << parlay::num_workers() << std::endl;
  std::cout << "Graph: " << input_file << std::endl;
  std::cout << "Num vertices: " << n << std::endl;

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

  std::string output_file = absl::GetFlag(FLAGS_output_clustering);
  // TODO(laxmand): Fix status warnings here (and potentially elsewhere).
  // TODO(jeshi): Support writing entire dendrogram to output file
  return WriteClustering(output_file.c_str(), clusterings[0]);
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
