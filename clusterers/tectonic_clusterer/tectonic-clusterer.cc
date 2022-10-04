#include "clusterers/tectonic_clusterer/tectonic-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"
#include "clusterers/tectonic_clusterer/tectonic_config.pb.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<TectonicClusterer::Clustering>
TectonicClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;
  std::size_t m = graph_.Graph()->m;

  TectonicClustererConfig tectonic_config;
  config.any_config().UnpackTo(&tectonic_config);

  double threshold = tectonic_config.threshold();
  const auto ordering_function = tectonic_config.ordering_function();

  parlay::sequence<gbbs::uintE> clusters;
  switch (ordering_function) {
    case TectonicClustererConfig::DEFAULT_DEGREE:
    {
      clusters = gbbs::Triangle_degree_ordering_edge(*(graph_.Graph()), threshold);
      // Now use the triangle_degrees and DG to do a union find
      // Might be useful to have offset too
      break;
    }
    case TectonicClustererConfig::GOODRICH_PSZONA:
    {
      auto ordering_fn = [&](gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>& graph) -> parlay::sequence<gbbs::uintE> {
        return gbbs::goodrichpszona_degen::DegeneracyOrder_intsort(graph, eps);
      };
      clusters = gbbs::Triangle_degeneracy_ordering_edge(*(graph_.Graph()), threshold, ordering_fn);
      break;
    }
    case TectonicClustererConfig::BARENBOIM_ELKIN:
    {
      auto ordering_fn = [&](gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>& graph) -> parlay::sequence<gbbs::uintE> {
        return gbbs::barenboimelkin_degen::DegeneracyOrder(graph);
      };
      clusters = gbbs::Triangle_degeneracy_ordering_edge(*(graph_.Graph()), threshold, ordering_fn);
      break;
    }
    case TectonicClustererConfig::KCORE:
    {
      auto ordering_fn = [&](gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>& graph) -> parlay::sequence<gbbs::uintE> {
        auto dyn_arr = gbbs::DegeneracyOrder(graph);
        auto ret = parlay::sequence<gbbs::uintE>::from_function(
          graph.n, [&](size_t i) { return dyn_arr.A[i]; });
        return ret;
      };
      clusters = gbbs::Triangle_degeneracy_ordering_edge(*(graph_.Graph()), threshold, ordering_fn);
      break;
    }
    default:
      std::cout << "Unkown ordering function" << std::endl; exit(0);
  }

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(clusters);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
