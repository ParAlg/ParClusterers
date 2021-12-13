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

#pragma once

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <tuple>


#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
// #include "parcluster/api/gbbs-graph.h"
// #include "parcluster/api/in-memory-clusterer-base.h"
// #include "parcluster/api/parallel/parallel-graph-utils.h"
// #include "parcluster/api/status_macros.h"

#include "../common/common.h"

namespace research_graph {
namespace in_memory {

class GeneralHACClusterer {
 public:
  using PointId = unsigned int;
  // Represents clustering: each element of the vector contains the set of
  // PointIds in one cluster. We call a clustering non-overlapping if the
  // elements of the clustering are nonempty vectors that together contain each
  // PointId exactly once.
  using Clustering = std::vector<std::vector<PointId>>;
  
  // Represents hierarhical clustering: each element of the vector
  // contains the set of 4 numbers: <id1, id2, size, height>
  using HierarchicalClustering = std::vector<std::tuple<PointId, PointId, std::size_t, double>>;

  absl::StatusOr<HierarchicalClustering> HierarchicalCluster(
      const ClustererConfig& config) const;

  absl::StatusOr<Clustering> Cluster(
      const ClustererConfig& config) const;

 private:
  SymMatrix<double> matrix_;
};

}  // namespace in_memory
}  // namespace research_graph

