
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

#include "clusterers/metric_example_clusterer/metric-example-clusterer.h"

#include "google/protobuf/text_format.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/datapoint.h"
#include "parcluster/api/in-memory-metric-clusterer-base.h"
#include "parcluster/api/status_macros.h"

ABSL_FLAG(std::string, clusterer_name, "",
          "Name of a clusterer (e.g., ParallelAffinityClusterer).");

ABSL_FLAG(std::string, clusterer_config, "",
          "Text-format research_graph.in_memory.MetricClustererConfig proto.");

ABSL_FLAG(std::string, input_graph, "",
          "Input file pattern of a graph. Should be in edge list format "
          "(SNAP format).");

ABSL_FLAG(std::string, output_clustering, "",
          "Output filename of a clustering.");

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
                             const std::vector<int64_t>& clustering) {
  std::ofstream file{filename};
  if (!file.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  for (const auto& cluster_id : clustering) {
    file << cluster_id << std::endl;
  }
  file.close();
  return absl::OkStatus();
}

absl::StatusOr<std::vector<DataPoint>> ReadDataPoints(const char* filename) {
  std::vector<DataPoint> points;
  std::ofstream file{filename};
  if (!file.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  std::string line;
  while (std::getline(file, line)){
    std::stringstream line_stream(line);
    DataPoint point;
    float coordinate;
    while (line_stream >> coordinate) point.coordinates.push_back(coordinate);
    points.push_back(point);
  }
  file.close();
  return points;
}

absl::Status Main() {
  std::string clusterer_name = absl::GetFlag(FLAGS_clusterer_name);

  // Set up clusterer
  MetricClustererConfig config;
  std::string clusterer_config = absl::GetFlag(FLAGS_clusterer_config);
  if (!google::protobuf::TextFormat::ParseFromString(clusterer_config,
                                                     &config)) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Cannot parse --clusterer_config as a text-format "
                        "research_graph.in_memory.MetricClustererConfig proto: %s",
                        clusterer_config));
  }

  std::unique_ptr<InMemoryMetricClusterer> clusterer;
  if (clusterer_name == "MetricExampleClusterer") {
    clusterer.reset(new MetricExampleClusterer);
  }
  else {
    std::cerr << "Clusterer name = " << clusterer_name << std::endl;
    return absl::UnimplementedError("Unknown clusterer.");
  }

  // Read input points
  std::vector<DataPoint> points;
  auto begin_read = std::chrono::steady_clock::now();
  std::string input_file = absl::GetFlag(FLAGS_input_graph);
  ASSIGN_OR_RETURN(points, ReadDataPoints(input_file));
  auto end_read = std::chrono::steady_clock::now();
  PrintTime(begin_read, end_read, "Read");

  std::cout << "Num workers: " << parlay::num_workers() << std::endl;
  std::cout << "Point set: " << input_file << std::endl;


  // Cluster points
  std::vector<int64_t> clustering;
  auto begin_cluster = std::chrono::steady_clock::now();
  std::cout << "Calling clustering." << std::endl;
  ASSIGN_OR_RETURN(clustering, clusterer->Cluster(points, config));
  auto end_cluster = std::chrono::steady_clock::now();
  PrintTime(begin_cluster, end_cluster, "Cluster");

  // Write clustering to output file
  std::string output_file = absl::GetFlag(FLAGS_output_clustering);
  // TODO(laxmand): fix status warnings here (and potentially elsewhere).
  WriteClustering(output_file.c_str(), clustering);

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
