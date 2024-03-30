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
    result[0] = (static_cast<double>(graph.Graph()->m)) / (static_cast<double>(n)*(n-1));
  }else{
    parlay::parallel_for(0, clustering.size(), [&] (size_t i) {
        if (clustering[i].size() == 1){
          result[i] = 0;
        }
        else{
          size_t m_subgraph = get_subgraph_num_edges(graph, clustering[i], cluster_ids);
          double m_total = clustering[i].size()*(clustering[i].size()-1);
          // std::cout << "m_subgraph" << " " << m_subgraph << std::endl;
          // std::cout <<  "m_total" << " " <<  m_total << std::endl;
          result[i] = (static_cast<double>(m_subgraph)) / (static_cast<double>(m_total));
        }
    });
  }
  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  auto weighted_result_func = [&](std::size_t i) {
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

// compute the edge density of each cluster
// edge density is the number of edges divided by the number of possible edges
inline absl::Status ComputeEdgeDensityOverlap(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_edge_density_overlap = clustering_stats_config.compute_edge_density_overlap();
  if (!compute_edge_density_overlap) {
    return absl::OkStatus();
  }

  parlay::sequence<gbbs::uintE> cluster_ids_overlap = parlay::sequence<gbbs::uintE>(graph.Graph()->n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_ids_overlap[cluster[j]] = i;
    });
  });

  std::size_t n = graph.Graph()->n;
  auto result = std::vector<double>(clustering.size());

  if(clustering.size()==1){
    result[0] = (static_cast<double>(graph.Graph()->m)) / (static_cast<double>(n)*(n-1));
  }else{
    for(size_t i = 0; i < clustering.size(); i++) {
        if (clustering[i].size() == 1){
          result[i] = 0;
        }
        else{
          const auto& cluster = clustering[i];
          parlay::parallel_for(0, cluster.size(), [&](size_t j){
            cluster_ids_overlap[cluster[j]] = i;
          });
          size_t m_subgraph = get_subgraph_num_edges(graph, clustering[i], cluster_ids_overlap);
          double m_total = clustering[i].size()*(clustering[i].size()-1);
          // std::cout << "m_subgraph" << " " << m_subgraph << std::endl;
          // std::cout <<  "m_total" << " " <<  m_total << std::endl;
          result[i] = (static_cast<double>(m_subgraph)) / (static_cast<double>(m_total));
        }
    }
  }
  auto result_func = [&](std::size_t i) {
    return result[i];
  };
  parlay::sequence<double> cluster_sum = parlay::sequence<double>(n, 0);
  parlay::sequence<gbbs::uintE> cluster_count = parlay::sequence<gbbs::uintE>(n, 0);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_sum[cluster[j]] += result_func(i);
      cluster_count[cluster[j]] += 1;
    });
  });

  double weighted_mean_overlap = 0;
  for (int i=0;i<cluster_sum.size();++i){
    if (cluster_count[i] != 0) {
      weighted_mean_overlap += cluster_sum[i] / cluster_count[i];
    }
  }
  weighted_mean_overlap = weighted_mean_overlap / n;
  clustering_stats->set_weighted_edge_density_overlap_mean(weighted_mean_overlap);

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

// compute the triangle density of each cluster with overlapping clusters
// triangle density is the number of triangles divided by the number of wedges
// if no wedge, density is 0
inline absl::Status ComputeTriangleDensityOverlap(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const parlay::sequence<gbbs::uintE>& cluster_ids, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_triangle_density_overlap = clustering_stats_config.compute_triangle_density_overlap();
  if (!compute_triangle_density_overlap) {
    return absl::OkStatus();
  }

  parlay::sequence<gbbs::uintE> cluster_ids_overlap = parlay::sequence<gbbs::uintE>(graph.Graph()->n);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_ids_overlap[cluster[j]] = i;
    });
  });

  std::size_t n = graph.Graph()->n;
  auto result = std::vector<double>(clustering.size());
  auto f = [&] (gbbs::uintE u, gbbs::uintE v, gbbs::uintE w) { };

  //even if clustering.size()==1, we need to get the subgraph because could not match 'symmetric_graph' against 'symmetric_ptr_graph'
    for(size_t i = 0; i < clustering.size(); i++) {
        const auto& cluster = clustering[i];
        parlay::parallel_for(0, cluster.size(), [&](size_t j){
          cluster_ids_overlap[cluster[j]] = i;
        });
        auto G = get_subgraph<gbbs::empty>(graph, clustering[i], cluster_ids_overlap); //have to use unweighted graph, otherwise result is wrong
        size_t num_wedges = get_num_wedges(&G);
        if(num_wedges == 0){
          result[i] = 0;
        }else{
          size_t num_tri = 0;
          if (G.num_edges() >= 3 && G.num_vertices() >= 3){
            num_tri =  gbbs::Triangle_degree_ordering(G, f);
          }
          result[i] = (static_cast<double>(num_tri)) / (static_cast<double>(num_wedges));
        }
    }
  // for(double l:result) std::cout << l << std::endl;
  auto result_func = [&](std::size_t i) {
    return result[i];
  };

  parlay::sequence<double> cluster_sum = parlay::sequence<double>(n, 0);
  parlay::sequence<gbbs::uintE> cluster_count = parlay::sequence<gbbs::uintE>(n, 0);
  parlay::parallel_for(0, clustering.size(), [&](size_t i){
    const auto& cluster = clustering[i];
    parlay::parallel_for(0, cluster.size(), [&](size_t j){
      cluster_sum[cluster[j]] += result_func(i);
      cluster_count[cluster[j]] += 1;
    });
  });

  double weighted_mean_overlap = 0;
  for (int i=0;i<cluster_sum.size();++i){
    if (cluster_count[i] != 0) {
      weighted_mean_overlap += cluster_sum[i] / cluster_count[i];
    }
  }
  weighted_mean_overlap = weighted_mean_overlap / n;
  clustering_stats->set_weighted_triangle_density_overlap_mean(weighted_mean_overlap);

  return absl::OkStatus();
}


}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_DENSITY_H_