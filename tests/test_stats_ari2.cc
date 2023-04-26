#include "gtest/gtest.h"

#include <vector>

#include "clusterers/stats/stats_ari_2.h"

using research_graph::in_memory::ComputeARI;
using uintE = unsigned int;
TEST(TestARI, TestAllSame) {
  size_t n = 9;
  std::vector<std::vector<uintE>> clustering = {
    { 1, 2, 3 }, // first cluster
    { 4, 5, 6 }, // second cluster
    { 7, 8, 9 }  // third cluster
};
  std::vector<std::vector<uintE>> communities  = {
    { 3, 2, 1 }, // first cluster
    { 4, 5, 6 }, // second cluster
    { 9, 8, 7 }  // third cluster
};
  
  auto ari = ComputeARI(n, clustering, communities);
  EXPECT_EQ(1, ari);
}