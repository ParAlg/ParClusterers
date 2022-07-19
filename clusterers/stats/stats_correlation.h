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
        // This assumes that the graph is undirected, and self-loops are counted
        // as half of the weight.
        if (cluster_id_i == cluster_ids[v])
          return (weight - edge_weight_offset[k]) / 2;
        return 0;
      };
      shifted_edge_weight[i] = graph.Graph()->get_vertex(i).out_neighbors().reduce(
          intra_cluster_sum_map_f, add_m);
    });
    double objective = parlay::reduace_add(shifted_edge_weight);
  
    auto resolution_seq = parlay::delayed_seq<double>(n, [&](std::size_t i) {
      return cluster_weights_[cluster_ids[i]]; // cluster_weight
      //return node_weights_[i] * (cluster_weight - node_weights_[i]);
    });
    objective -= resolution[k] * parlay::reduce_add(resolution_seq) / 2;
  
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

  for(size_t k = 0; k < resolution.size(); k++){
    double total_edge_weight = 0;
    double modularity = 0;
    for (std::size_t i = 0; i < n; i++) {
      auto vtx = graph.Graph()->get_vertex(i);
      auto map_out = [&](uintE u, uintE nbhr, float w){
        total_edge_weight++;
        if (cluster_ids[i] == cluster_ids[nbhr]) {
          modularity++;
        }
      });
      vtx.out_neighbors().map(map_out, false);
    }
    //modularity = modularity / 2; // avoid double counting
    for (std::size_t i = 0; i < clustering.size(); i++) {
      double degree = 0;
      for (std::size_t j = 0; j < clustering[i].size(); j++) {
        auto vtx_id = clustering[i][j];
        auto vtx = graph.Graph()->get_vertex(vtx_id);
        degree += vtx.out_degree();
      }
      modularity -= (resolution[k] * degree * degree) / (total_edge_weight);
    }
    modularity = modularity / (total_edge_weight);
    clustering_stats->add_modularity_objective(modularity);
  }
  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_COMMUNITIES_H_