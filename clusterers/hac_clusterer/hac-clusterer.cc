#include "clusterers/hac_clusterer/hac-clusterer.h"

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

absl::StatusOr<std::vector<int64_t>> HACClusterer::Cluster(
      absl::Span<const DataPoint> datapoints,
      const MetricClustererConfig& config) const {
  const HACClustererConfig& hac_config = config.hac_clusterer_config();

  const HACClustererConfig_LinkageMethod linkage_method = hac_config.linkage_method();
  if(linkage_method== HACClustererConfig::COMPLETE){

  }else if(linkage_method== HACClustererConfig::AVERAGE){

  }else{ //should not reach here if all methods in proto are implemented
    std::cerr << "Linkage method = " << linkage_method << std::endl;
    return absl::UnimplementedError("Unknown linkage method.");
  }
  std::size_t n = datapoints.size();

  // Initially each vertex is its own cluster.
  std::vector<int64_t> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  std::cout << "Num clusters = " << cluster_ids.size() << std::endl;
  return cluster_ids;
}

}  // namespace in_memory
}  // namespace research_graph
