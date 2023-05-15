#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_ARI_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_ARI_H_

#include <chrono>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <cmath>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"

#include "clusterers/clustering_stats.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

#include "parlay/hash_table.h"

namespace research_graph::in_memory {

namespace {
inline std::size_t nChoose2(std::size_t x) {
  return x * (x - 1) / 2 ;
}

// Example for hashing numeric values.
// T must be some integer type
template <class T>
struct hash_numeric_unsigned {
  using eType = T;
  using kType = T;
  eType empty() { return std::numeric_limits<T>::max(); }
  kType getKey(eType v) { return v; }
  size_t hash(kType v) { return static_cast<size_t>(parlay::hash64(v)); }
  int cmp(kType v, kType b) { return (v > b) ? 1 : ((v == b) ? 0 : -1); }
  bool replaceQ(eType, eType) { return 0; }
  eType update(eType v, eType) { return v; }
  bool cas(eType* p, eType o, eType n) {
    // TODO: Make this use atomics properly. This is a quick
    // fix to get around the fact that the hashtable does
    // not use atomics. This will break for types that
    // do not inline perfectly inside a std::atomic (i.e.,
    // any type that the standard library chooses to lock)
    return std::atomic_compare_exchange_strong_explicit(
      reinterpret_cast<std::atomic<eType>*>(p), &o, n, std::memory_order_relaxed, std::memory_order_relaxed);
  }
};

}

// we only need the n choose 2 values of all contingency matrix values
// do not need store all of them
// only works for non-overlapping clustering
inline absl::Status ComputeARI(
  const size_t n,
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const InMemoryClusterer::Clustering& ground_truth,
  const ClusteringStatsConfig& clustering_stats_config){
    const bool compute_ari = clustering_stats_config.compute_ari();
  if (!compute_ari) {
    return absl::OkStatus();
  }



  size_t num_cluster_1 = clustering.size();
  size_t num_cluster_2 = ground_truth.size();

  parlay::sequence<std::atomic<std::size_t> > row_sums(num_cluster_1); //  sums for clustering
  parlay::sequence<std::atomic<std::size_t> > column_sums(num_cluster_2); //  sums for ground_truth


  using tableT = parlay::hashtable<hash_numeric_unsigned <gbbs::uintE> >;

  auto empty_val = hash_numeric_unsigned <gbbs::uintE>().empty();

  std::vector<tableT*> tables1(num_cluster_1);
  std::vector<tableT*> tables2(num_cluster_2);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    row_sums[i].store(0);
    tables1[i] = new tableT(clustering[i].size(), hash_numeric_unsigned<gbbs::uintE>{});
    parlay::parallel_for(0, clustering[i].size(), [&](size_t j){
    tables1[i]->insert(clustering[i][j]);
    });
  });
  parlay::parallel_for(0, num_cluster_2, [&](size_t i){
    column_sums[i].store(0);
    tables2[i] = new tableT(ground_truth[i].size(), hash_numeric_unsigned<gbbs::uintE>{});
    parlay::parallel_for(0, ground_truth[i].size(), [&](size_t j){
    tables2[i]->insert(ground_truth[i][j]);
    });
  });

  std::atomic<size_t> nChoose2ContingencySum;
  nChoose2ContingencySum.store(0);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
  parlay::parallel_for(0, num_cluster_2, [&](size_t j){
    size_t val = 0;

    if(clustering[i].size() < ground_truth[j].size()){
      auto flags = parlay::delayed_seq<std::size_t>(clustering[i].size(), [&](size_t k){
        return tables2[j]->find(clustering[i][k]) == empty_val? 0 : 1 ; // find clustering's id in table2
      });
      val = parlay::reduce(flags);
    }else{
      auto flags = parlay::delayed_seq<std::size_t>(ground_truth[j].size(), [&](size_t k){
        return tables1[i]->find(ground_truth[j][k]) == empty_val? 0 : 1 ;
      });
      val = parlay::reduce(flags);
    }
    parlay::write_add(&nChoose2ContingencySum, nChoose2(val));

    parlay::write_add(&(row_sums[i]), val);
    parlay::write_add(&(column_sums[j]), val);
  });
  });
  
  auto row_n_choose_2_values = parlay::delayed_seq<std::size_t>(row_sums.size(), [&](std::size_t i){
    return nChoose2(row_sums[i].load());
  });
   auto column_n_choose_2_values = parlay::delayed_seq<std::size_t>(column_sums.size(), [&](std::size_t i){
    return nChoose2(column_sums[i].load());
  });

  size_t nChoose2RowSum = parlay::reduce(row_n_choose_2_values);
  size_t nChoose2ColumnSum = parlay::reduce(column_n_choose_2_values);

  std::cout << "nChoose2RowSum " << nChoose2RowSum << std::endl;
  std::cout << "nChoose2ColumnSum " << nChoose2ColumnSum << std::endl;
  std::cout << "nChoose2ContingencySum " << nChoose2ContingencySum  << std::endl;



  double n_choose_2 = (double) nChoose2(n);
  double largest_sqrt = std::sqrt(std::numeric_limits<double>::max());
  double ariValue = 0;

  if (n_choose_2 >= largest_sqrt || nChoose2ContingencySum >= largest_sqrt ||
      nChoose2RowSum >= largest_sqrt/2 || nChoose2ColumnSum >= largest_sqrt/2){
    // avoid overflow
    double numerator = ((double)nChoose2ContingencySum)
                      - (nChoose2RowSum * nChoose2ColumnSum) / n_choose_2;
    double denominator = 0.5 * (nChoose2RowSum + nChoose2ColumnSum)
                        - (nChoose2RowSum * nChoose2ColumnSum) / n_choose_2;
    ariValue = numerator / denominator;
  }else {
    // better numerical accuracy
    double numerator = ((double)nChoose2ContingencySum) *  n_choose_2
                      - (nChoose2RowSum * nChoose2ColumnSum) ;
    double denominator = 0.5 * (nChoose2RowSum + nChoose2ColumnSum) * n_choose_2
                        - (nChoose2RowSum * nChoose2ColumnSum);
    ariValue = numerator / denominator;
  }


  
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