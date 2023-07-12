#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <vector>

#include "clusterers/labelprop_clusterer/labelprop-clusterer.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"
#include "clusterers/labelprop_clusterer/labelprop_config.pb.h"
#include "google/protobuf/any.pb.h"

#include "clusterers/gbbs_graph_io.h"
#include "absl/status/statusor.h"

using research_graph::in_memory::LabelPropagationClusterer;
using research_graph::in_memory::InMemoryClusterer;
using research_graph::in_memory::internal::WriteEdgeListAsGraph;
using research_graph::in_memory::LabelPropagationClustererConfig;
using research_graph::in_memory::ClustererConfig;

using testing::UnorderedElementsAre;


TEST(TestLP, TestEdgeGraph) {
  std::unique_ptr<InMemoryClusterer> clusterer;
  clusterer.reset(new LabelPropagationClusterer);
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0, 1, 0.5}};

  auto n_status =  WriteEdgeListAsGraph(clusterer->MutableGraph(), edge_list, 
                                        /*is_symmetric_graph*/true);

  research_graph::in_memory::LabelPropagationClustererConfig labelprop_config;
  labelprop_config.set_max_iteration(200);
  labelprop_config.set_update_threshold(0);
  labelprop_config.set_par_threshold(300);

  ClustererConfig config;
  google::protobuf::Any* any = config.mutable_any_config();
  any->PackFrom(labelprop_config);
  auto result = clusterer->Cluster(config);
  auto clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(1)));


  labelprop_config.set_par_threshold(0);
  any->PackFrom(labelprop_config);
  result = clusterer->Cluster(config);
  clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(1)));
}


TEST(TestLP, TestTwoEdgesGraph) {
  std::unique_ptr<InMemoryClusterer> clusterer;
  clusterer.reset(new LabelPropagationClusterer);
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0, 1}, {0, 2}};

  auto n_status =  WriteEdgeListAsGraph(clusterer->MutableGraph(), edge_list, 
                                        /*is_symmetric_graph*/true);

  research_graph::in_memory::LabelPropagationClustererConfig labelprop_config;
  labelprop_config.set_max_iteration(3);
  labelprop_config.set_update_threshold(0);
  labelprop_config.set_par_threshold(300);

  ClustererConfig config;
  google::protobuf::Any* any = config.mutable_any_config();
  any->PackFrom(labelprop_config);
  auto result = clusterer->Cluster(config);
  auto clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(1, 2)));


  labelprop_config.set_par_threshold(0);
  any->PackFrom(labelprop_config);
  result = clusterer->Cluster(config);
  clustering = *result;

  EXPECT_THAT(clustering, UnorderedElementsAre(UnorderedElementsAre(0), UnorderedElementsAre(1, 2)));
}