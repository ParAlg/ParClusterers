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
      // TODO(jeshi): Ignoring error
      graph->Import(adjacency_list);
    });
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
      // TODO(jeshi): Ignoring error
      graph->Import(adjacency_list);
    });
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
    ASSIGN_OR_RETURN(n, WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  } else {
    const auto edge_list{
        gbbs::gbbs_io::read_unweighted_edge_list(input_file.c_str())};
    ASSIGN_OR_RETURN(n, WriteEdgeListAsGraph(graph, edge_list, is_symmetric_graph));
  }
  return n;
}

}  // namespace in_memory
}  // namespace research_graph