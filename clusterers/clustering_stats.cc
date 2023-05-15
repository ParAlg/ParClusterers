#include "clusterers/clustering_stats.h"

namespace research_graph::in_memory {

absl::StatusOr<ClusteringStatistics> GetStats(const GbbsGraph& graph, 
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

  std::vector<std::vector<gbbs::uintE>> communities;

  const bool compute_ari = clustering_stats_config.compute_ari();
  const bool compute_precision_recall = clustering_stats_config.compute_precision_recall();

  if (compute_ari || compute_precision_recall){
    if (input_communities.empty()){
      return absl::InvalidArgumentError(
        absl::StrFormat("input_communities is not provided."));
    }
    ReadCommunities(input_communities.c_str(), communities);
  }
  

  CompareCommunities(communities, clustering, &clustering_stats, clustering_stats_config);

  parlay::sequence<gbbs::uintE> cluster_ids = parlay::sequence<gbbs::uintE>(graph.Graph()->n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_ids[cluster[j]] = i;
    });
  });

  ComputeCorrelationObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  ComputeModularityObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  ComputeDiameter(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  ComputeEdgeDensity(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  ComputeTriangleDensity(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);

  size_t n = graph.Graph()->n;
  ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);


  return clustering_stats;
}

}  // namespace research_graph::in_memory