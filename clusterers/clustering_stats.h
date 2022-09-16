#ifndef RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_
#define RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_

#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"

#include "google/protobuf/text_format.h"
#include "clusterers/clustering_stats.pb.h"
#include "clusterers/stats/stats_communities.h"
#include "clusterers/stats/stats_correlation.h"
#include "clusterers/stats/stats_diameter.h"
#include "clusterers/stats/stats_density.h"
#include "parcluster/api/gbbs-graph.h"
#include "parcluster/api/in-memory-clusterer-base.h"
#include "parcluster/api/status_macros.h"

namespace research_graph::in_memory {

ClusteringStatistics GetStats(const GbbsGraph& graph, const InMemoryClusterer::Clustering& clustering,
  const std::string& input_graph, const std::string& input_communities,
  const ClusteringStatsConfig& clustering_stats_config);

}  // namespace research_graph::in_memory

#endif  // RESEARCH_GRAPH_IN_MEMORY_CLUSTERING_CLUSTERING_STATS_H_