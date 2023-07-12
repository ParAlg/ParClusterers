#include "clusterers/labelprop_clusterer/labelprop-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "clusterers/labelprop_clusterer/labelprop_config.pb.h"

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<LabelPropagationClusterer::Clustering>
LabelPropagationClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;

  LabelPropagationClustererConfig labelprop_config;
  config.any_config().UnpackTo(&labelprop_config);
  int max_iteration = labelprop_config.max_iteration();
  int update_threshold = labelprop_config.update_threshold();

  if(update_threshold < 0){
    return absl::FailedPreconditionError("update_threshold must be non-negative");
  }


  // TODO: fix the case when the graph is unweighted, then all wgh should be 1.


  auto clusters = parlay::sequence<gbbs::uintE>::from_function(n, [&] (size_t i) { return i; });
  auto active_nodes = parlay::sequence<gbbs::uintE>::from_function(n, [&] (size_t i) { return i; });
  auto is_active = parlay::sequence<bool>(n);
  
  int n_iterations = 0; // number of iterations
  std::atomic<int> n_update;
  n_update.store(0);

  // propagate labels as long as a label has changed... or maximum iterations reached
  while ((n_update.load() > update_threshold) && (n_iterations < max_iteration)) {
    n_iterations += 1;

    // reset updated
    n_update = 0;

    parlay::parallel_for(0, active_nodes.size(), [&] (size_t i) {
      auto node_id = active_nodes[i];
      is_active[node_id] = false;
      });

    parlay::parallel_for(0, active_nodes.size(), [&] (size_t i) {
      auto node_id = active_nodes[i];

      auto degree =  graph_.Graph()->get_vertex(node_id).out_degree();
      gbbs::uintE heaviest;
      if(degree < 1000){
          // neighborLabelCounts maps label -> frequency in the neighbors
          std::map<gbbs::uintE, double> label_weights_sum;
          auto map_f = [&] (const auto& u, const auto& v, const auto& wgh) {
            label_weights_sum[clusters[v]] += wgh;
          };
          graph_.Graph()->get_vertex(node_id).out_neighbors().map(map_f, false);

          heaviest = std::max_element(label_weights_sum.begin(), label_weights_sum.end(),
                                                  [](const std::pair<gbbs::uintE, double> &p1,
                                                    const std::pair<gbbs::uintE, double> &p2) {
                                                      return p1.second < p2.second;
                                                  })
                                    ->first;
      } else {
        auto neighbors = graph_.Graph()->get_vertex(node_id).out_neighbors();

        auto label_weights = parlay::delayed_seq<std::pair<gbbs::uintE, double>>(degree, [&](size_t i){
          auto [v, w] = neighbors.get_ith_neighbor(i);
          return std::make_pair(clusters[v], w);
        });

        auto grouped = parlay::group_by_key(label_weights);
        auto label_weights_sum = parlay::sequence<std::pair<double, gbbs::uintE>>(grouped.size());
        parlay::parallel_for(0, grouped.size(), [&](gbbs::uintE i){
          auto cluster_id = grouped[i].first;
          auto w = parlay::reduce(grouped[i].second);
          label_weights_sum[i] = {w, cluster_id};
        });

        heaviest = parlay::max_element(label_weights_sum)->second;
      }


      // A can only change in the next round if any of its neighbors change label.
      if (clusters[node_id] != heaviest) { // UPDATE
          clusters[node_id] = heaviest;
          n_update.fetch_add(1);
          auto activate_f = [&] (const auto& u, const auto& v, const auto& wgh) {
            if(!is_active[v]) is_active[v] = true; 
          };
          graph_.Graph()->get_vertex(node_id).out_neighbors().map(activate_f);
      } 
    });


    active_nodes = parlay::pack(active_nodes, is_active.cut(0, active_nodes.size()));
    } // end while

  auto ret = research_graph::DenseClusteringToNestedClustering<gbbs::uintE>(clusters);
  std::cout << "Num clusters = " << ret.size() << std::endl;
  return ret;
}

}  // namespace in_memory
}  // namespace research_graph
