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

#ifndef PARALLEL_CLUSTERING_CLUSTERERS_SLPA_CLUSTERER_H_
#define PARALLEL_CLUSTERING_CLUSTERERS_SLPA_CLUSTERER_H_

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

class SLPAClusterer : public InMemoryClusterer {
 public:
  Graph* MutableGraph() override { return &graph_; }

  absl::StatusOr<Clustering> Cluster(
      const ClustererConfig& config) const override;

  std::set<std::set<gbbs::uintE>> findMaximalSets(const std::vector<std::set<gbbs::uintE>>& sets) const;

  // Postprocess `memory` and return clustering. If `remove_nested` is true, nested communities are removed. `max_iteration` is the 
  // total number of labels in each map in `memory`. labels appears with probability less than 
  // `prune_threshold` are removed from each map in `memory`.
  // `memory[i]` contains the memory of node `i`. 
  Clustering postprocessing(const parlay::sequence<std::map<gbbs::uintE, size_t>>& memory, bool remove_nested, int max_iteration, 
                            double prune_threshold) const;

 private:
  GbbsGraph graph_;
};

}  // namespace in_memory
}  // namespace research_graph

#endif  // PARALLEL_CLUSTERING_CLUSTERERS_SLPA_CLUSTERER_H_
