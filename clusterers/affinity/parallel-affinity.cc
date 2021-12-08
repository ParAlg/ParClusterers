// Copyright 2020 The Google Research Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "clusterers/affinity/parallel-affinity.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "clusterers/affinity/parallel-affinity-internal.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

void AddNewClusters(ParallelAffinityClusterer::Clustering new_clusters,
                    ParallelAffinityClusterer::Clustering* clustering) {
  std::cout << "Num new clusters being added: " << new_clusters.size() << std::endl;
  clustering->reserve(clustering->size() + new_clusters.size());
  std::move(new_clusters.begin(), new_clusters.end(),
            std::back_inserter(*clustering));
  std::cout << "Added new clusters." << std::endl;
}

double GetDynamicWeightThreshold(const AffinityClustererConfig& affinity_config, int iteration) {
  int num_iterations = affinity_config.num_iterations();
  auto config = affinity_config.dynamic_weight_threshold_config();
  if (num_iterations == 1) return config.upper_bound();
  auto weight_decay_function = config.weight_decay_function();
  if (weight_decay_function == DynamicWeightThresholdConfig::LINEAR_DECAY) {
    return config.upper_bound() - ((config.upper_bound() - config.lower_bound()) / (num_iterations - 1)) * iteration;
  } else if (weight_decay_function == DynamicWeightThresholdConfig::EXPONENTIAL_DECAY) {
    return config.upper_bound() * std::pow(config.lower_bound() / config.upper_bound(),
      (double) iteration / ((double) num_iterations - 1.0));
  } else {
    std::cout << "Unknown weight decay function" << std::endl;
    exit(-1);
  }
}

double GetAffinityWeightThreshold(const AffinityClustererConfig& affinity_config, int iteration) {
  auto cases = affinity_config.weight_threshold_config_case();
  if (cases == AffinityClustererConfig::kWeightThreshold) return affinity_config.weight_threshold();
  if (cases == AffinityClustererConfig::kPerIterationWeightThresholds) {
    int num_thresholds = affinity_config.per_iteration_weight_thresholds().thresholds_size();
    if (num_thresholds == 0) return 0.0;
    return affinity_config.per_iteration_weight_thresholds().thresholds(std::min(iteration, num_thresholds - 1));
  }
  if (cases == AffinityClustererConfig::kDynamicWeightThresholdConfig) return GetDynamicWeightThreshold(affinity_config, iteration);
  return 0.0;
}

absl::StatusOr<ParallelAffinityClusterer::Clustering>
ParallelAffinityClusterer::Cluster(const ClustererConfig& config) const {
  const AffinityClustererConfig& affinity_config = config.affinity_clusterer_config();
  std::size_t n = graph_.Graph()->n;

  // Initially each vertex is its own cluster.
  std::vector<gbbs::uintE> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  // The output (flat) clustering to emit.
  ParallelAffinityClusterer::Clustering clustering;

  std::unique_ptr<gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>> compressed_graph;

  std::vector<gbbs::uintE> node_weights;
  std::cout << "Affinity num iterations = " << affinity_config.num_iterations() << std::endl;

  for (int i = 0; i < affinity_config.num_iterations(); ++i) {

    double weight_threshold = GetAffinityWeightThreshold(affinity_config, i);
    gbbs::symmetric_ptr_graph<gbbs::symmetric_vertex, float>* current_graph =
      (i == 0) ? graph_.Graph() : compressed_graph.get();

    std::vector<gbbs::uintE> compressed_cluster_ids;
    ASSIGN_OR_RETURN(
      compressed_cluster_ids,
      NearestNeighborLinkage(*current_graph,
                            weight_threshold));

    std::cout << "Iteration: " << i << " n = " << (*current_graph).n << " m = " << (*current_graph).m << std::endl;

    cluster_ids = FlattenClustering(cluster_ids, compressed_cluster_ids);

    // TODO(jeshi): Performance can be improved by not finding finished
    // clusters on the last round
    auto new_clusters =
        FindFinishedClusters(*(graph_.Graph()), affinity_config, cluster_ids);

    AddNewClusters(std::move(new_clusters), &clustering);

    // Exit if all clusters are finished
    auto exit_seq = parlay::sequence<bool>::from_function(
        cluster_ids.size(),
        [&](std::size_t i) { return (cluster_ids[i] == UINT_E_MAX); });
    bool to_exit = parlay::reduce(
        exit_seq,
        parlay::make_monoid([](bool a, bool b) { return a && b; }, true));
    if (to_exit || i == affinity_config.num_iterations() - 1) break;

    GraphWithWeights<gbbs::uintE> new_compressed_graph;
    ASSIGN_OR_RETURN(
      new_compressed_graph,
      CompressGraph(*current_graph, node_weights, compressed_cluster_ids, affinity_config));
    compressed_graph.swap(new_compressed_graph.graph);
    node_weights = new_compressed_graph.node_weights;
  }

  std::unique_ptr<bool[]> finished_vertex(new bool[n]);
  parlay::parallel_for(0, n, [&](std::size_t i) {
    finished_vertex[i] = (cluster_ids[i] != UINT_E_MAX);
  });
  auto new_clusters = ComputeClusters(cluster_ids, std::move(finished_vertex));
  AddNewClusters(std::move(new_clusters), &clustering);

  return clustering;
}

}  // namespace in_memory
}  // namespace research_graph
