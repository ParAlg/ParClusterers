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
  InMemoryClusterer::Graph* graph, bool float_weighted);

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