#pragma once
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "external/gbbs/benchmarks/Connectivity/SimpleUnionAsync/Connectivity.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"
#include "external/gbbs/gbbs/graph.h"

namespace research_graph::in_memory {


// return a subgraph that has the vertices in V
// the ids in subgraph do not neccessarily corresponds to the original graph's node id
// the number of nodes in the new subgraph = V.size()
gbbs::symmetric_graph<gbbs::symmetric_vertex, float> GetSubgraph(const GbbsGraph& graph_, const std::vector<InMemoryClusterer::NodeId>& V, const std::vector<int>& labels, bool keep_ids = false){
    using Wgh = float;
    using uintE = gbbs::uintE;
    // auto G = graph_.Graph();
    std::size_t n = V.size();
    auto offsets = parlay::sequence<size_t>::from_function(
      n, [&](size_t i) { return graph_.Graph()->get_vertex(V[i]).out_degree(); }); //double check, is numbers in V corresponding to the graph nodes?
    auto new_m = parlay::scan_inclusive_inplace(offsets);
    auto edges = parlay::sequence<std::tuple<uintE, uintE, Wgh>>::uninitialized(new_m);
    auto flags = parlay::sequence<bool>(new_m, false); // flag[i] = true if the edge should be in the subgraph

    auto map =  parlay::sequence<InMemoryClusterer::NodeId>::uninitialized(graph_.Graph()->n); // map from original graph id to new id in subgraph
    if(keep_ids){
    parlay::parallel_for(0, n, [&] (size_t i) {
        map[V[i]] = V[i];
    });
    }else{
    parlay::parallel_for(0, n, [&] (size_t i) {
        map[V[i]] = i;
    });
    }


    // copy edges in the subgraph to edges, and mark true in flags
    parlay::parallel_for(0, n, [&] (size_t i) {
        uintE vert = V[i];
        std::size_t offset = i==0 ? 0 : offsets[i-1];
        auto map_f = [&] (const auto& u, const auto& v, const auto& wgh, const auto& j) {
            if(labels[u]==labels[v]){
                flags[offset+j] = true;
                edges[offset+j] = std::tuple<uintE, uintE, Wgh>(map[u],map[v],wgh);
            }
        };
        graph_.Graph()->get_vertex(vert).out_neighbors().map_with_index(map_f);
    });

    auto subgraph_edges = parlay::pack(edges, flags);

    if(keep_ids){
        return gbbs::sym_graph_from_edges(subgraph_edges, graph_.Graph()->n);
    }else{
        return gbbs::sym_graph_from_edges(subgraph_edges, n);
    }

}

std::vector<bool> ClusterConnectivity(const InMemoryClusterer::Clustering& clustering, const GbbsGraph& graph_) {
//   std::size_t n = graph_.Graph()->n;
  std::vector<bool> result = std::vector<bool>(clustering.size());
  auto labels = gbbs::simple_union_find::SimpleUnionAsync(*graph_.Graph());
  std::size_t cc = gbbs::simple_union_find::num_cc(labels);
  std::cout << cc << std::endl;

  // can also loop over edges on-the-fly and use unionfind

//   auto clusters = parlay::sequence<gbbs::uintE>::from_function(n, [&] (size_t i) { return i; });
//   parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
//     auto map_f = [&] (const auto& u, const auto& v, const auto& wgh) {
//       gbbs::simple_union_find::unite_impl(u, v, clusters);
//     };
//     graph_.Graph()->get_vertex(i).out_neighbors().map(map_f);
//   });

//   parlay::parallel_for(0, n, [&] (size_t i) {
//     gbbs::simple_union_find::find_compress(i, clusters);
//   });

  return result;
}

}  // namespace research_graph::in_memory
