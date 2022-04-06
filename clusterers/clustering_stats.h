#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_

#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"

#include "google/protobuf/text_format.h"
#include "clusterers/clustering_stats.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph::in_memory {

class ClusteringStats{
 public:
  ClusteringStatistics GetStats(const InMemoryClusterer::Clustering& clustering,
      const std::string& input_graph,
      const ClusteringStatsConfig& clustering_stats_config);
  GbbsGraph* MutableGraph() { return &graph_; }

 private:
  GbbsGraph graph_;
  ClusteringStatistics clustering_stats_;
};

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_