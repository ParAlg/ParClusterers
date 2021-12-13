#include "spatial_clusterers/general_hac/general_hac.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
// #include "parcluster/api/gbbs-graph.h"
// #include "parcluster/api/in-memory-clusterer-base.h"
// #include "parcluster/api/parallel/parallel-graph-utils.h"
// #include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr< GeneralHACClusterer::Clustering>
GeneralHACClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = matrix_.n;

  // Initially each vertex is its own cluster.
  parlay::sequence<PointID> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  std::vector<std::vector<PointID>> ret;
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}


absl::StatusOr< GeneralHACClusterer::HierarchicalClustering>
GeneralHACClusterer::HierarchicalCluster(const ClustererConfig& config) const {
  std::size_t n = matrix_.n;

  // Initially each vertex is its own cluster.
  parlay::sequence<PointID> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  HierarchicalClustering ret;
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
