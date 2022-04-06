#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_

#include "absl/flags/flag.h"
#include "absl/strings/string_view.h"
#include "google/protobuf/text_format.h"
#include "clusterers/clustering_stats.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"

namespace research_graph::in_memory {

class ClusteringStats{
 public:
  ClusteringStatistics GetStats(const InMemoryClusterer::Clustering& clustering,
      absl::string_view input_graph,
      const ClusteringStatsConfig& clustering_stats_config);
  GbbsGraph* MutableGraph() override { return &graph_; }

 private:
  GbbsGraph graph_;
  ClusteringStatistics clustering_stats_;
};

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_