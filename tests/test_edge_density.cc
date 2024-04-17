#include "gtest/gtest.h"

#include <vector>

#include "clusterers/clustering_stats.pb.h"
#include "clusterers/gbbs_graph_io.h"
#include "clusterers/stats/stats_density.h"
#include "google/protobuf/repeated_field.h"
#include "google/protobuf/text_format.h"
#include "in_memory/status_macros.h"

namespace research_graph::in_memory {

parlay::sequence<gbbs::uintE>
GetClusteringIds(const std::size_t n,
                 const std::vector<std::vector<gbbs::uintE>> &clustering) {
  parlay::sequence<gbbs::uintE> cluster_ids = parlay::sequence<gbbs::uintE>(n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i) {
    const auto &cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(),
                         [&](size_t j) { cluster_ids[cluster[j]] = i; });
  });
  return cluster_ids;
}

TEST(TestEdgeDensity, TestSimple) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {
      {0, 1}, // first cluster
  };
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0, 1, 0.5}};

  parlay::sequence<gbbs::uintE> cluster_ids = GetClusteringIds(n, clustering);

  GbbsGraph graph;
  ASSERT_OK_AND_ASSIGN(n,
                       internal::WriteEdgeListAsGraph(&graph, edge_list, true));

  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_edge_density(true);
  clustering_stats_config.set_include_zero_degree_nodes(true);

  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));

  EXPECT_EQ(1, clustering_stats.weighted_edge_density_mean());
  EXPECT_EQ(1, clustering_stats.edge_density().mean());
}

TEST(TestEdgeDensity, TestSingletonClusters) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {{0}, {1}, {2}, {3}};
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0, 1, 0.5},
                                                              {0, 3, 1}};

  parlay::sequence<gbbs::uintE> cluster_ids = GetClusteringIds(n, clustering);

  GbbsGraph graph;
  ASSERT_OK_AND_ASSIGN(n,
                       internal::WriteEdgeListAsGraph(&graph, edge_list, true));

  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_edge_density(true);

  clustering_stats_config.set_include_zero_degree_nodes(true);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_EQ(1, clustering_stats.weighted_edge_density_mean());
  EXPECT_EQ(1, clustering_stats.edge_density().mean());

  clustering_stats_config.set_include_zero_degree_nodes(false);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_EQ(1, clustering_stats.weighted_edge_density_mean());
  EXPECT_EQ(1, clustering_stats.edge_density().mean());
}

TEST(TestEdgeDensity, TestClique) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {
      {0, 1, 3},
      {2},
  };
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {
      {0, 1, 0.5}, {0, 3, 1}, {1, 3, 1}};

  parlay::sequence<gbbs::uintE> cluster_ids = GetClusteringIds(n, clustering);

  GbbsGraph graph;
  ASSERT_OK_AND_ASSIGN(n,
                       internal::WriteEdgeListAsGraph(&graph, edge_list, true));

  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_edge_density(true);

  clustering_stats_config.set_include_zero_degree_nodes(true);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_EQ(1, clustering_stats.weighted_edge_density_mean());
  EXPECT_EQ(1, clustering_stats.edge_density().mean());

  clustering_stats_config.set_include_zero_degree_nodes(false);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_EQ(1, clustering_stats.weighted_edge_density_mean());
  EXPECT_EQ(1, clustering_stats.edge_density().mean());
}

TEST(TestEdgeDensity, TestNonClique) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {
      {0, 1, 3},
      {2},
  };
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {
      {0, 1, 0.5}, {0, 3, 1}};

  parlay::sequence<gbbs::uintE> cluster_ids = GetClusteringIds(n, clustering);

  GbbsGraph graph;
  ASSERT_OK_AND_ASSIGN(n,
                       internal::WriteEdgeListAsGraph(&graph, edge_list, true));

  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_edge_density(true);

  clustering_stats_config.set_include_zero_degree_nodes(true);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_EQ(0.75, clustering_stats.weighted_edge_density_mean());
  EXPECT_DOUBLE_EQ(5.0/6, clustering_stats.edge_density().mean());

  clustering_stats_config.set_include_zero_degree_nodes(false);
  ASSERT_OK(ComputeEdgeDensity(graph, clustering, &clustering_stats,
                               cluster_ids, clustering_stats_config));
  EXPECT_DOUBLE_EQ(2.0/3, clustering_stats.weighted_edge_density_mean());
  EXPECT_DOUBLE_EQ(2.0/3, clustering_stats.edge_density().mean());
}

} // namespace research_graph::in_memory
