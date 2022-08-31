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

namespace research_graph::in_memory {

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
  auto dist_im_f = [&](size_t i) {
    return (SP[i] == (std::numeric_limits<Distance>::max())) ? 0 : SP[i];
  };
  return SP;
}

inline absl::Status ComputeDiameter(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  
  parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
      auto G = get_subgraph(graph, clustering[i], cluster_ids); 
      auto distances = parlay::sequence<float>::uninitialized(clustering[i].size());
      parlay::parallel_for(0, clustering[i].size(), [&] (size_t j) {
          auto SP = BellmanFordNoPrint(G, j); //use j because the subgraph is re-indexed
          distances[j] = *(parlay::max_element(SP));            
      });
      auto diameter = *(parlay::max_element(distances));
      clustering_stats->add_diameter(diameter);
  });

  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DIAMETER_H_