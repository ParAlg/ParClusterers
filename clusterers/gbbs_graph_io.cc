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

#include "in_memory/clustering/in_memory_clusterer.h"

namespace research_graph {
namespace in_memory {

namespace internal {

double DoubleFromWeight(gbbs::empty weight) { return static_cast<double>(1); }
double DoubleFromWeight(double weight) { return weight; }

float FloatFromWeight(float weight) { return weight; }
float FloatFromWeight(gbbs::empty weight) { return static_cast<float>(1); }

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

template <class Graph>
absl::Status GbbsGraphToInMemoryClustererGraph(graph_mining::in_memory::InMemoryClusterer::Graph* graph,
                                               Graph& gbbs_graph) {
  using weight_type = typename Graph::weight_type;
  using NodeId = graph_mining::in_memory::InMemoryClusterer::NodeId;
  using AdjacencyList = graph_mining::in_memory::InMemoryClusterer::Graph::AdjacencyList;
  for (std::size_t i = 0; i < gbbs_graph.n; i++) {
    auto vertex = gbbs_graph.get_vertex(i);
    std::vector<std::pair<NodeId, double>> outgoing_edges(
        vertex.out_degree());
    NodeId index = 0;
    auto add_outgoing_edge = [&](gbbs::uintE, const gbbs::uintE neighbor,
                                 weight_type wgh) {
      outgoing_edges[index] = std::make_pair(static_cast<NodeId>(neighbor),
                                             DoubleFromWeight(wgh));
      index++;
    };
    vertex.out_neighbors().map(add_outgoing_edge, false);
    AdjacencyList adjacency_list{
        static_cast<NodeId>(i), 1,
        std::move(outgoing_edges), std::nullopt};
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

template <typename Weight>
absl::StatusOr<std::size_t> WriteEdgeListAsGraph(
     graph_mining::in_memory::InMemoryClusterer::Graph* graph,
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

// Explicit instantiation of the template
template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<double>(
    InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<double>>& edge_list,
    bool is_symmetric_graph);

template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<gbbs::empty>(
    InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<gbbs::empty>>& edge_list,
    bool is_symmetric_graph);

template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<int>(
    InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<int>>& edge_list,
    bool is_symmetric_graph);

template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<double>(
    graph_mining::in_memory::InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<double>>& edge_list,
    bool is_symmetric_graph);

template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<gbbs::empty>(
    graph_mining::in_memory::InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<gbbs::empty>>& edge_list,
    bool is_symmetric_graph);

template absl::StatusOr<std::size_t> WriteEdgeListAsGraph<int>(
    graph_mining::in_memory::InMemoryClusterer::Graph* graph,
    const std::vector<gbbs::gbbs_io::Edge<int>>& edge_list,
    bool is_symmetric_graph);

} // namespace internal

// TODO(jeshi): This always assumes a symmetric graph
absl::StatusOr<std::size_t> ReadGbbsGraphFormat(const std::string& input_file,
  InMemoryClusterer::Graph* graph, bool float_weighted) {
  std::size_t n = 0;
  if (float_weighted){
    std::size_t m;
    gbbs::uintT* offsets;
    std::tuple<gbbs::uintE, float>* edges;
    std::tie(n, m, offsets, edges) =
      gbbs::gbbs_io::internal::parse_weighted_graph<float>(input_file.c_str(),
                                                           false, false);
    RETURN_IF_ERROR(graph->PrepareImport(n));
    parlay::parallel_for(0, n, [&](std::size_t i){
      std::size_t degree = offsets[i+1] - offsets[i];
      std::vector<std::pair<gbbs::uintE, double>> outgoing_edges(degree);
      parlay::parallel_for(0, degree, [&](std::size_t j){
        outgoing_edges[j] =
          std::make_pair(std::get<0>(edges[offsets[i] + j]),
                         static_cast<double>(std::get<1>(edges[offsets[i] + j])));
      });
      InMemoryClusterer::Graph::AdjacencyList adjacency_list{
        static_cast<InMemoryClusterer::NodeId>(i), 1, std::move(outgoing_edges)};
      (void)graph->Import(adjacency_list);
    });
    gbbs::free_array(offsets, n + 1);
    gbbs::free_array(edges, m);
  } else {
    std::size_t m;
    gbbs::uintT* offsets;
    gbbs::uintE* edges;
    std::tie(n, m, offsets, edges) =
      gbbs::gbbs_io::parse_unweighted_graph(input_file.c_str(),
                                                      false, false);
    RETURN_IF_ERROR(graph->PrepareImport(n));
    parlay::parallel_for(0, n, [&](std::size_t i){
      std::size_t degree = offsets[i+1] - offsets[i];
      std::vector<std::pair<gbbs::uintE, double>> outgoing_edges(degree);
      parlay::parallel_for(0, degree, [&](std::size_t j){
        outgoing_edges[j] = std::make_pair(edges[offsets[i] + j], 1);
      });
      InMemoryClusterer::Graph::AdjacencyList adjacency_list{
        static_cast<InMemoryClusterer::NodeId>(i), 1, std::move(outgoing_edges)};
      (void)graph->Import(adjacency_list);
    });
    gbbs::free_array(offsets, n + 1);
    gbbs::free_array(edges, m);
  }
  RETURN_IF_ERROR(graph->FinishImport());
  return n; 
}

absl::StatusOr<std::size_t> ReadEdgeListGraphFormat(const std::string& input_file,
  InMemoryClusterer::Graph* graph, bool float_weighted, bool is_symmetric_graph) {
  std::size_t n = 0;
  if (float_weighted) {
    const auto edge_list{
        gbbs::gbbs_io::read_weighted_edge_list<float>(input_file.c_str())};
    ASSIGN_OR_RETURN(n, internal::WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  } else {
    const auto edge_list{
        gbbs::gbbs_io::read_unweighted_edge_list(input_file.c_str())};
    ASSIGN_OR_RETURN(n, internal::WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  }
  return n;
}


absl::StatusOr<std::size_t> ReadGbbsGraphFormat(const std::string& input_file,
  graph_mining::in_memory::InMemoryClusterer::Graph* graph, bool float_weighted) {
  using NodeId = graph_mining::in_memory::InMemoryClusterer::NodeId;
  using AdjacencyList = graph_mining::in_memory::InMemoryClusterer::Graph::AdjacencyList;
  std::size_t n = 0;
  if (float_weighted){
    std::size_t m;
    gbbs::uintT* offsets;
    std::tuple<gbbs::uintE, float>* edges;
    std::tie(n, m, offsets, edges) =
      gbbs::gbbs_io::internal::parse_weighted_graph<float>(input_file.c_str(),
                                                           false, false);
    RETURN_IF_ERROR(graph->PrepareImport(n));
    parlay::parallel_for(0, n, [&](std::size_t i){
      std::size_t degree = offsets[i+1] - offsets[i];
      std::vector<std::pair<NodeId, double>> outgoing_edges(degree);
      parlay::parallel_for(0, degree, [&](std::size_t j){
        outgoing_edges[j] =
          std::make_pair(static_cast<NodeId>(std::get<0>(edges[offsets[i] + j])),
                         static_cast<double>(std::get<1>(edges[offsets[i] + j])));
      });
      AdjacencyList adjacency_list{
        static_cast<NodeId>(i), 1, std::move(outgoing_edges), std::nullopt};
     (void)graph->Import(adjacency_list);
    });
    gbbs::free_array(offsets, n + 1);
    gbbs::free_array(edges, m);
  } else {
    std::size_t m;
    gbbs::uintT* offsets;
    gbbs::uintE* edges;
    std::tie(n, m, offsets, edges) =
      gbbs::gbbs_io::parse_unweighted_graph(input_file.c_str(),
                                                      false, false);
    RETURN_IF_ERROR(graph->PrepareImport(n));
    parlay::parallel_for(0, n, [&](std::size_t i){
      std::size_t degree = offsets[i+1] - offsets[i];
      std::vector<std::pair<NodeId, double>> outgoing_edges(degree);
      parlay::parallel_for(0, degree, [&](std::size_t j){
        outgoing_edges[j] = std::make_pair(static_cast<NodeId>(edges[offsets[i] + j]), 1);
      });
      AdjacencyList adjacency_list{
        static_cast<NodeId>(i), 1, std::move(outgoing_edges), std::nullopt};
      (void)graph->Import(adjacency_list);
    });
    gbbs::free_array(offsets, n + 1);
    gbbs::free_array(edges, m);
  }
  RETURN_IF_ERROR(graph->FinishImport());
  return n; 
}

absl::StatusOr<std::size_t> ReadEdgeListGraphFormat(const std::string& input_file,
  graph_mining::in_memory::InMemoryClusterer::Graph* graph, bool float_weighted, bool is_symmetric_graph) {
  std::size_t n = 0;
  if (float_weighted) {
    const auto edge_list{
        gbbs::gbbs_io::read_weighted_edge_list<float>(input_file.c_str())};
    ASSIGN_OR_RETURN(n, internal::WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  } else {
    const auto edge_list{
        gbbs::gbbs_io::read_unweighted_edge_list(input_file.c_str())};
    ASSIGN_OR_RETURN(n, internal::WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  }
  return n;
}

}  // namespace in_memory
}  // namespace research_graph