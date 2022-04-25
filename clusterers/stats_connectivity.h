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
// labels[i] is the cluster id of vertex i
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

// return the number of edges in a subgraph that has the vertices in V
// labels[i] is the cluster id of vertex i
std::size_t GetSubgraphNumEdges(const GbbsGraph& graph_, const std::vector<InMemoryClusterer::NodeId>& V, const std::vector<int>& labels){
    using uintE = gbbs::uintE;
    // auto G = graph_.Graph();
    std::size_t n = V.size();
    auto offsets = parlay::sequence<size_t>::from_function(
      n, [&](size_t i) { return graph_.Graph()->get_vertex(V[i]).out_degree(); }); //double check, is numbers in V corresponding to the graph nodes?
    auto new_m = parlay::scan_inclusive_inplace(offsets);
    auto flags = parlay::sequence<size_t>(new_m, 0); // flag[i] = true if the edge should be in the subgraph
    // mark true in flags where an edge is in subgraph
    parlay::parallel_for(0, n, [&] (size_t i) {
        uintE vert = V[i];
        std::size_t offset = i==0 ? 0 : offsets[i-1];
        auto map_f = [&] (const auto& u, const auto& v, const auto& wgh, const auto& j) {
            if(labels[u]==labels[v]){
                flags[offset+j] = 1;
            }
        };
        graph_.Graph()->get_vertex(vert).out_neighbors().map_with_index(map_f);
    });

    return parlay::reduce(flags);

}

std::vector<int> GetLabels(const InMemoryClusterer::Clustering& clustering, std::size_t n){
    auto labels = std::vector<int>(n);
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        parlay::parallel_for(0, clustering[i].size(), [&] (size_t j) {
            labels[clustering[i][j]] = i;
        });
    });
    return labels;
}

// copy from gbbs::simple_union_find::num_cc, removed printing statement
template <class Seq>
inline size_t num_cc(Seq& labels) {
  size_t n = labels.size();
  auto flags = parlay::sequence<gbbs::uintE>::from_function(n + 1, [&](size_t i) { return 0; });
  parlay::parallel_for(0, n, [&] (size_t i) {
    if (!flags[labels[i]]) {
      flags[labels[i]] = 1;
    }
  }, gbbs::kDefaultGranularity);
  parlay::scan_inplace(flags);
//   std::cout << "# n_cc = " << flags[n] << "\n";
  return flags[n];
}


std::vector<bool> ClusterConnectivity(const InMemoryClusterer::Clustering& clustering, const GbbsGraph& graph_) {
  std::size_t n = graph_.Graph()->n;
  std::vector<bool> result = std::vector<bool>(clustering.size());

  if(clustering.size()==1){
    auto cc_labels = gbbs::simple_union_find::SimpleUnionAsync(*graph_.Graph());
    std::size_t cc = num_cc(cc_labels);
    result[0] = cc==1;
    return result;
  }

    auto labels = GetLabels(clustering, n);
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        auto G = GetSubgraph(graph_, clustering[i], labels);
        auto cc_labels = gbbs::simple_union_find::SimpleUnionAsync(G);
        std::size_t cc = num_cc(cc_labels);
        result[i] = cc==1;
    });

    // for(bool l:result) std::cout << l << std::endl;

  return result;
}

std::vector<double> ClusterEdgeDensity(const InMemoryClusterer::Clustering& clustering, const GbbsGraph& graph_) {
  std::size_t n = graph_.Graph()->n;
  auto result = std::vector<double>(clustering.size());

  if(clustering.size()==1){
    result[0] = ((double)graph_.Graph()->m) / ((double)n*(n-1));
  }else{
    auto labels = GetLabels(clustering, n);
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        size_t m_subgraph = GetSubgraphNumEdges(graph_, clustering[i], labels);
        double m_total = clustering[i].size()*(clustering[i].size()-1);
        result[i] = ((double)m_subgraph) / ((double)m_total);
    });
  }
//   for(double l:result) std::cout << l << std::endl;
  return result;
}

}  // namespace research_graph::in_memory
