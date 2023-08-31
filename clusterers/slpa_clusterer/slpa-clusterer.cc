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



gbbs::uintE speak_sequential(const gbbs::uintE v, const std::map<gbbs::uintE, std::size_t>& memory, const std::size_t m, int seed){
    // std::random_device rd;
    // std::mt19937 gen(seed);
    // std::uniform_int_distribution<size_t> dist(0, m - 1);
    // size_t rnd = dist(gen);
    size_t rnd = parlay::hash64(seed + static_cast<std::size_t>(v) + m) % m;
    // std::cout << "speaker: " << v << "\n";
    // std::cout << "rnd: " << rnd << "\n";
    // std::cout << "m: " << m << "\n";

    for (const auto& kv : memory) {
        if (rnd < kv.second){
            // std::cout << " return: k " << kv.first << " v " << kv.second << "\n";
            return kv.first;
        }
        rnd -= kv.second;
    }
    return 0; // should never reach this line
}



SLPAClusterer::Clustering SLPAClusterer::findMaximalSets(std::vector<std::set<gbbs::uintE>>& sets) const {
    std::vector<bool> flags(sets.size(), true);
      double max_d = 0;
    auto start = std::chrono::high_resolution_clock::now();
    parlay::parallel_for(0, sets.size(), [&](size_t i){
        const auto& set = sets[i];
        bool isMaximal = true;
        parlay::parallel_for(0, sets.size(), [&](size_t j){
            if (isMaximal && i != j){
              //
              const auto& otherSet = sets[j];
              if(std::includes(otherSet.begin(), otherSet.end(), set.begin(), set.end())) {
                if (set.size() == otherSet.size()){ // same set
                  isMaximal = i < j;
                } else {
                  isMaximal = false;
                }
              }
              // 

            }
        });
        flags[i] = isMaximal;
    });
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // std::cout << "Time taken by function: " << duration.count() / 1e6 << " seconds" << std::endl;

    SLPAClusterer::Clustering maximalSets;
    for (int i=0;i<sets.size();++i){
      if(flags[i]) {
          maximalSets.push_back(std::vector<gbbs::uintE>(sets[i].begin(), sets[i].end()));
        }
    }
    return maximalSets;
}

