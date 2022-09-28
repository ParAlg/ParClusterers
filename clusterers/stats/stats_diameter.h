#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DIAMETER_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DIAMETER_H_

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
#include "google/protobuf/repeated_field.h"
#include "clusterers/clustering_stats.pb.h"
#include "clusterers/stats/stats_utils.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

#include "external/gbbs/benchmarks/GeneralWeightSSSP/BellmanFord/BellmanFord.h"
#include "external/gbbs/benchmarks/Connectivity/SimpleUnionAsync/Connectivity.h"

namespace research_graph::in_memory {

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
  return flags[n];
}

inline void ComputeComponentHelper(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, std::vector<int>& component_vec) {

  if(clustering.size()==1){ // a single cluster, no need to obtain subgraph of each cluster
    auto cc_labels = gbbs::simple_union_find::SimpleUnionAsync(*graph.Graph());
    std::size_t cc = num_cc(cc_labels);
    component_vec[0] = cc;
  }else{
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        auto G = get_subgraph(graph, clustering[i], cluster_ids);
        auto cc_labels = gbbs::simple_union_find::SimpleUnionAsync(G);
        std::size_t cc = num_cc(cc_labels);
        component_vec[i] = cc;
    });
  }
}

// Same as the gbbs BellmanFord algorithm, but no print statement
template <class Graph>
auto BellmanFordNoPrint(Graph& G, gbbs::uintE start) {
  using W = typename Graph::weight_type;
  using Distance =
      typename std::conditional<std::is_same<W, gbbs::empty>::value, gbbs::uintE,
                                W>::type;

  size_t n = G.n;
  auto Visited = parlay::sequence<int>(n, 0);
  auto SP = parlay::sequence<Distance>(n, std::numeric_limits<Distance>::max());
  SP[start] = 0;

  gbbs::vertexSubset Frontier(n, start);
  size_t round = 0;
  while (!Frontier.isEmpty()) {
    // Check for a negative weight cycle
    if (round == n) {
      std::cout << " Found negative weight cycle." << std::endl;
      break;
    }
    auto em_f = gbbs::BF_F<W, Distance>(SP.begin(), Visited.begin());
    auto output =
        gbbs::edgeMap(G, Frontier, em_f, G.m / 10, gbbs::sparse_blocked | gbbs::dense_forward);
    gbbs::vertexMap(output, gbbs::BF_Vertex_F(Visited.begin()));
    Frontier = std::move(output);
    round++;
  }
  return SP;
}

inline absl::Status ComputeDiameter(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  
  const bool compute_num_component = clustering_stats_config.compute_num_component();
  const bool compute_diameter = clustering_stats_config.compute_diameter();

  if ((!compute_num_component) && (!compute_diameter)) {
    return absl::OkStatus();
  }

  std::vector<int> component_vec = std::vector<int>(clustering.size());
  ComputeComponentHelper(graph, clustering, clustering_stats, cluster_ids, component_vec);

  if (compute_num_component) {
    auto component_func = [&](std::size_t i) {
      return component_vec[i];
    };
    set_distribution_stats(component_vec.size(), component_func, clustering_stats->mutable_num_component());
  }
  
  if (!compute_diameter) {
    return absl::OkStatus();
  }
  parlay::sequence<double> diameter_vec = parlay::sequence<double>::uninitialized(clustering.size());

  parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
    if(component_vec[i]==1){ // only compute diameter for single connected component cluster
      auto G = get_subgraph(graph, clustering[i], cluster_ids); 
      auto distances = parlay::sequence<float>::uninitialized(clustering[i].size());
      parlay::parallel_for(0, clustering[i].size(), [&] (size_t j) {
          auto SP = BellmanFordNoPrint(G, j); //use j because the subgraph is re-indexed
          distances[j] = *(parlay::max_element(SP));            
      });
      auto diameter = *(parlay::max_element(distances));
      diameter_vec[i] = diameter;
    }
  });

  auto flags = parlay::delayed_seq<bool>(clustering.size(), [&] (size_t i) {
    return component_vec[i]==1;
  });
  auto diameter_vec_filtered = parlay::pack(diameter_vec, flags);

  auto diameter_func = [&](std::size_t i) {
    return diameter_vec_filtered[i];
  };
  set_distribution_stats(diameter_vec_filtered.size(), diameter_func, clustering_stats->mutable_diameter());

  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DIAMETER_H_