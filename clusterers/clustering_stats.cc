#include "clusterers/clustering_stats.h"

namespace research_graph::in_memory {

ClusteringStatistics GetStats(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph, const std::string& input_communities,
  const ClusteringStatsConfig& clustering_stats_config) {
    ClusteringStatistics clustering_stats;
    clustering_stats.set_filename(input_graph);

    if (!input_communities.empty()) CompareCommunities(input_communities.c_str(), clustering);

    return clustering_stats;
  }

}  // namespace research_graph::in_memory