#pragma once
#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_

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
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph::in_memory {

inline void split(const std::string& s, char delim, std::vector<InMemoryClusterer::NodeId>& elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::stoi(item));
  }
}

template <class T>
inline void set_distribution_stats(std::size_t size, const T& data, DistributionStatistics* distribution_stats){
  double sum = 0;
  gbbs::uintE min = UINT_E_MAX;
  gbbs::uintE max = 0;
  for (std::size_t i = 0; i < size; i++) {
    sum += data(i);
    if (data(i) < min) min = data(i);
    if (data(i) > max) max = data(i);
  }
  distribution_stats->set_count(size);
  distribution_stats->set_total(sum);
  distribution_stats->set_mean(sum / size);
  distribution_stats->set_minimum(min);
  distribution_stats->set_maximum(max);
}

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_STATS_UTILS_H_