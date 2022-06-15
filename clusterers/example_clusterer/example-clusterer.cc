#include "clusterers/example_clusterer/example-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "clusterers/example_clusterer/example_config.pb.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<ExampleClusterer::Clustering>
ExampleClusterer::Cluster(const ClustererConfig& config) const {
  ExampleClustererConfig example_config;
  config.any_config().unpack(&example_config);
  std::cout << "Num iterations: " << example_config.num_iterations() << std::endl;
  std::size_t n = graph_.Graph()->n;

  // Initially each vertex is its own cluster.
  parlay::sequence<gbbs::uintE> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(cluster_ids);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
