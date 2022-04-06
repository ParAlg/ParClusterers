#include "clusterers/clustering_stats.h"

namespace research_graph::in_memory {

ClusteringStatistics ClusteringStats::GetStats(
  const InMemoryClusterer::Clustering& clustering,
  absl::string_view input_graph,
  const ClusteringStatsConfig& clustering_stats_config) {
    clustering_stats_.set_filename(input_graph);
    return clustering_stats_;
  }

}  // namespace research_graph::in_memory