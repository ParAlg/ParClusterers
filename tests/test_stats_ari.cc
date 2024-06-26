#include "gtest/gtest.h"

#include <vector>

#include "clusterers/stats/stats_ari.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"
#include "clusterers/clustering_stats.pb.h"

using research_graph::in_memory::ComputeARI;
using research_graph::in_memory::ClusteringStatistics;
using research_graph::in_memory::ClusteringStatsConfig;


TEST(TestARI, TestAllSame) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    { 1, 2, 3 }, // first cluster
    { 4, 5, 6 }, // second cluster
    { 7, 8, 9 }  // third cluster
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    { 3, 2, 1 }, // first cluster
    { 4, 5, 6 }, // second cluster
    { 9, 8, 7 }  // third cluster
};
  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_ari(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_EQ(1, clustering_stats.ari());
}

TEST(TestARI, TestAllSameDifferentOrder) {
  size_t n = 9;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    { 4, 5, 6 }, // second cluster
    { 1, 2, 3 }, // first cluster
    { 7, 8, 9 }  // third cluster
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    { 3, 2, 1 }, // first cluster
    { 4, 5, 6 }, // second cluster
    { 9, 8, 7 }  // third cluster
};
  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_ari(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_EQ(1, clustering_stats.ari());
}

TEST(TestARI, TestOneDifferent) {
  size_t n = 3;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    { 1, 2 }, // first cluster
    { 3 }  // second cluster
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    { 3, 2}, // first cluster
    { 1}, // second cluster
};  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_ari(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_DOUBLE_EQ(-0.5, clustering_stats.ari());
}


TEST(TestARI, TestEmpty) {
  size_t n = 3;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    { 1, 2, 3 }, // first cluster
    {  }  // second cluster
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    { 3, 2}, // first cluster
    { 1}, // second cluster
};  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_ari(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_DOUBLE_EQ(0, clustering_stats.ari());
}



TEST(TestARI, TestDifferentGridNum) {
  // The contingency grid is
  //  1 2
  //  4 3
  size_t n = 10;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    { 1, 2, 3 }, // first cluster
    { 4, 5, 6, 7, 8, 9, 10 }  // second cluster
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    { 1, 7, 8, 9, 10}, // first cluster
    { 2, 3, 4, 5, 6}, // second cluster
};  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_ari(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeARI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_DOUBLE_EQ(-1.0/17, clustering_stats.ari());
}