SLPAClusterer::Clustering SLPAClusterer::postprocessing(const parlay::sequence<std::map<gbbs::uintE, size_t>>& memory, bool remove_nested, double prune_threshold, int total_n) const {
  std::size_t n = memory.size();
  parlay::sequence<std::vector<std::pair<gbbs::uintE, gbbs::uintE>>> labels(n);
  parlay::parallel_for(0, n, [&] (size_t i) {
    // double total = 0;
    // for(const auto& kv: memory[i]){
    //   total += kv.second;
    // }
    // assert(total == total_n);
    for(const auto& kv: memory[i]){
      if(kv.second > prune_threshold * total_n) labels[i].push_back({kv.first, i});
    }
    // if (labels[i].size() > 1){
    //   std::cout << "label size larger than 1\n";
    // }
  });

  auto pairs = parlay::flatten(labels); // cluster id, node id
  parlay::sort_inplace(parlay::make_slice(pairs));

  // std::cout << "pairs: \n";
  // for(auto [k,v]: pairs){
  //   std::cout << "k " << k << " v " << v << "\n";
  // }

  // auto grouped = parlay::group_by_key(label_flatten);
  SLPAClusterer::Clustering output;
  // parlay::parallel_for(0, grouped.size(), [&] (size_t i) {
  //   ret[i] = std::vector(grouped.second.begin(), grouped.second.end());
  // });

  if(remove_nested){
    std::vector<std::set<gbbs::uintE>> sets;
    for (std::size_t i=0; i<pairs.size(); i++) {
      auto& cluster_i = pairs[i].first;
      if (i == 0 || cluster_i != pairs[i-1].first) {
        sets.emplace_back(std::set<gbbs::uintE>{pairs[i].second});
      } else {
        sets[sets.size()-1].insert(pairs[i].second);
      }
    }

    std::cout << "Num. clusters before removing: " << sets.size() << std::endl;
    
    output = findMaximalSets(sets);
    // output.resize(maximal_sets.size());
    // parlay::parallel_for(0, maximal_sets.size(), [&](size_t i){
    //   const auto& s = maximal_sets[i];
    //   output[i] = std::vector<gbbs::uintE>(s.begin(), s.end())
    // });
    
  } else {
    // for (std::size_t i=0; i<pairs.size(); i++) {
    //   auto& cluster_i = pairs[i].first;
    //   if (i == 0 || cluster_i != pairs[i-1].first) {
    //     output.emplace_back(std::vector<gbbs::uintE>{pairs[i].second});
    //   } else {
    //     output[output.size()-1].emplace_back(pairs[i].second);
    //   }
    // }
    // std::cout << "[ ";
    // for (const auto& p : pairs) {
    //     std::cout << "(" << p.first << ", " << p.second << ") ";
    // }
    // std::cout << "]" << std::endl;

    std::size_t n = pairs.size();
    parlay::sequence<int>prefix_sums(n, 0);

    // 1. Compute the binary prefix_sums array.
    parlay::parallel_for(1, n, [&](std::size_t i) {
        if (pairs[i].first != pairs[i-1].first) {
            prefix_sums[i] = 1;
        }
    });
    prefix_sums[0] = 1; // The first cluster always starts a new sequence.

    // 2. Compute the prefix sum.
    parlay::scan_inclusive_inplace(prefix_sums);
    prefix_sums.push_back(prefix_sums[n-1]+1);

    // std::cout << "[ ";
    // for (const auto& elem : prefix_sums) {
    //     std::cout << elem << " ";
    // }
    // std::cout << "]" << std::endl;

    // 3. Allocate space for the output.
    output.resize(prefix_sums[n-1]); // Total number of unique clusters.
    parlay::sequence<int> start_ids(prefix_sums[n]);

    // 4. Fill the output in parallel.
    parlay::parallel_for(0, n, [&](std::size_t i) {
        if (i==0 || prefix_sums[i] != prefix_sums[i-1]) {
            start_ids[prefix_sums[i]] = i;
        }
    });
    start_ids.push_back(n);

    // std::cout << "[ ";
    // for (const auto& elem : start_ids) {
    //     std::cout << elem << " ";
    // }
    // std::cout << "]" << std::endl;

    parlay::parallel_for(0, n, [&](std::size_t i) {
        std::size_t pos = prefix_sums[i] - 1;
        if (i==0 || prefix_sums[i] != prefix_sums[i-1]) {
            std::size_t start_point = start_ids[prefix_sums[i]];
            std::size_t cluster_size = start_ids[prefix_sums[i]+1] - start_point;
            output[pos].resize(cluster_size);
            parlay::parallel_for(0, cluster_size, [&](std::size_t j){
              output[pos][j] = pairs[start_point + j].second;
            });
        }
    });


  }
  return output;
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

  std::cout << "max_iteration: " << max_iteration << std::endl;
  std::cout << "par_threshold: " << par_threshold << std::endl;
  std::cout << "prune_threshold: " << prune_threshold << std::endl;
  std::cout << "remove_nested: " << (remove_nested ? "true" : "false") << std::endl;

  int seed = slpa_config.seed();

  auto memory = parlay::sequence<std::map<gbbs::uintE, size_t>>::from_function(n, [&] (size_t i) { return std::map<gbbs::uintE, size_t>{{i, 1}}; });
  
  for (int n_iterations = 0; n_iterations < max_iteration; n_iterations++) {
      // std::cout << "Round " << n_iterations << std::endl;

    parlay::parallel_for(0, n, [&] (gbbs::uintE node_id) {
      // auto node_id = active_nodes[i];

      // std::cout << "Node " << node_id << std::endl;
      auto degree =  graph_.Graph()->get_vertex(node_id).out_degree();
      gbbs::uintE heaviest;
      if(degree == 0){
          heaviest = node_id;
      } else {
      // } else if(degree < par_threshold){
          // neighborLabelCounts maps label -> frequency in the neighbors
          std::map<gbbs::uintE, double> label_weights_sum;
          auto listen_f = [&] (const auto& u, const auto& v, const auto& wgh) {
            auto label = speak_sequential(v, memory[v], n_iterations + 1, seed);
            label_weights_sum[label] += wgh;
          };
          graph_.Graph()->get_vertex(node_id).out_neighbors().map(listen_f, false);

          // std::cout << " label_weights_sum: \n";
          // for(auto [v, w]: label_weights_sum){
          //   std::cout << v << " " << w << "\n";
          // }

          heaviest = std::max_element(label_weights_sum.begin(), label_weights_sum.end(),
                                                  [](const std::pair<gbbs::uintE, double> &p1,
                                                    const std::pair<gbbs::uintE, double> &p2) {
                                                      return p1.second < p2.second || (p1.second == p2.second && p1.first < p2.first);
                                                  })
                                    ->first;
      // } else {
      //   auto neighbors = graph_.Graph()->get_vertex(node_id).out_neighbors();

      //   auto label_weights = parlay::delayed_seq<std::pair<gbbs::uintE, double>>(degree, [&](size_t i){
      //     auto [v, w] = neighbors.get_ith_neighbor(i);
      //     auto label = speak_sequential(v, memory[v], n_iterations + 1, seed);
      //     return std::make_pair(label, w);
      //   });

      //   auto grouped = parlay::group_by_key(label_weights);
      //   auto label_weights_sum = parlay::sequence<std::pair<double, gbbs::uintE>>(grouped.size());
      //   parlay::parallel_for(0, grouped.size(), [&](gbbs::uintE i){
      //     auto cluster_id = grouped[i].first;
      //     auto w = parlay::reduce(grouped[i].second);
      //     label_weights_sum[i] = {w, cluster_id};
      //   });

      //   heaviest = parlay::max_element(label_weights_sum)->second;
      }

      // std::cout << "node_id " << node_id << " heaviest " << heaviest << "\n";
      memory[node_id][heaviest]++;
    });


    } // end for loop
  std::cout << "Num iterations: " << max_iteration << std::endl;
  std::cout << "postprocessing" << "\n";
  auto output = postprocessing(memory, remove_nested, prune_threshold, max_iteration + 1);

  std::cout << "Num clusters = " << output.size() << std::endl;
  return output;
}

}  // namespace in_memory
}  // namespace research_graph
