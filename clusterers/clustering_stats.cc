#include "clusterers/clustering_stats.h"
#include "stats_connectivity.h"

namespace research_graph::in_memory {

ClusteringStatistics ClusteringStats::GetStats(
  const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph,
  const ClusteringStatsConfig& clustering_stats_config) {
    // ClusterConnectivity(clustering, graph_);
    // ClusterEdgeDensity(clustering, graph_);
    // ClusterTriangleDensity(clustering, graph_);
    // ClusterDiameter(clustering, graph_);
    clustering_stats_.set_filename(input_graph);
    return clustering_stats_;
  }

}  // namespace research_graph::in_memory