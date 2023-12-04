#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_COMMUNITIES_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_COMMUNITIES_H_

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
#include "clusterers/stats/stats_utils.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph::in_memory {

// TODO(jeshi): bad coding practice fix
inline absl::Status ReadCommunities(const char* filename,
  std::vector<std::vector<gbbs::uintE>>& communities) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  std::string line;
  while (std::getline(infile, line)) {
    std::vector<gbbs::uintE> row_values;
    split(line, '\t', row_values);
    std::sort(row_values.begin(), row_values.end());
    communities.push_back(row_values);
  }
  return absl::OkStatus();
}

inline absl::Status CompareCommunities(std::vector<std::vector<gbbs::uintE>>& communities, const InMemoryClusterer::Clustering& clustering_, ClusteringStatistics* clustering_stats, const ClusteringStatsConfig& clustering_stats_config) {
  const bool compute_precision_recall = clustering_stats_config.compute_precision_recall();
  if (!compute_precision_recall) {
    return absl::OkStatus();
  }

  const double f_score = clustering_stats_config.f_score_param() != 0 ? clustering_stats_config.f_score_param() : 1;
  clustering_stats->set_f_score_param(f_score);

  auto clustering = clustering_;
  // Pre-sort clusters to optimize the inner loop
  for (auto& cluster : clustering) {
    std::sort(cluster.begin(), cluster.end());
  }

  // precision = num correct results (matches b/w clustering and comm) / num returned results (in clustering)
  // recall = num correct results (matches b/w clustering and comm) / num relevant results (in comm)
  parlay::sequence<double> precision_vec = parlay::sequence<double>::from_function(
    communities.size(), [](std::size_t i){return 0;});
  parlay::sequence<double> recall_vec = parlay::sequence<double>::from_function(
    communities.size(), [](std::size_t i){return 0;});
  parlay::sequence<double> f_score_vec = parlay::sequence<double>::from_function(
    communities.size(), [](std::size_t i){return 0;});
  /*parlay::parallel_for(0, clustering.size(), [&](std::size_t i) {
    auto cluster = clustering[i];
    std::sort(cluster.begin(), cluster.end());
  });*/
  parlay::parallel_for(0, communities.size(), [&](std::size_t j) {
    auto community = communities[j];
    std::sort(community.begin(), community.end());

    std::size_t max_intersect = 0;
    std::size_t max_idx = 0;
    if (communities.size() > 30) {
      std::vector<gbbs::uintE> intersect(community.size());
      // Find the community in communities that has the greatest intersection with cluster
      for (std::size_t i = 0; i < clustering.size(); i++) {
        const auto& cluster = clustering[i];
        auto it = std::set_intersection(cluster.begin(), cluster.end(), 
          community.begin(), community.end(), intersect.begin());
        std::size_t it_size = it - intersect.begin();

        if (it_size > max_intersect) {
          max_intersect = it_size;
          max_idx = i;
        }
      }
    }
    else{
      std::vector<std::size_t> intersections(clustering.size());
      parlay::parallel_for(0, clustering.size(), [&](std::size_t i) {
        const auto& cluster = clustering[i];
        std::vector<gbbs::uintE> intersect(std::min(cluster.size(), community.size()));
        auto it = std::set_intersection(cluster.begin(), cluster.end(), 
          community.begin(), community.end(), intersect.begin());
        std::size_t it_size = it - intersect.begin();
        intersections[i] = it_size;
      });
        
      max_intersect = intersections[0];
      for (std::size_t i = 1; i < clustering.size(); ++i) {
          if (intersections[i] > max_intersect) {
              max_intersect = intersections[i];
              max_idx = i;
          }
      }
    }
    precision_vec[j] = (double) max_intersect / (double) clustering[max_idx].size();
    recall_vec[j] = (communities[j].size() == 0) ? 0 : 
      (double) max_intersect / (double) communities[j].size();
    f_score_vec[j] = (precision_vec[j] == 0 & recall_vec[j] == 0) ? 0:
      (1 + f_score * f_score) * precision_vec[j] * recall_vec[j] / ((f_score * f_score * precision_vec[j]) + recall_vec[j]);
  });

  auto precision_func = [&](std::size_t i) {
    return precision_vec[i];
  };
  set_distribution_stats(precision_vec.size(), precision_func, clustering_stats->mutable_community_precision());

  auto recall_func = [&](std::size_t i) {
    return recall_vec[i];
  };
  set_distribution_stats(recall_vec.size(), recall_func, clustering_stats->mutable_community_recall());

  auto f_score_func = [&](std::size_t i) {
    return f_score_vec[i];
  };
  set_distribution_stats(f_score_vec.size(), f_score_func, clustering_stats->mutable_f_score());

  /*double avg_precision = parlay::reduce(precision_vec);
  double avg_recall = parlay::reduce(recall_vec);
  avg_precision /= communities.size();
  avg_recall /= communities.size();
  std::cout << "Avg precision: " << std::setprecision(17) << avg_precision << std::endl;
  std::cout << "Avg recall: " << std::setprecision(17) << avg_recall << std::endl;*/

  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_COMMUNITIES_H_