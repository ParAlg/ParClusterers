#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_

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

#include "google/protobuf/text_format.h"
#include "clusterers/clustering_stats.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph::in_memory {

inline void split(const std::string& s, char delim, std::vector<InMemoryClusterer::NodeId>& elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::stoi(item));
  }
}

template <class T>
inline void set_distribution_stats(std::size_t size, const T& data, DistributionStats* distribution_stats){
  double sum = 0;
  double min = std::numeric_limits<double>::max();
  double max = std::numeric_limits<double>::lowest();
  for (std::size_t i = 0; i < size; i++) {
    sum += data(i);
    if (data(i) < min) min = data(i);
    if (data(i) > max) max = data(i);
  }
  distribution_stats->set_count(size);
  distribution_stats->set_total(sum);
  distribution_stats->set_mean(sum / size);
  distribution_stats->set_minimum(min);
  distribution_stats->set_maximum(max);
}

// return a subgraph that has the vertices in V
// if keep_ids=false, the ids in subgraph do not neccessarily corresponds to the original graph's node id
// the number of nodes in the new subgraph is V.size()
// labels[i] is the cluster id of vertex i
template <class Wgh=float>
inline gbbs::symmetric_graph<gbbs::symmetric_vertex, Wgh> get_subgraph(const GbbsGraph& graph_, const std::vector<InMemoryClusterer::NodeId>& V, const parlay::sequence<gbbs::uintE>& labels, bool keep_ids = false){
    // using Wgh = float;
    using uintE = gbbs::uintE;
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

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_