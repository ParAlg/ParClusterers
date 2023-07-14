#include "clusterers/slpa_clusterer/slpa-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <random>
#include <map>

#include "clusterers/slpa_clusterer/slpa_config.pb.h"

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/parallel/parallel-graph-utils.h"
#include "parcluster/api/status_macros.h"

#include "parlay/hash_table.h"

namespace research_graph {
namespace in_memory {



gbbs::uintE speak_sequential(const gbbs::uintE& v, const std::map<gbbs::uintE, std::size_t>& memory, const std::size_t m){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m - 1);

    size_t rnd = dist(gen);

    for (const auto& kv : memory) {
        if (rnd < kv.second)
            return kv.first;
        rnd -= kv.second;
    }
    return 0; // should never reach this line
}

absl::StatusOr<SLPAClusterer::Clustering>
SLPAClusterer::Cluster(const ClustererConfig& config) const {
  std::size_t n = graph_.Graph()->n;

  SLPAClustererConfig slpa_config;
  config.any_config().UnpackTo(&slpa_config);
  int max_iteration = slpa_config.max_iteration();
  gbbs::uintE par_threshold = slpa_config.par_threshold();
  double prune_threshold = slpa_config.prune_threshold();
  bool remove_nested = slpa_config.remove_nested();

  auto memory = parlay::sequence<std::map<gbbs::uintE, size_t>>::from_function(n, [&] (size_t i) { return std::map<gbbs::uintE, size_t>{{i, 1}}; });
  
  // auto active_nodes = parlay::sequence<gbbs::uintE>::from_function(n, [&] (size_t i) { return i; });
  // auto is_active = parlay::sequence<bool>(n);
  
  for (int n_iterations = 0; n_iterations < max_iteration; n_iterations++) {

    parlay::parallel_for(0, n, [&] (gbbs::uintE node_id) {
      // auto node_id = active_nodes[i];

      // std::cout << "Node " << node_id << std::endl;
      auto degree =  graph_.Graph()->get_vertex(node_id).out_degree();
      gbbs::uintE heaviest;
      if(degree < par_threshold){
          // neighborLabelCounts maps label -> frequency in the neighbors
          std::map<gbbs::uintE, double> label_weights_sum;
          auto listen_f = [&] (const auto& u, const auto& v, const auto& wgh) {
            auto label = speak_sequential(v, memory[v], n_iterations + 1);
            label_weights_sum[label] += wgh;
          };
          graph_.Graph()->get_vertex(node_id).out_neighbors().map(listen_f, false);

          // for(auto [v, w]: label_weights_sum){
          //   std::cout << v << " " << w << "\n";
          // }

          heaviest = std::max_element(label_weights_sum.begin(), label_weights_sum.end(),
                                                  [](const std::pair<gbbs::uintE, double> &p1,
                                                    const std::pair<gbbs::uintE, double> &p2) {
                                                      return p1.second < p2.second && p1.first < p2.first;
                                                  })
                                    ->first;
      } else {
        auto neighbors = graph_.Graph()->get_vertex(node_id).out_neighbors();

        auto label_weights = parlay::delayed_seq<std::pair<gbbs::uintE, double>>(degree, [&](size_t i){
          auto [v, w] = neighbors.get_ith_neighbor(i);
          auto label = speak_sequential(v, memory[v], n_iterations);
          return std::make_pair(label, w);
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

      memory[node_id][heaviest]++;
    });


    } // end for loop

  // TODO: Postprocessing

  parlay::sequence<std::vector<std::pair<gbbs::uintE, gbbs::uintE>>> labels(n);
  prune_threshold *= max_iteration;
  parlay::parallel_for(0, n, [&] (size_t i) {
    for(const auto& kv: memory[i]){
      if(kv.second > prune_threshold) labels[i].push_back({kv.first, i});
    }
  });

  auto pairs = parlay::flatten(labels);
  parlay::sort_inplace(parlay::make_slice(pairs));

  // auto grouped = parlay::group_by_key(label_flatten);
  SLPAClusterer::Clustering output;
  // parlay::parallel_for(0, grouped.size(), [&] (size_t i) {
  //   ret[i] = std::vector(grouped.second.begin(), grouped.second.end());
  // });


  if(remove_nested){
    std::vector<std::set<NodeId>> sets;
    for (std::size_t i=0; i<pairs.size(); i++) {
      auto& cluster_i = pairs[i].first;
      if (i == 0 || cluster_i != pairs[i-1].first) {
        sets.emplace_back(std::set<NodeId>{pairs[i].second});
      } else {
        sets[sets.size()-1].insert(pairs[i].second);
      }
    }

    for (const auto& set1 : sets) {
        bool isSubset = false;
        for (const auto& set2 : sets) {
            if (&set1 != &set2 && std::includes(set2.begin(), set2.end(), set1.begin(), set1.end())) {
                isSubset = true;
                break;
            }
        }
        if (!isSubset) {
            output.push_back(std::vector(set1.begin(), set1.end()));
        }
    }
    
  } else {
    for (std::size_t i=0; i<pairs.size(); i++) {
      auto& cluster_i = pairs[i].first;
      if (i == 0 || cluster_i != pairs[i-1].first) {
        output.emplace_back(std::vector<NodeId>{pairs[i].second});
      } else {
        output[output.size()-1].emplace_back(pairs[i].second);
      }
    }
  }
  

  std::cout << "Num clusters = " << output.size() << std::endl;
  return output;
}

}  // namespace in_memory
}  // namespace research_graph
