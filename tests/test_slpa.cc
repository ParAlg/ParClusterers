#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <vector>
#include <set>

#include "clusterers/slpa_clusterer/slpa-clusterer.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"
#include "clusterers/slpa_clusterer/slpa_config.pb.h"
#include "google/protobuf/any.pb.h"

#include "clusterers/gbbs_graph_io.h"
#include "absl/status/statusor.h"

using research_graph::in_memory::SLPAClusterer;
using research_graph::in_memory::InMemoryClusterer;
using research_graph::in_memory::internal::WriteEdgeListAsGraph;
using research_graph::in_memory::SLPAClustererConfig;
using research_graph::in_memory::ClustererConfig;

using testing::UnorderedElementsAre;


TEST(TestSLPA, TestEdgeGraph) {
  std::unique_ptr<InMemoryClusterer> clusterer;
  clusterer.reset(new SLPAClusterer);
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0, 1, 0.5}};

  auto n_status =  WriteEdgeListAsGraph(clusterer->MutableGraph(), edge_list, 
                                        /*is_symmetric_graph*/true);

  research_graph::in_memory::SLPAClustererConfig slpa_config;
  slpa_config.set_max_iteration(200);
  slpa_config.set_par_threshold(300);
  slpa_config.set_remove_nested(true);


  ClustererConfig config;
  google::protobuf::Any* any = config.mutable_any_config();
  any->PackFrom(slpa_config);
  auto result = clusterer->Cluster(config);
  auto clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0, 1)));


  slpa_config.set_par_threshold(0);
  any->PackFrom(slpa_config);
  result = clusterer->Cluster(config);
  clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0, 1)));
}


TEST(TestPostprocessing, TestThreshold) {
  SLPAClusterer clusterer;

  parlay::sequence<std::map<gbbs::uintE, size_t>> memory(2);
  memory[0][0] = 10;
  memory[0][1] = 1;
  memory[0][2] = 11;

  memory[1][1] = 22;

  // cluster 0: 0
  // cluster 1: 1
  // cluster 2: 0

  auto clustering = clusterer.postprocessing(memory, /*remove_nested*/ false, 22, 0.2);
  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(0), UnorderedElementsAre(1)));


  clustering = clusterer.postprocessing(memory, /*remove_nested*/ true, 22, 0.2);
  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(1)));

}


TEST(FindMaximalSetsTest, EmptySet) {
    SLPAClusterer clusterer;
    std::vector<std::set<gbbs::uintE>> sets = {};
    auto maximalSets = clusterer.findMaximalSets(sets);
    EXPECT_TRUE(maximalSets.empty());
}

TEST(FindMaximalSetsTest, SingleSet) {
    SLPAClusterer clusterer;

    std::vector<std::set<gbbs::uintE>> sets = { {1, 2, 3} };
    std::set<std::set<gbbs::uintE>> expected = { {1, 2, 3} };
    auto maximalSets = clusterer.findMaximalSets(sets);
    EXPECT_EQ(maximalSets, expected);
}

TEST(FindMaximalSetsTest, NestedSets) {
    SLPAClusterer clusterer;

    std::vector<std::set<gbbs::uintE>> sets = { {1, 2, 3}, {1, 2}, {2, 3} };
    std::set<std::set<gbbs::uintE>> expected = { {1, 2, 3} };
    auto maximalSets = clusterer.findMaximalSets(sets);
    EXPECT_EQ(maximalSets, expected);
}

TEST(FindMaximalSetsTest, MultipleMaximalSets) {
    SLPAClusterer clusterer;

    std::vector<std::set<gbbs::uintE>> sets = { {1, 2, 3}, {4, 5, 6}, {1, 2}, {4, 5} };
    auto maximalSets = clusterer.findMaximalSets(sets);
    EXPECT_THAT(maximalSets, UnorderedElementsAre(UnorderedElementsAre(1,2,3), UnorderedElementsAre(4,5,6)));
}

TEST(FindMaximalSetsTest, AllSameSets) {
    SLPAClusterer clusterer;

    std::vector<std::set<gbbs::uintE>> sets = { {1, 2, 3}, {1, 2, 3}, {1, 2, 3} };
    std::set<std::set<gbbs::uintE>> expected = { {1, 2, 3} };
    auto maximalSets = clusterer.findMaximalSets(sets);
    EXPECT_EQ(maximalSets, expected);
}


