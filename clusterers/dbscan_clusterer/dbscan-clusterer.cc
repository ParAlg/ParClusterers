#include "clusterers/dbscan_clusterer/dbscan-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<std::vector<int64_t>> MetricDBSCANClusterer::Cluster(
      absl::Span<const DataPoint> datapoints,
      const MetricClustererConfig& config) const {
  std::size_t n = datapoints.size();
  double eps = config.metric_dbscan_clusterer_config().epsilon();
  long minPts = config.metric_dbscan_clusterer_config().min_pts();

  // Initially each vertex is its own cluster.
  std::vector<int64_t> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  std::cout << "Num clusters = " << cluster_ids.size() << std::endl;
  return cluster_ids;
}

}  // namespace in_memory
}  // namespace research_graph
