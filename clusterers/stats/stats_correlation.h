#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_CORRELATION_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_CORRELATION_H_

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

namespace research_graph::in_memory {

inline absl::Status ComputeCorrelationObjective(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const google::protobuf::RepeatedField<double>& edge_weight_offset = clustering_stats_config.correlation_edge_weight_offsets();
  const google::protobuf::RepeatedField<double>& resolution = clustering_stats_config.correlation_resolutions();
  size_t n = graph.Graph()->n;
  assert(edge_weight_offset.size() == resolution.size());
  parlay::sequence<double> shifted_edge_weight(n);

  for(size_t k = 0; k < edge_weight_offset.size(); k++){
    // Compute cluster statistics contributions of each vertex
    parlay::parallel_for(0, n, [&](std::size_t i) {
      gbbs::uintE cluster_id_i = cluster_ids[i];
      auto add_m = parlay::addm<double>();

      auto intra_cluster_sum_map_f = [&](gbbs::uintE u, gbbs::uintE v,
                                         float weight) -> double {
        // Since the graph is undirected, each undirected edge is represented by
        // two directed edges, with the exception of self-loops, which are
        // represented as one edge. Hence, the weight of each intra-cluster edge
        // must be divided by 2, unless it's a self-loop.
        if (u == v)
          return weight;
        // This assumes that the graph is undirected, and self-loops are counted
        // as half of the weight.
        if (cluster_id_i == cluster_ids[v])
          return (weight - edge_weight_offset[k]) / 2;
        return 0;
      };
      shifted_edge_weight[i] = graph.Graph()->get_vertex(i).out_neighbors().reduce(
          intra_cluster_sum_map_f, add_m);
    });
    double objective = parlay::reduce(shifted_edge_weight);
  
    auto resolution_seq = parlay::delayed_seq<double>(n, [&](std::size_t i) {
      auto cluster_weight = clustering[cluster_ids[i]].size(); // cluster_weight
      const double node_weight = 1.0;
      return node_weight * (cluster_weight - node_weight);
    });
    objective -= resolution[k] * parlay::reduce(resolution_seq) / 2;
  
    clustering_stats->add_correlation_objective(objective);
  }

  return absl::OkStatus();
}

inline absl::Status ComputeModularityObjective(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids,
  const ClusteringStatsConfig& clustering_stats_config){
  size_t n = graph.Graph()->n;
  const google::protobuf::RepeatedField<double>& resolution = clustering_stats_config.modularity_resolutions();

  std::vector<double> modularities(resolution.size());
  parlay::parallel_for(0, resolution.size(), [&](std::size_t k){
    parlay::sequence<double> edge_weights(n, 0);  // Initialized with zeros
    parlay::sequence<double> modularities_tmp(n, 0);

    parlay::parallel_for(0, n, [&](std::size_t i) {
      auto vtx = graph.Graph()->get_vertex(i);
      double local_edge_weight = 0;
      double local_modularity = 0;
      auto map_out = [&](gbbs::uintE u, gbbs::uintE nbhr, float w) {
          local_edge_weight++;
          if (cluster_ids[i] == cluster_ids[nbhr]) {
              local_modularity++;
          }
      };
      vtx.out_neighbors().map(map_out, false);
      edge_weights[i] = local_edge_weight;
      modularities_tmp[i] = local_modularity;
    });

    // Accumulate the results using parallel reduction
    const double total_edge_weight = parlay::reduce(edge_weights);
    double modularity = parlay::reduce(modularities_tmp);

    //modularity = modularity / 2; // avoid double counting

    auto modularity_deltas = parlay::delayed_seq<double>(clustering.size(), [&](std::size_t i){
      auto degrees = parlay::delayed_seq<double>(clustering[i].size(), [&](std::size_t j){
        auto vtx_id = clustering[i][j];
        auto vtx = graph.Graph()->get_vertex(vtx_id);
        return vtx.out_degree();
      });
      double degree = parlay::reduce(degrees);
      return (resolution[k] * degree * degree) / (total_edge_weight);
    });
    double modularity_delta = parlay::reduce(modularity_deltas);

    modularity -= modularity_delta;

    modularity = modularity / (total_edge_weight);
    modularities[k] = modularity;
  }); 

  for (auto modularity: modularities){
    clustering_stats->add_modularity_objective(modularity);
  }
  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_COMMUNITIES_H_