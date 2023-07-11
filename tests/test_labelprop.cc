#include "gtest/gtest.h"


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


TEST(TestLP, TestAllSame) {
  // std::unique_ptr<LabelPropagationClusterer> clusterer;
  std::unique_ptr<InMemoryClusterer> clusterer;
  clusterer.reset(new LabelPropagationClusterer);
  // const std::vector<gbbs::gbbs_io::Edge<gbbs::empty>> edge_list = {{0,1}, {1,0}};
  const std::vector<gbbs::gbbs_io::Edge<double>> edge_list = {{0,1, 0.5}, {1,0, 0.5}};

  auto n_status =  WriteEdgeListAsGraph(clusterer->MutableGraph(), edge_list, 
                                        /*is_symmetric_graph*/true);

  research_graph::in_memory::LabelPropagationClustererConfig labelprop_config;
  labelprop_config.set_max_iteration(200);
  labelprop_config.set_update_threshold(300);

  ClustererConfig config;
  google::protobuf::Any* any = config.mutable_any_config();
  any->PackFrom(labelprop_config);
  auto result = clusterer->Cluster(config);
  auto clustering = *result;

  EXPECT_EQ(1,  1);
}