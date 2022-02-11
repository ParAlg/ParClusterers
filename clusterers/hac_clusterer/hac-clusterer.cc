#include "clusterers/hac_clusterer/hac-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/in-memory-metric-clusterer-base.h"
#include "parcluster/api/status_macros.h"

#include "linkage.h"

namespace research_graph {
namespace in_memory {

absl::StatusOr<std::vector<int64_t>> HACClusterer::Cluster(
      absl::Span<const DataPoint> datapoints,
      const MetricClustererConfig& config) const {
  
  const HACClustererConfig& hac_config = config.hac_clusterer_config();
  const HACClustererConfig_LinkageMethod linkage_method = hac_config.linkage_method();
  std::size_t n = datapoints.size();
      
  using T=double;

  internal::SymMatrix<T> W = internal::getDistanceMatrix<T>(datapoints); 
  vector<internal::dendroLine> dendro;


  if(linkage_method== HACClustererConfig::COMPLETE){
    std::cout << "Linkage method: " << "complete linkage" << std::endl;
    using distT = internal::distComplete<T>;
    dendro = internal::chain_linkage_matrix<T, distT>(&W);
  }else if(linkage_method== HACClustererConfig::AVERAGE){
    std::cout << "Linkage method: " << "average linkage" << std::endl;
    using distT = internal::distAverage<T>;
    dendro = internal::chain_linkage_matrix<T, distT>(&W);
  }else{ //should not reach here if all methods in proto are implemented
    std::cerr << "Linkage method = " << linkage_method << std::endl;
    return absl::UnimplementedError("Unknown linkage method.");
  }

  // ofstream file_obj;
  // file_obj.open(output); 
  // for(size_t i=0;i<n-1;i++){
  //     dendro[i].print(file_obj);
  // }
  // file_obj.close();

  // Initially each vertex is its own cluster.
  std::vector<int64_t> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  std::cout << "Num clusters = " << cluster_ids.size() << std::endl;
  return cluster_ids;
}

}  // namespace in_memory
}  // namespace research_graph
