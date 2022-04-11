#include "clusterers/hac_euclidean_clusterer/hac-euclidean-clusterer.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/in-memory-metric-clusterer-base.h"
#include "parcluster/api/status_macros.h"

#include "clusterer.h"
#include "utils/point.h"
#include "parlay/sequence.h"
#include "dist.h"

namespace research_graph {
namespace in_memory {
namespace internal{
namespace HACTree{

  template<int dim>
  parlay::sequence<iPoint<dim>> getPoints(absl::Span<const DataPoint> datapoints){
    std::size_t n = datapoints.size();
    parlay::sequence<iPoint<dim>> P = parlay::sequence<iPoint<dim>>(n);
    parlay::parallel_for(0, n, [&](size_t i){
      for (std::size_t j=0; j<datapoints[i].coordinates.size(); j++) {
          P[i][j] = datapoints[i].coordinates[j];
      }
      P[i].idx(i);
    });
    return P;
  }
} //end of namespace HACTree
  template<int dim>
  vector<HACTree::dendroLine> runComplete(absl::Span<const DataPoint> datapoints, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = datapoints.size();
    auto P = HACTree::getPoints<dim>(datapoints); // copy points
    return runCompleteHAC(P, no_cache, cache_size, eps);
  }

  template<int dim>
  vector<HACTree::dendroLine> runAVG(absl::Span<const DataPoint> datapoints, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = datapoints.size();
    auto P = HACTree::getPoints<dim>(datapoints); // copy points
    return runAVGHAC(P, no_cache, cache_size, eps);
  }

  template<int dim>
  vector<HACTree::dendroLine> runAVGSQ(absl::Span<const DataPoint> datapoints, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = datapoints.size();
    auto P = HACTree::getPoints<dim>(datapoints); // copy points
    return runAVGSQHAC(P, no_cache, cache_size, eps);
  }

  template<int dim>
  vector<HACTree::dendroLine> runWARD(absl::Span<const DataPoint> datapoints, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = datapoints.size();
    auto P = HACTree::getPoints<dim>(datapoints); // copy points
    return runWARDHAC(P, no_cache, cache_size, eps);
  }

} //end of namespace internal

absl::StatusOr<std::vector<int64_t>> HACEuclideanClusterer::Cluster(
      absl::Span<const DataPoint> datapoints,
      const MetricClustererConfig& config) const {
  
  const HACClustererConfig& hac_config = config.hac_clusterer_config();
  const HACClustererConfig_LinkageMethod linkage_method = hac_config.linkage_method();
  const HACClustererConfig_Distance distance = hac_config.distance();
  const string output_dendro = hac_config.output_dendro();
  //TODO: add eps and cache size to config
  bool no_cache = true;
  std::size_t n = datapoints.size();
  if(n < 1){
    std::cerr << "empty dataset " << std::endl;
    return absl::InvalidArgumentError("Empty dataset");
  }
  int dim = datapoints[0].coordinates.size();
  if(dim !=2){
    return absl::UnimplementedError("dim > 2 hasn't implemented");
  }
// 
  vector<internal::HACTree::dendroLine> dendro;
  
  if(linkage_method== HACClustererConfig::COMPLETE && distance == HACClustererConfig::EUCLIDEAN ){
    std::cout << "Linkage method: " << "complete linkage , euclidean distance" << std::endl;
    dendro = internal::runComplete<2>(datapoints, no_cache );
  }else if(linkage_method== HACClustererConfig::AVERAGE &&  distance == HACClustererConfig::EUCLIDEAN ){
    std::cout << "Linkage method: " << "average linkage, euclidean distance" << std::endl;
    dendro = internal::runAVG<2>(datapoints, no_cache);
  }else if(linkage_method== HACClustererConfig::AVERAGE &&  distance == HACClustererConfig::EUCLIDEAN_SQ){
    std::cout << "Linkage method: " << "average linkage, squared euclidean distance" << std::endl;
    dendro = internal::runAVGSQ<2>(datapoints, no_cache);
  }else if(linkage_method== HACClustererConfig::WARD &&  distance == HACClustererConfig::EUCLIDEAN ){
    std::cout << "Linkage method: " << "Ward's linkage, euclidean distance" << std::endl;
    dendro = internal::runWARD<2>(datapoints, no_cache);
  }else{
    std::cerr << "Linkage method = " << linkage_method << std::endl;
    std::cerr << "Distance = " << distance << std::endl;
    return absl::UnimplementedError("Unknown linkage method and distnace metric.");
  }

  if(output_dendro != ""){
    std::cout << "dednrogram output file: " << output_dendro << std::endl;
    ofstream file_obj;
    file_obj.open(output_dendro.c_str()); 
    for(size_t i=0;i<n-1;i++){
        dendro[i].print(file_obj);
    }
    file_obj.close();
  }

  double checksum = parlay::reduce(parlay::delayed_seq<double>(n-1, [&](size_t i){return dendro[i].height;}));
  cout << "Cost: " << std::setprecision(10)  << checksum << endl;

  // Initially each vertex is its own cluster.
  std::vector<int64_t> cluster_ids(n);
  parlay::parallel_for(0, n, [&](std::size_t i) { cluster_ids[i] = i; });

  std::cout << "Num clusters = " << cluster_ids.size() << std::endl;
  return cluster_ids;
}

}  // namespace in_memory
}  // namespace research_graph
