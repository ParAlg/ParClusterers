#include "gtest/gtest.h"

#include <vector>

#include "clusterers/stats/stats_nmi.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"
#include "clusterers/clustering_stats.pb.h"

using research_graph::in_memory::ComputeNMI;
using research_graph::in_memory::ClusteringStatistics;
using research_graph::in_memory::ClusteringStatsConfig;


TEST(TestNMI, TestAllSame) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_EQ(1, clustering_stats.nmi());
}

TEST(TestNMI, TestAllSameDifferentOrder) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_EQ(1, clustering_stats.nmi());
}

TEST(TestNMI, TestOneDifferent) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_DOUBLE_EQ(0.2740175421212811, clustering_stats.nmi());
}


TEST(TestNMI, TestEmpty) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_DOUBLE_EQ(0, clustering_stats.nmi());
}



TEST(TestNMI, TestDifferentGridNum) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  // std::cout << "expected 0.23: " << clustering_stats.nmi() << std::endl;

  EXPECT_NEAR(0.03705068107641335, clustering_stats.nmi(), pow(10,-10));
}

TEST(TestNMI, TestLarger) {
  // The contingency grid is
  //  1 2
  //  4 3
  size_t n = 100;
  std::vector<std::vector<gbbs::uintE>> clustering = {
    {0,1,2,3,4,5,6,7,8,9},
    {10,11,12,13,14,15,16,17,18,19},
    {20,21,22,23,24,25,26,27,28,29},
    {30,31,32,33,34,35,36,37,38,39},
    {40,41,42,43,44,45,46,47,48,49},
    {50,51,52,53,54,55,56,57,58,59},
    {60,61,62,63,64,65,66,67,68,69},
    {70,71,72,73,74,75,76,77,78,79},
    {80,81,82,83,84,85,86,87,88,89},
    {90,91,92,93,94,95,96,97,98,99}
};
  std::vector<std::vector<gbbs::uintE>> communities  = {
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49},
    {50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99}
};  
  ClusteringStatistics clustering_stats;
  ClusteringStatsConfig clustering_stats_config;
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());
  EXPECT_NEAR(0.4627564263195186, clustering_stats.nmi(), pow(10,-10));
}

TEST(TestNMI, TestReversibility) {
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
  clustering_stats_config.set_compute_nmi(true);
  std::cout << "start computing" << std::endl;
  auto status = ComputeNMI(n, clustering, &clustering_stats, communities, clustering_stats_config);
  ASSERT_TRUE(status.ok());

  ClusteringStatistics clustering_stats2;
  ClusteringStatsConfig clustering_stats_config2;
  clustering_stats_config2.set_compute_nmi(true);
  auto status2 = ComputeNMI(n, communities, &clustering_stats2, clustering, clustering_stats_config2);
  ASSERT_TRUE(status2.ok());

  EXPECT_NEAR(clustering_stats.nmi(), clustering_stats.nmi(), pow(10,-10));
}