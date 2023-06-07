#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_NMI_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_NMI_H_

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
#include "clusterers/stats/stats_ari.h"

#include "parlay/hash_table.h"

namespace research_graph::in_memory {


// Compute NMI using cluster entropy, ground truth entropy, and conditional entropy
inline absl::Status ComputeNMI(
  const size_t n,
  const InMemoryClusterer::Clustering& clustering, ClusteringStatistics* clustering_stats,
  const InMemoryClusterer::Clustering& ground_truth,
  const ClusteringStatsConfig& clustering_stats_config){
    const bool compute_nmi = clustering_stats_config.compute_nmi();
  if (!compute_nmi) {
    return absl::OkStatus();
  }


  size_t num_cluster_1 = clustering.size();
  size_t num_cluster_2 = ground_truth.size();

  // Ground truth entropy
  std::vector<double> community_entropys(num_cluster_2);
  parlay::parallel_for(0, num_cluster_2, [&](size_t i){
    double proportion = ((double)ground_truth[i].size()) / n;
    community_entropys[i] = -proportion * log2(proportion); 
  });
  double community_entropy = parlay::reduce(community_entropys);

  // Cluster entropy
  std::vector<double> cluster_entropys(num_cluster_1);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    double proportion = ((double)clustering[i].size()) / n;
    cluster_entropys[i] = -proportion * log2(proportion); 
  });
  double cluster_entropy = parlay::reduce(cluster_entropys);


  // Calculate conditional entropy
  using tableT = parlay::hashtable<hash_numeric_unsigned <gbbs::uintE> >;
  auto empty_val = hash_numeric_unsigned <gbbs::uintE>().empty();

  std::vector<tableT*> tables1(num_cluster_1);
  std::vector<tableT*> tables2(num_cluster_2);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    tables1[i] = new tableT(clustering[i].size(), hash_numeric_unsigned<gbbs::uintE>{});
    parlay::parallel_for(0, clustering[i].size(), [&](size_t j){
    tables1[i]->insert(clustering[i][j]);
    });
  });
  parlay::parallel_for(0, num_cluster_2, [&](size_t i){
    tables2[i] = new tableT(ground_truth[i].size(), hash_numeric_unsigned<gbbs::uintE>{});
    parlay::parallel_for(0, ground_truth[i].size(), [&](size_t j){
    tables2[i]->insert(ground_truth[i][j]);
    });
  });

  std::vector<double> conditional_entropys(num_cluster_1);
  parlay::parallel_for(0, num_cluster_1, [&](size_t i){
    std::vector<double> match(num_cluster_2);
    size_t cluster_size = clustering[i].size();
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

        double proportion = ((double)val) / cluster_size;
        if (proportion > 0){
            match[j] = proportion * log2(proportion);
        }
        else{
            match[j] = 0;
        }
        
        
    });
    double entropy = parlay::reduce(match);
    double proportion = ((double) cluster_size) / n;
    conditional_entropys[i] = - proportion * entropy;
  });

  double conditional_entropy = parlay::reduce(conditional_entropys);

  // NMI calculation from calculated entropys
  double nmi_value = 2 * (community_entropy - conditional_entropy) / (community_entropy + cluster_entropy);


  clustering_stats->set_nmi(nmi_value);

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