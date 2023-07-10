#include "clusterers/labelprop_clusterer/labelprop-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "clusterers/labelprop_clusterer/labelprop_config.pb.h"

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<LabelPropagationClusterer::Clustering>
LabelPropagationClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;

  LabelPropagationClustererConfig labelprop_config;
  config.any_config().UnpackTo(&labelprop_config);
  int max_iteration = labelprop_config.max_iteration();



  auto ret = LabelPropagationClusterer::Clustering();
  std::cout << "Num clusters = " << 1 << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
