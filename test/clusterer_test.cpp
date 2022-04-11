#include "gbbs/bridge.h"
#include "clusterers/hac_euclidean_clusterer/clusterer.h"
#include "clusterers/hac_euclidean_clusterer/utils/point.h"
#include "clusterers/hac_euclidean_clusterer/utils/dendro.h"
#include "pointIO.h"
#include <gtest/gtest.h>
#include "gmock/gmock.h"

using namespace research_graph::in_memory::internal;
// bazel run //test:clusterer_test
string dataset_addr = "/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/";

TEST(AVGSQTest, simple) {
  string filename = dataset_addr + "small/example2.pbbs";
  auto P0 = pargeo::pointIO::readPointsFromFile<HACTree::point<2>>(filename.c_str());
  auto P = HACTree::makeIPoint<2>(P0);
  bool no_cache = true;
    // no cache
  vector<HACTree::dendroLine> dendro = runAVGSQHAC<2>(P, no_cache);
  double checksum = HACTree::getCheckSum(dendro);

  ASSERT_NEAR(60.1333, checksum, 0.0001);
}



// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}