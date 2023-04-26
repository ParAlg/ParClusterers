#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_ARI_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_ARI_H_

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

#include "parlay/hash_table.h"

namespace research_graph::in_memory {

namespace {
inline std::size_t nChoose2(std::size_t x) {
  return x * (x - 1) / 2 ;
}
}

// we only need the n choose 2 values of all contingency matrix values
// do not need store all of them
// only works for non-overlapping clustering
inline absl::Status ComputeARI(const GbbsGraph& graph, 
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const InMemoryClusterer::Clustering& ground_truth,
  const ClusteringStatsConfig& clustering_stats_config){
    const bool compute_ari = clustering_stats_config.compute_ari();
  if (!compute_ari) {
    return absl::OkStatus();
  }
  size_t n = graph.Graph()->n;

  parlay::sequence<std::atomic<std::size_t> > row_sums; //  sums for clustering
  parlay::sequence<std::atomic<std::size_t> > column_sums; //  sums for cluster_ids

  // auto unique_cluster_ids = ground_truth.size();

  // auto cluster_id_indexed = parlay::delayed_seq<std::pair<gbbs::uintE, gbbs::uintE>>(n,
  // [&](size_t i){
  //   return std::make_pair(cluster_ids[i],i);
  // }
  // );

  // auto grouped_ids = parlay::group_by_key_ordered(cluster_id_indexed);

  size_t num_cluster_1 = clustering.size();
  size_t num_cluster_2 = ground_truth.size();

  using tableT = parlay::hashtable<parlay::hash_numeric<gbbs::uintE> >;

  std::vector<tableT*> tables1(num_cluster_1);
  std::vector<tableT*> tables2(num_cluster_2);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    tables1[i] = new tableT(clustering[i].size(), parlay::hash_numeric<gbbs::uintE>{});
    parlay::parallel_for(0, clustering[i].size(), [&](size_t j){
    tables1[i]->insert(clustering[i][j]);
    });
  });
  parlay::parallel_for(0, num_cluster_2, [&](size_t i){
    tables2[i] = new tableT(ground_truth[i].size(), parlay::hash_numeric<gbbs::uintE>{});
    parlay::parallel_for(0, ground_truth[i].size(), [&](size_t j){
    tables2[i]->insert(ground_truth[i][j]);
    });
  });

  // parlay::parallel_for(0, num_cluster_2, [&](size_t i){
  //   tables2[i] = new tableT(grouped_ids[i].second.size(), parlay::hash_numeric<gbbs::uintE>{});
  //   parlay::parallel_for(0, grouped_ids[i].second.size(), [&](size_t j){
  //   tables2[i]->insert(grouped_ids[i].second[j]);
  //   });
  // });

  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
  parlay::parallel_for(0, num_cluster_2, [&](size_t j){
    size_t val = 0;

    if(clustering[i].size() < ground_truth[j].size()){
      auto flags = parlay::delayed_seq<std::size_t>(clustering[i].size(), [&](size_t k){
        return tables2[j]->find(clustering[i][k]) == -1? 0 : 1 ; // find clustering's id in table2
      });
      val = parlay::reduce(flags);
    }else{
      auto flags = parlay::delayed_seq<std::size_t>(ground_truth[j].size(), [&](size_t k){
        return tables1[i]->find(ground_truth[j][k]) == -1? 0 : 1 ;
      });
      val = parlay::reduce(flags);
    }

    parlay::write_add(&(row_sums[i]), val);
    parlay::write_add(&(column_sums[j]), val);
  });
  });

  auto row_sums_delay = parlay::delayed_seq<size_t>(row_sums.size(), [&](size_t i){
    return row_sums[i].load();
  });
  size_t nChoose2ContingencySum = parlay::reduce(row_sums_delay);
  
  auto row_n_choose_2_values = parlay::delayed_seq<std::size_t>(row_sums.size(), [&](std::size_t i){
    return nChoose2(row_sums[i]);
  });
   auto column_n_choose_2_values = parlay::delayed_seq<std::size_t>(column_sums.size(), [&](std::size_t i){
    return nChoose2(column_sums[i]);
  });

  size_t nChoose2RowSum = parlay::reduce(row_n_choose_2_values);
  size_t nChoose2ColumnSum = parlay::reduce(column_n_choose_2_values);

  double numerator = nChoose2ContingencySum
                     - (nChoose2RowSum * nChoose2ColumnSum) / nChoose2(n);
  double denominator = 0.5 * (nChoose2RowSum + nChoose2ColumnSum)
                       - (nChoose2RowSum * nChoose2ColumnSum) / nChoose2(n);
  double ariValue = numerator / denominator;
  
  clustering_stats->set_ari(ariValue);

  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    delete tables1[i];
  });

  parlay::parallel_for(0, num_cluster_2, [&](size_t i){
    delete tables2[i];
  });

  return absl::OkStatus();
}

}  // namespace research_graph::in_memory

#endif