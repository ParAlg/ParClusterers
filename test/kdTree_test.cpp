#include "gbbs/bridge.h"
#include "point.h"
#include "kdtree/kdtree.h"
#include "uniform.h"
#include "node.h"
#include <gtest/gtest.h>

#include <vector>

using namespace research_graph::in_memory;
using namespace research_graph::in_memory::internal::HACTree;

parlay::sequence<point<2>> smallData()
{
  parlay::sequence<point<2>> P(7);
  P[0][0] = 0.5;  
  P[0][1] = 3.5;
  P[1][0] = 0.5;
  P[1][1] = 2.5;
  P[2][0] = 1;
  P[2][1] = 3;
  P[3][0] = 1.5;
  P[3][1] = 2.5;
  P[4][0] = 3.5;
  P[4][1] = 0.5;
  P[5][0] = 3.5;
  P[5][1] = 0.5;
  P[6][0] = 2.5;
  P[6][1] = 0.5;

  return P;
}

// template <int dim>
// inline void testKdTree(parlay::sequence<point<dim>> &P)
// {
//   using nodeT = internal::HACTree::node<dim, point<dim>, nodeInfo>;

//   nodeT *tree1 = internal::HACTree::build<dim, point<dim>, nodeInfo>(P, true);

//   std::function<size_t(nodeT *)> checkSum =
//       [&](nodeT *node) -> size_t
//   {
//     if (!node->isLeaf())
//     {
//       size_t lSize = checkSum(node->L());
//       size_t rSize = checkSum(node->R());

//       // Check if node sizes are consistent
//       EXPECT_EQ(lSize + rSize, node->size());

//       // Check if each point is in the bounding box
//       for (size_t i = 0; i < node->size(); ++i)
//       {
//         auto p = *(node->at(i));
//         for (size_t d = 0; d < node->dim; ++d)
//         {
//           EXPECT_TRUE(p[d] <= node->getMax(d));
//           EXPECT_TRUE(p[d] >= node->getMin(d));
//         }
//       }

//       // Check if box sizes are consistent
//       for (size_t d = 0; d < node->dim; ++d)
//       {
//         EXPECT_FLOAT_EQ(std::max(node->L()->getMax(d),
//                                  node->R()->getMax(d)),
//                         node->getMax(d));
//       }
//     }
//     return node->size();
//   };

//   checkSum(tree1);

//   internal::HACTree::del(tree1);
// }

TEST(kdTree_structure, testSerial2d)
{
  static const int dim = 2;
  auto P = pargeo::uniformInPolyPoints<dim>(100, 0, 1.0);
  // testKdTree<dim>(P);
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