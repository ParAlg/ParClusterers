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


// Compute the edge density of each cluster.
// Edge density is the number of edges divided by the number of possible edges.
// It assumes that all node ids in clustering and cluster_ids are in `graph`.
inline absl::Status ComputeEdgeDensity(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_edge_density = clustering_stats_config.compute_edge_density();
  const bool include_zero_degree_nodes = clustering_stats_config.include_zero_degree_nodes();
  if (!compute_edge_density) {
    return absl::OkStatus();
  }

  std::size_t n = graph.Graph()->n;
  auto result = parlay::sequence<double>(clustering.size());

  parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
    const double cluster_size = clustering[i].size();
    if (cluster_size == 1){ // A singleton cluster.
      const auto singleton_node_id = clustering[i][0];
      if ((!include_zero_degree_nodes) && graph.Degree(singleton_node_id) == 0){
        result[i] = -1;
      }else{
        result[i] = 1;
      }
    }else{
      double m_total = cluster_size * (cluster_size - 1);
      // std::cout <<  "m_total" << " " <<  m_total << std::endl;
      if(clustering.size()==1 && cluster_size == n){ // All nodes are in a cluster.
        result[i] = (static_cast<double>(graph.Graph()->m)) / m_total;
      }else{
        size_t m_subgraph = get_subgraph_num_edges(graph, clustering[i], cluster_ids);
        // std::cout << "m_subgraph" << " " << m_subgraph << std::endl;
        result[i] = (static_cast<double>(m_subgraph)) / m_total;
      }
    }
  });

  // Remove singleton zero degree nodes.
  // Recompute `n` for weighted_result_func to ignore singleton zero-degree nodes.
  if (!include_zero_degree_nodes){
    result = parlay::filter(result, [&](double i){return i != -1;});
    auto nodes_flags = parlay::delayed_seq<size_t>(n, [&](size_t i){
      return graph.Degree(i) == 0 ? 0 : 1;
    });
    n = parlay::reduce(nodes_flags);
  }

  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  auto weighted_result_func = [&](std::size_t i) {
    // Divide result.size() instead of n because some singleton 
    // zero degree nodes might be filtered out above.
    return result[i] * (clustering[i].size() * 1.0 / n);
  };
  double weighted_mean = 0;
  for (int i=0;i<result.size();++i){
    weighted_mean += weighted_result_func(i);
    // std::cout << result[i] << " " << weighted_result_func(i) << std::endl;
  }
  set_distribution_stats(result.size(), result_func, clustering_stats->mutable_edge_density());
  clustering_stats->set_weighted_edge_density_mean(weighted_mean);

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

  std::size_t n = graph.Graph()->n;
  auto result = std::vector<double>(clustering.size());
  auto f = [&] (gbbs::uintE u, gbbs::uintE v, gbbs::uintE w) { };

  //even if clustering.size()==1, we need to get the subgraph because could not match 'symmetric_graph' against 'symmetric_ptr_graph'
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
      if (clustering[i].size() == 1){
        result[0] = 1;
      } else {
        auto G = get_subgraph<gbbs::empty>(graph, clustering[i], cluster_ids); //have to use unweighted graph, otherwise result is wrong
        size_t num_wedges = get_num_wedges(&G);
        if(num_wedges == 0){
          result[i] = 0;
        }else{
          // std::cout << "start \n";
          // std::cout << "clustering size " << clustering[i].size() << std::endl;
          // std::cout << clustering[i][0] << std::endl;
          // std::cout << "subgrpah num nodes " << G.n << std::endl;
          // std::cout << "subgraph num edges " << G.num_edges() << std::endl;

          size_t num_tri = 0;
          if (G.num_edges() >= 3 && G.num_vertices() >= 3){
            num_tri =  gbbs::Triangle_degree_ordering(G, f);
          }
          // std::cout << "end num_tri = " << num_tri << std::endl;
          result[i] = (static_cast<double>(num_tri)) / (static_cast<double>(num_wedges));
        }
      }
    });
  // for(double l:result) std::cout << l << std::endl;
  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  set_distribution_stats(result.size(), result_func, clustering_stats->mutable_triangle_density());

  auto weighted_result_func = [&](std::size_t i) {
    return result[i] * (clustering[i].size() * 1.0 / n);
  };
  double weighted_mean = 0;
  for (int i=0;i<result.size();++i){
    weighted_mean += weighted_result_func(i);
    // std::cout << result[i] << " " << weighted_result_func(i) << std::endl;
  }
  clustering_stats->set_weighted_triangle_density_mean(weighted_mean);

  return absl::OkStatus();
}


}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DENSITY_H_