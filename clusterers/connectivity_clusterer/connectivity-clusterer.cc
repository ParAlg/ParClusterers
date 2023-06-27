#include "clusterers/connectivity_clusterer/connectivity-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "clusterers/connectivity_clusterer/connectivity_config.pb.h"
#include "external/gbbs/benchmarks/Connectivity/SimpleUnionAsync/Connectivity.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<ConnectivityClusterer::Clustering>
ConnectivityClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;
  ConnectivityClustererConfig connectivity_config;
  config.any_config().UnpackTo(&connectivity_config);
  double threshold = connectivity_config.threshold();
  bool upper_bound = connectivity_config.upper_bound();

  auto clusters = parlay::sequence<gbbs::uintE>::from_function(n, [&] (size_t i) { return i; });
  parlay::parallel_for(0, n, [&] (size_t i) {
    auto map_f = [&] (const auto& u, const auto& v, const auto& wgh) {
      if ((upper_bound && wgh <= threshold) || ((!upper_bound) && wgh >= threshold)){
        gbbs::simple_union_find::unite_impl(u, v, clusters);
      }
    };
    graph_.Graph()->get_vertex(i).out_neighbors().map(map_f);
  });

  parlay::parallel_for(0, n, [&] (size_t i) {
    gbbs::simple_union_find::find_compress(i, clusters);
  });

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(clusters);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
