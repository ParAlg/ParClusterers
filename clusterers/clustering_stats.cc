#include "clusterers/clustering_stats.h"

namespace research_graph::in_memory {

ClusteringStatistics GetStats(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph, const std::string& input_communities,
  const ClusteringStatsConfig& clustering_stats_config) {
  ClusteringStatistics clustering_stats;
  clustering_stats.set_filename(input_graph);
  clustering_stats.set_number_nodes(graph.Graph()->n);
  clustering_stats.set_number_clusters(clustering.size());

  set_distribution_stats(clustering.size(), [&](std::size_t i) {
    return clustering[i].size();
  }, clustering_stats.mutable_cluster_sizes());

  if (!input_communities.empty()) CompareCommunities(input_communities.c_str(), clustering, &clustering_stats);

  parlay::sequence<gbbs::uintE> cluster_ids = parlay::sequence<gbbs::uintE>(graph.Graph()->n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_ids[cluster[j]] = i;
    });
  });

  ComputeCorrelationObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  ComputeModularityObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);

  return clustering_stats;
}

}  // namespace research_graph::in_memory