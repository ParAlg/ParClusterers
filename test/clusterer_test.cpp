#include "gbbs/bridge.h"
#include "clusterers/hac_euclidean_clusterer/clusterer.h"
#include "clusterers/hac_euclidean_clusterer/utils/point.h"
#include "clusterers/hac_euclidean_clusterer/utils/dendro.h"
#include "pointIO.h"
#include <gtest/gtest.h>
#include "gmock/gmock.h"

using namespace research_graph::in_memory::internal;
// bazel run //test:clusterer_test
// ground truth came from clusterer/hac_clusterer which uses a distance matrix
// TODO: add large datasets test
string dataset_addr = "/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/";

namespace {
class SimpleTest : public testing::Test {
protected:
  static void SetUpTestSuite() {
    // Avoid reallocating static objects if called in subclasses of FooTest.
    if (P.size() == 0) {
        string filename = dataset_addr + "small/example2.pbbs";
        auto P0 = pargeo::pointIO::readPointsFromFile<HACTree::point<2>>(filename.c_str());
        P = HACTree::makeIPoint<2>(P0);
    }
  }

  static void TearDownTestSuite() {
    P = parlay::sequence<HACTree::iPoint<2>>();
  }

  static parlay::sequence<HACTree::iPoint<2>> P;
};

parlay::sequence<HACTree::iPoint<2>> SimpleTest::P;

TEST_F(SimpleTest, AVGSQ) {
  bool no_cache = true;
  // no cache
  vector<HACTree::dendroLine> dendro = runAVGSQHAC<2>(P, no_cache);
  double checksum = HACTree::getCheckSum(dendro);
  ASSERT_NEAR(60.1333, checksum, 0.0001);
}

TEST_F(SimpleTest, AVG) {
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runAVGHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(16.79327897, checksum, 0.0001);
}

TEST_F(SimpleTest, Complete) {
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runCompleteHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(19.62780549, checksum, 0.0001);
}
TEST_F(SimpleTest, WARD) {
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runWARDHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(20.90836938, checksum, 0.0001);
}

TEST_F(SimpleTest, AVGSQCache) {
  bool no_cache = false;
  // no cache
  vector<HACTree::dendroLine> dendro = runAVGSQHAC<2>(P, no_cache);
  double checksum = HACTree::getCheckSum(dendro);
  ASSERT_NEAR(60.1333, checksum, 0.0001);
}

TEST_F(SimpleTest, AVGCache) {
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runAVGHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(16.79327897, checksum, 0.0001);
}

TEST_F(SimpleTest, CompleteCache) {
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runCompleteHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(19.62780549, checksum, 0.0001);
}
TEST_F(SimpleTest, WARDCache) {
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runWARDHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(20.90836938, checksum, 0.0001);
}

} // namespace

namespace {
class TwoDGaussianOneKTest : public testing::Test {
protected:
  static void SetUpTestSuite() {
    // Avoid reallocating static objects if called in subclasses of FooTest.
    if (P.size() == 0) {
        string filename = dataset_addr + "2D_GaussianDisc_1K.pbbs";
        auto P0 = pargeo::pointIO::readPointsFromFile<HACTree::point<2>>(filename.c_str());
        P = HACTree::makeIPoint<2>(P0);
    }
    check_sum_eps = 0.1;
  }

  static void TearDownTestSuite() {
    P = parlay::sequence<HACTree::iPoint<2>>();
  }

  static parlay::sequence<HACTree::iPoint<2>> P;
  static double check_sum_eps;
};

parlay::sequence<HACTree::iPoint<2>> TwoDGaussianOneKTest::P;
double TwoDGaussianOneKTest::check_sum_eps;

TEST_F(TwoDGaussianOneKTest, AVGSQ) {
  bool no_cache = true;
  // no cache
  vector<HACTree::dendroLine> dendro = runAVGSQHAC<2>(P, no_cache);
  double checksum = HACTree::getCheckSum(dendro);
  ASSERT_NEAR(5984206.371, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, AVG) {
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runAVGHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(31650.13896, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, Complete) { //wrong answer
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runCompleteHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(48308.50497, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, WARD) {
    bool no_cache = true;
    vector<HACTree::dendroLine> dendro = runWARDHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(98231.0161, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, AVGSQCache) {
  bool no_cache = false;
  // no cache
  vector<HACTree::dendroLine> dendro = runAVGSQHAC<2>(P, no_cache);
  double checksum = HACTree::getCheckSum(dendro);
  ASSERT_NEAR(5984206.371, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, AVGCache) {
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runAVGHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(31650.13896, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, CompleteCache) { //segfault
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runCompleteHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(48308.50497, checksum, check_sum_eps);
}

TEST_F(TwoDGaussianOneKTest, WARDCache) {
    bool no_cache = false;
    vector<HACTree::dendroLine> dendro = runWARDHAC<2>(P, no_cache);
    double checksum = HACTree::getCheckSum(dendro);
    ASSERT_NEAR(98231.0161, checksum, check_sum_eps);
}

} // namespace

// // Demonstrate some basic assertions.
// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}