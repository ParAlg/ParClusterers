#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DENSITY_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DENSITY_H_

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

#include "external/gbbs/benchmarks/TriangleCounting/ShunTangwongsan15/Triangle.h"

namespace research_graph::in_memory {


// compute the edge density of each cluster
// edge density is the number of edges divided by the number of possible edges
inline absl::Status ComputeEdgeDensity(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_edge_density = clustering_stats_config.compute_edge_density();
  if (!compute_edge_density) {
    return absl::OkStatus();
  }

  std::size_t n = graph.Graph()->n;
  auto result = std::vector<double>(clustering.size());

  if(clustering.size()==1){
    result[0] = ((double)graph.Graph()->m) / ((double)n*(n-1));
  }else{
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        size_t m_subgraph = get_subgraph_num_edges(graph, clustering[i], cluster_ids);
        double m_total = clustering[i].size()*(clustering[i].size()-1);
        result[i] = ((double)m_subgraph) / ((double)m_total);
    });
  }
  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  set_distribution_stats(result.size(), result_func, clustering_stats->mutable_edge_density());

  return absl::OkStatus();
}

// compute the triangle density of each cluster
// triangle density is the number of triangles divided by the number of wedges
// if no wedge, density is 0
inline absl::Status ComputeTriangleDensity(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_triangle_density = clustering_stats_config.compute_triangle_density();
  if (!compute_triangle_density) {
    return absl::OkStatus();
  }

  auto result = std::vector<double>(clustering.size());
  auto f = [&] (gbbs::uintE u, gbbs::uintE v, gbbs::uintE w) { };

  //even if clustering.size()==1, we need to get the subgraph because could not match 'symmetric_graph' against 'symmetric_ptr_graph'
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        auto G = get_subgraph<gbbs::empty>(graph, clustering[i], cluster_ids); //have to use unweighted graph, otherwise result is wrong
        size_t num_wedges = get_num_wedges(&G);
        if(num_wedges == 0){
          result[i] = 0;
        }else{
          size_t num_tri = gbbs::Triangle_degree_ordering(G, f);
          result[i] = ((double)num_tri) / ((double)num_wedges);
        }
    });
  // for(double l:result) std::cout << l << std::endl;
  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  set_distribution_stats(result.size(), result_func, clustering_stats->mutable_triangle_density());

  return absl::OkStatus();
}


}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DENSITY_H_