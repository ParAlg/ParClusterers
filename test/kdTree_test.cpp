#include "gbbs/bridge.h"
#include "clusterers/hac_euclidean_clusterer/point.h"
#include "clusterers/hac_euclidean_clusterer/kdtree/kdtree.h"
#include "uniform.h"
#include "clusterers/hac_euclidean_clusterer/node.h"
#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include "clusterers/hac_euclidean_clusterer/finder.h"
#include "clusterers/hac_euclidean_clusterer/dist.h"
#include "clusterers/hac_euclidean_clusterer/clusterer.h"

#include "pointIO.h"

#include <vector>

using namespace research_graph::in_memory;
using namespace research_graph::in_memory::internal::HACTree;


template <int dim>
inline void testKdTree(parlay::sequence<point<dim>> &P)
{ 
  using pointT = iPoint<dim>;
  using nodeT = internal::HACTree::node<dim, pointT, nodeInfo>;
  
  auto PP = makeIPoint(P.cut(0,P.size()));
  for (std::size_t i = 0; i < P.size(); ++i){
    EXPECT_EQ(PP[i].pointDist(P[i]),0); //sanity check that the copy is successful
  }
  
  nodeT *tree1 = internal::HACTree::build<dim, pointT , nodeInfo>(PP, true);
  vector<int> pts = vector(P.size(), 0);
  

  std::function<size_t(nodeT *)> checkSum =
      [&](nodeT *node) -> size_t
  {
    if (!node->isLeaf())
    {
      size_t lSize = checkSum(node->L());
      size_t rSize = checkSum(node->R());

      // Check if node sizes are consistent
      EXPECT_EQ(lSize + rSize, node->size());

      // Check if each point is in the bounding box
      for (int i = 0; i < node->size(); ++i)
      {
        auto p = *(node->at(i));
        for (size_t d = 0; d < node->dim; ++d)
        {
          EXPECT_TRUE(p[d] <= node->getMax(d));
          EXPECT_TRUE(p[d] >= node->getMin(d));
        }
      }

      // Check if box sizes are consistent
      for (size_t d = 0; d < node->dim; ++d)
      {
        EXPECT_FLOAT_EQ(std::max(node->L()->getMax(d),
                                 node->R()->getMax(d)),
                        node->getMax(d));
      }

      //check nodeinfo for a default tree
      nodeInfo nInfo = node->getInfo();
      EXPECT_EQ(nInfo.cId, -1);
      EXPECT_EQ(nInfo.ub, numeric_limits<double>::max());
      EXPECT_EQ(nInfo.round, 0);
      EXPECT_EQ(nInfo.idx, -1);
      EXPECT_EQ(nInfo.min_n, 1);
      // EXPECT_THAT(node->getInfo(), ::testing::FieldsAre(-1, numeric_limits<double>::max(), 0, -1, 1));
    }else{
      for (int i = 0; i < node->size(); ++i)
      {
        iPoint<dim> *p = node->getItem(i);
        pts[p->idx()]++;
      }
    }
    return node->size();
  };

  ASSERT_THAT(pts, ::testing::Each(0));
  size_t tree_n = checkSum(tree1);
  
  EXPECT_EQ(tree_n, P.size());
  ASSERT_THAT(pts, ::testing::Each(1)); //each point is stored once

  internal::HACTree::del(tree1);
}

TEST(kdTree, testBuild)
{
  static const int dim = 2;
  auto P = pargeo::uniformInPolyPoints<dim>(100, 0, 1.0);
  testKdTree<dim>(P);
}

TEST(kdTree, small)
{
  auto P = smallData2();
  int n = P.size();
  int chainNum = n;
  int round = 0;
  int *flags = (int*) malloc(sizeof(int)*n);// used in linkterminalnodes

  UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
  // auto P = makeIPoint(P0);

  using objT = iPoint<2>;
  // using nodeT = internal::HACTree::node<2, objT, nodeInfo>;

  using distT = distAverageSq<2>;
  using F = RangeQueryCenterF<2, objT, distT>;
  
  distT *dist = new distT();
  NNFinder<2, distT, F> *finder = new NNFinder<2, distT, F>(n, P.data(), uf, dist, true); //a no cache finder
  TreeChainInfo *info = new TreeChainInfo(n, finder->eps);
  finder->initChain(info);
  vector<int> round1_nn = {1,0,1,2,3,6,5};
  vector<long> round1_rev = {1,0,3,4,-1,6,5};
  for(int i=0;i<n;++i){
    EXPECT_EQ(finder->edges[i].first, i);
    EXPECT_EQ(finder->edges[i].second, round1_nn[i]);
    EXPECT_EQ(info->getNN(i), round1_nn[i]);
    EXPECT_EQ(info->chainRev[i].second, round1_rev[i]);
  }

  link_terminal_nodes<dim, TF>(uf, finder, info, round, flags);
  EXPECT_EQ(uf->find(0), uf->find(1));
  EXPECT_EQ(uf->find(5), uf->find(6));
  vector<int> round1_nn2 = {-1,-1,1,2,3,-1,-1};
  vector<long> round1_rev2 = {-1,-1,3,4,-1,-1,-1};
  for(int i=0;i<n;++i){
    EXPECT_EQ(finder->edges[i].first, i);
    EXPECT_EQ(finder->edges[i].second, round1_nn[i]);
    EXPECT_EQ(info->getNN(i), round1_nn2[i]);
    EXPECT_EQ(info->chainRev[i].second, round1_rev2[i]);
  }
  // chain_find_nn(chainNum, finder, info);

  free(flags);
}

// TEST(kdTree, AllNNlarge)
// {
//   auto P = pargeo::pointIO::readPointsFromFile<point<2>>("2D_Var_10K");
//   int n = P.size();
//   UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
//   // auto P = makeIPoint(P0);

//   using objT = iPoint<2>;
//   // using nodeT = internal::HACTree::node<2, objT, nodeInfo>;

//   using distT = distAverageSq<2>;
//   using F = RangeQueryCenterF<2, objT, distT>;
  
//   distT *dist = new distT();
//   NNFinder<2, distT, F> *finder = new NNFinder<2, distT, F>(n, P.data(), uf, dist, true); //a no cache finder
//   TreeChainInfo *info = new TreeChainInfo(n, finder->eps);
//   finder->initChain(info);
// }

// Demonstrate some basic assertions.
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