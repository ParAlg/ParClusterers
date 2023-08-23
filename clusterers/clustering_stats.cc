#include "clusterers/clustering_stats.h"
#include <chrono>

namespace research_graph::in_memory {

namespace {
  void PrintTime(std::chrono::steady_clock::time_point begin,
               std::chrono::steady_clock::time_point end,
               const std::string& input) {
  std::cout << input << " Time: "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                      begin)
                    .count()) /
                   1000000.0
            << std::endl;
}
}

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
  const bool compute_nmi = clustering_stats_config.compute_nmi();

  auto begin_read = std::chrono::steady_clock::now();
  if (compute_ari || compute_precision_recall || compute_nmi){
    if (input_communities.empty()){
      return absl::InvalidArgumentError(
        absl::StrFormat("input_communities is not provided."));
    }
    ReadCommunities(input_communities.c_str(), communities);
  }
  auto end_read = std::chrono::steady_clock::now();
  PrintTime(begin_read, end_read, "Read Commmunities");

  CompareCommunities(communities, clustering, &clustering_stats, clustering_stats_config);

  auto end_comm = std::chrono::steady_clock::now();
  PrintTime(end_read, end_comm, "Compare Commmunities");

  parlay::sequence<gbbs::uintE> cluster_ids = parlay::sequence<gbbs::uintE>(graph.Graph()->n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_ids[cluster[j]] = i;
    });
  });

  ComputeCorrelationObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  auto end_corr = std::chrono::steady_clock::now();
  PrintTime(end_comm, end_corr, "Compute Correlation");
  ComputeModularityObjective(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  auto end_modularity = std::chrono::steady_clock::now();
  PrintTime(end_corr, end_modularity, "Compute Modularity");
  ComputeDiameter(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  auto end_diameter = std::chrono::steady_clock::now();
  PrintTime(end_modularity, end_diameter, "Compute Diameter");
  ComputeEdgeDensity(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  auto end_edge_density = std::chrono::steady_clock::now();
  PrintTime(end_diameter, end_edge_density, "Compute EdgeDensity");
  ComputeTriangleDensity(graph, clustering, &clustering_stats, cluster_ids, clustering_stats_config);
  auto end_triangle_density = std::chrono::steady_clock::now();
  PrintTime(end_edge_density, end_triangle_density, "Compute Triangle Density");

  size_t n = graph.Graph()->n;
  ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  auto end_ari = std::chrono::steady_clock::now();
  PrintTime(end_triangle_density, end_ari, "Compute ARI");
  ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  auto end_nmi = std::chrono::steady_clock::now();
  PrintTime(end_ari, end_nmi, "Compute NMI");


  return clustering_stats;
}

}  // namespace research_graph::in_memory