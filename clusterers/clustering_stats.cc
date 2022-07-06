#include "clusterers/clustering_stats.h"

namespace research_graph::in_memory {

ClusteringStatistics GetStats(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph, const std::string& input_communities,
  const ClusteringStatsConfig& clustering_stats_config) {
  ClusteringStatistics clustering_stats;
  clustering_stats.set_filename(input_graph);
  clustering_stats.set_num_clusters(clustering.size());

  set_aggregate_statistics(clustering.size(), [&](std::size_t i) {
    return clustering[i].size();
  }, clustering_stats.mutable_size());

  if (!input_communities.empty()) CompareCommunities(input_communities.c_str(), clustering, &clustering_stats);

  return clustering_stats;
}

}  // namespace research_graph::in_memory