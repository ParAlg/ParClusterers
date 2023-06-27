#include "clusterers/scan_clusterer/scan-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "clusterers/scan_clusterer/scan_config.pb.h"
#include "external/gbbs/benchmarks/SCAN/IndexBased/scan.h"
#include "external/gbbs/benchmarks/SCAN/IndexBased/utils.h"
#include "external/gbbs/benchmarks/SCAN/IndexBased/similarity_measure.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<ScanClusterer::Clustering>
ScanClusterer::Cluster(const ClustererConfig& config) const {
  ScanClustererConfig scan_config;
  config.any_config().UnpackTo(&scan_config);
  std::size_t mu = scan_config.mu();
  double epsilon = scan_config.epsilon();

  std::cout << "Scan parameters: mu = " << mu << ", epsilon = " << epsilon
            << '\n';

  const gbbs::indexed_scan::Index scan_index{*(graph_.Graph()), gbbs::scan::CosineSimilarity{}};

  const gbbs::indexed_scan::Clustering clustering{scan_index.Cluster(mu, epsilon)};

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(clustering);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
