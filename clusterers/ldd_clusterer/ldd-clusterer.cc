#include "clusterers/ldd_clusterer/ldd-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "clusterers/ldd_clusterer/ldd_config.pb.h"

#include "absl/status/statusor.h"
#include "external/gbbs/benchmarks/LowDiameterDecomposition/MPX13/LowDiameterDecomposition.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<LDDClusterer::Clustering>
LDDClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;

  LDDClustererConfig ldd_config;
  config.any_config().UnpackTo(&ldd_config);
  double beta = ldd_config.beta();

  // Partitions vertices of an n-vertex, m-edge graph into subsets where each
  // subset has O((log n) / beta) diameter and at most O(beta * m) edges exiting
  // the subset.
  auto clusters = gbbs::LDD(*(graph_.Graph()), beta = beta);

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(clusters);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
