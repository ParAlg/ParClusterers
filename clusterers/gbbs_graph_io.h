#include <chrono>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"

#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

double DoubleFromWeight(gbbs::empty weight) { return static_cast<double>(1); }
double DoubleFromWeight(double weight) { return weight; }

float FloatFromWeight(float weight) { return weight; }
float FloatFromWeight(gbbs::empty weight) { return static_cast<float>(1); }

// TODO(jeshi): This always assumes a symmetric graph
absl::StatusOr<std::size_t> ReadGbbsGraphFormat(const std::string& input_file,
  InMemoryClusterer::Graph* graph, bool float_weighted);

template <class Graph>
absl::Status GbbsGraphToInMemoryClustererGraph(InMemoryClusterer::Graph* graph,
                                               Graph& gbbs_graph) {
  using weight_type = typename Graph::weight_type;
  for (std::size_t i = 0; i < gbbs_graph.n; i++) {
    auto vertex = gbbs_graph.get_vertex(i);
    std::vector<std::pair<gbbs::uintE, double>> outgoing_edges(
        vertex.out_degree());
    gbbs::uintE index = 0;
    auto add_outgoing_edge = [&](gbbs::uintE, const gbbs::uintE neighbor,
                                 weight_type wgh) {
      outgoing_edges[index] = std::make_pair(static_cast<gbbs::uintE>(neighbor),
                                             DoubleFromWeight(wgh));
      index++;
    };
    vertex.out_neighbors().map(add_outgoing_edge, false);
    InMemoryClusterer::Graph::AdjacencyList adjacency_list{
        static_cast<InMemoryClusterer::NodeId>(i), 1,
        std::move(outgoing_edges)};
    RETURN_IF_ERROR(graph->Import(adjacency_list));
  }
  RETURN_IF_ERROR(graph->FinishImport());
  return absl::OkStatus();
}

template <typename Weight>
absl::StatusOr<std::size_t> WriteEdgeListAsGraph(
    InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<Weight>>& edge_list,
    bool is_symmetric_graph) {
  std::size_t n = 0;
  if (is_symmetric_graph) {
    auto gbbs_graph{gbbs::gbbs_io::edge_list_to_symmetric_graph(edge_list)};
    n = gbbs_graph.n;
    auto status = GbbsGraphToInMemoryClustererGraph<
        gbbs::symmetric_graph<gbbs::symmetric_vertex, Weight>>(graph,
                                                               gbbs_graph);
    RETURN_IF_ERROR(status);
  } else {
    auto gbbs_graph{gbbs::gbbs_io::edge_list_to_asymmetric_graph(edge_list)};
    n = gbbs_graph.n;
    auto status = GbbsGraphToInMemoryClustererGraph<
        gbbs::asymmetric_graph<gbbs::asymmetric_vertex, Weight>>(graph,
                                                                 gbbs_graph);
    RETURN_IF_ERROR(status);
  }
  return n;
}

absl::StatusOr<std::size_t> ReadEdgeListGraphFormat(const std::string& input_file,
  InMemoryClusterer::Graph* graph, bool float_weighted, bool is_symmetric_graph);

template <class Graph>
gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float> CopyGraph(
    Graph& graph) {
  using vertex_data = gbbs::symmetric_vertex<float>;
  using edge_type = std::tuple<gbbs::uintE, float>;
  auto vd = gbbs::new_array_no_init<vertex_data>(graph.n);
  auto ed = gbbs::new_array_no_init<edge_type>(graph.m);
  parlay::parallel_for(0, graph.n, [&](size_t i) {
    vd[i].degree = graph.v_data[i].degree;
    vd[i].neighbors = ed + graph.v_data[i].offset;
    vd[i].id = i;
  });
  parlay::parallel_for(0, graph.m, [&](size_t i) {
    ed[i] = std::make_tuple(
        std::get<0>(graph.e0[i]),
        FloatFromWeight(std::get<1>(graph.e0[i])));  // graph.e0[i];
  });
  size_t n = graph.n;
  size_t m = graph.m;
  auto g = gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>(
      graph.n, graph.m, vd, [vd, ed, n, m]() {
        gbbs::free_array(vd, n);
        gbbs::free_array(ed, m);
      });
  return g;
}

}  // namespace in_memory
}  // namespace research_graph