#include "clusterers/clustering_stats.h"
#include "stats_connectivity.h"

namespace research_graph::in_memory {

ClusteringStatistics ClusteringStats::GetStats(
  const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph,
  const ClusteringStatsConfig& clustering_stats_config) {
    auto clustering2 = std::vector<std::vector<gbbs::uintE>>();
    // std::vector<gbbs::uintE> c1{0,1,4};
    // std::vector<gbbs::uintE> c2{2,3};
    // clustering2.push_back(c1);
    // clustering2.push_back(c2);
    // ClusterTriangleDensity(clustering2, graph_);

    // ClusterConnectivity(clustering, graph_);
    // ClusterEdgeDensity(clustering, graph_);
    // ClusterTriangleDensity(clustering, graph_);
    clustering_stats_.set_filename(input_graph);
    return clustering_stats_;
  }

}  // namespace research_graph::in_memory