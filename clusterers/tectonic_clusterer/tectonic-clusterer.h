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

#ifndef PARALLEL_CLUSTERING_CLUSTERERS_TECTONIC_CLUSTERER_H_
#define PARALLEL_CLUSTERING_CLUSTERERS_TECTONIC_CLUSTERER_H_

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
#include "external/gbbs/benchmarks/TriangleCounting/ShunTangwongsan15/Triangle.h"
#include "benchmarks/DegeneracyOrder/GoodrichPszona11/DegeneracyOrder.h"
#include "benchmarks/DegeneracyOrder/BarenboimElkin08/DegeneracyOrder.h"
#include "benchmarks/KCore/JulienneDBS17/KCore.h"
#include "external/gbbs/benchmarks/Connectivity/SimpleUnionAsync/Connectivity.h"

namespace gbbs {

template <
    template <class inner_wgh> class vtx_type, class wgh_type, typename P,
    typename std::enable_if<
        std::is_same<vtx_type<wgh_type>, symmetric_vertex<wgh_type>>::value,
        int>::type = 0>
static inline symmetric_graph<symmetric_vertex, wgh_type> filterGraph(
    symmetric_ptr_graph<vtx_type, wgh_type>& G, P& pred) {
  auto ret = filter_graph<vtx_type, wgh_type>(G, pred);
  auto newN = std::get<0>(ret);
  auto newM = std::get<1>(ret);
  auto newVData = std::get<2>(ret);
  auto newEdges = std::get<3>(ret);

  assert(newN == G.num_vertices());
  return symmetric_graph<symmetric_vertex, wgh_type>(
      newVData, newN, newM,
      [=]() {
        gbbs::free_array(newVData, newN);
        gbbs::free_array(newEdges, newM);
      },
      newEdges);
}

namespace intersection {

template <class SeqA, class SeqB, class F>
size_t seq_merge_full_idx(SeqA& A, SeqB& B, F& f, size_t offset_a, size_t offset_b, bool flip) {
  using T = typename SeqA::value_type;
  size_t nA = A.size(), nB = B.size();
  size_t i = 0, j = 0;
  size_t ct = 0;
  while (i < nA && j < nB) {
    const uintE& a = std::get<0>(A[i]);
    const uintE& b = std::get<0>(B[j]);
    if (a == b) {
      if (!flip) f(a, offset_a + i, offset_b + j);
      else f(a, offset_b + j, offset_a + i);
      i++;
      j++;
      ct++;
    } else if (a < b) {
      i++;
    } else {
      j++;
    }
  }
  return ct;
}

template <class SeqA, class SeqB, class F>
size_t seq_merge_idx(const SeqA& A, const SeqB& B, const F& f, size_t offset_a, size_t offset_b, bool flip) {
  using T = typename SeqA::value_type;
  size_t nA = A.size();
  size_t ct = 0;
  for (size_t i = 0; i < nA; i++) {
    const uintE& a = std::get<0>(A[i]);
    std::tuple<uintE, float> search = std::make_tuple(a, static_cast<float>(0));
    auto less_tuple = [](const T& x, const T& y){
      return std::get<0>(x) < std::get<0>(y);
    };
    size_t mB = parlay::binary_search(B, search, less_tuple);
    if (mB < B.size() && a == std::get<0>(B[mB])) {
      if (!flip) f(a, offset_a + i, offset_b + mB);
      else f(a, offset_b + mB, offset_a + i);
      ct++;
    }
  }
  return ct;
}

template <class SeqA, class SeqB, class F>
size_t merge_idx(const SeqA& A, const SeqB& B, const F& f, size_t offset_a, size_t offset_b, bool flip) {
  using T = typename SeqA::value_type;
  size_t nA = A.size();
  size_t nB = B.size();
  size_t nR = nA + nB;
  if (nR < _seq_merge_thresh) {  // handles (small, small) using linear-merge
    return intersection::seq_merge_full_idx(A, B, f, offset_a, offset_b, flip);
  } else if (nB < nA) {
    return intersection::merge_idx(B, A, f, offset_b, offset_a, !flip);
  } else if (nA < _bs_merge_base) {
    return intersection::seq_merge_idx(A, B, f, offset_a, offset_b, flip);
  } else {
    size_t mA = nA / 2;
    size_t mB = parlay::binary_search(B, A[mA], std::less<T>());
    size_t m_left = 0;
    size_t m_right = 0;
    par_do(
        [&]() { m_left = intersection::merge_idx(A.cut(0, mA), B.cut(0, mB), f, offset_a, offset_b, flip); },
        [&]() {
          m_right = intersection::merge_idx(A.cut(mA, nA), B.cut(mB, nB), f, offset_a + mA, offset_b + mB, flip);
        });
    return m_left + m_right;
  }
}

template <class Nghs, class F>
inline size_t intersect_f_par_idx(Nghs* A, Nghs* B, const F& f) {
  using EdgeType = std::tuple<uintE, float>;
  uintT nA = A->degree, nB = B->degree;
  EdgeType* nghA = (EdgeType*)(A->neighbors);
  EdgeType* nghB = (EdgeType*)(B->neighbors);

  // Will not work if W is not gbbs::empty, should assert.
  auto seqA = gbbs::make_slice<EdgeType>(nghA, nA);
  auto seqB = gbbs::make_slice<EdgeType>(nghB, nB);

  uintE a = A->id;
  uintE b = B->id;
  auto merge_f = [&](uintE ngh, size_t a_idx, size_t b_idx) { f(ngh, a_idx, b_idx); };
  return intersection::merge_idx(seqA, seqB, merge_f, 0, 0, false);
}

}  // namespace intersection
  template <class Graph, class F>
  inline size_t CountDirectedBalancedEdge(Graph& DG, size_t* counts, const F& f, sequence<uintE>& offset) {
  using W = typename Graph::weight_type;

  size_t n = DG.n;

  auto parallel_work = sequence<size_t>::uninitialized(n);
  {
    auto map_f = [&](uintE u, uintE v, W wgh) -> size_t {
      return DG.get_vertex(v).out_degree();
    };
    parallel_for(0, n, [&](size_t i) {
      auto monoid = parlay::addm<size_t>();
      parallel_work[i] = DG.get_vertex(i).out_neighbors().reduce(map_f, monoid);
    });
  }
  size_t total_work = parlay::scan_inplace(make_slice(parallel_work));

  size_t block_size = 50000;
  size_t n_blocks = total_work / block_size + 1;
  size_t work_per_block = (total_work + n_blocks - 1) / n_blocks;

  auto run_intersection = [&](size_t start_ind, size_t end_ind) {
    for (size_t i = start_ind; i < end_ind; i++) {  // check LEQ
      auto our_neighbors = DG.get_vertex(i).out_neighbors();
      size_t total_ct = 0;
      size_t iv_index = 0;
      auto map_f = [&](uintE u, uintE v, W wgh) {
        // Edge i -> v = u -> v = offset[i] + iv_index
        // Edge i -> w = u -> w = offset[i] + u_idx
        // Edge v -> w = offset[v] + v_idx
        // f should be (uintE w, u_idx, v_idx)
        auto their_neighbors = DG.get_vertex(v).out_neighbors();
        auto f_tmp = [&](uintE w, size_t u_idx, size_t v_idx){
          // TODO(jeshi): fix offset
          f(offset[i] + iv_index, offset[i] + u_idx, offset[v] + v_idx);
        };
        total_ct += intersection::intersect_f_par_idx(&our_neighbors, &their_neighbors, f_tmp);
        iv_index++;
      };
      our_neighbors.map(map_f, false);  // run map sequentially
      counts[i] = total_ct;
    }
  };

  parallel_for(0, n_blocks, 1, [&](size_t i) {
    size_t start = i * work_per_block;
    size_t end = (i + 1) * work_per_block;
    auto less_fn = std::less<size_t>();
    size_t start_ind = parlay::binary_search(parallel_work, start, less_fn);
    size_t end_ind = parlay::binary_search(parallel_work, end, less_fn);
    run_intersection(start_ind, end_ind);
  });

  auto count_seq = gbbs::make_slice<size_t>(counts, DG.n);
  size_t count = parlay::reduce(count_seq);

  return count;
}


template <class Graph, class DirectedGraph>
inline sequence<uintE> Triangle_union_find(Graph& G, DirectedGraph& DG, 
  sequence<uintE>& triangle_degrees, double threshold, sequence<uintE>& offset){

  auto clusters = parlay::sequence<gbbs::uintE>::from_function(G.n, [&] (size_t i) { return i; });
  parlay::parallel_for(0, G.n, [&] (size_t i) {
    size_t v_index = 0;
    auto map_f = [&] (const auto& u, const auto& v, const auto& wgh) {
      if (static_cast<double>(triangle_degrees[offset[i] + v_index]) / static_cast<double>(G.get_vertex(u).out_degree() + G.get_vertex(v).out_degree()) >= threshold) {
        gbbs::simple_union_find::unite_impl(u, v, clusters);
      }
      v_index++;
    };
    DG.get_vertex(i).out_neighbors().map(map_f, false);
  });

  parlay::parallel_for(0, G.n, [&] (size_t i) {
    gbbs::simple_union_find::find_compress(i, clusters);
  });

  return clusters;

}

template <class Graph>
inline sequence<uintE> Triangle_degree_ordering_edge(Graph& G, double threshold) {
  using W = typename Graph::weight_type;
  timer gt;
  gt.start();
  uintT n = G.n;
  auto counts = sequence<size_t>::uninitialized(n);
  parallel_for(0, n, kDefaultGranularity, [&](size_t i) { counts[i] = 0; });

  // 1. Rank vertices based on degree
  timer rt;
  rt.start();
  uintE* rank = rankNodes(G, G.n);
  rt.stop();
  //rt.next("rank time");

  // 2. Direct edges to point from lower to higher rank vertices.
  // Note that we currently only store out-neighbors for this graph to save
  // memory.
  auto pack_predicate = [&](const uintE& u, const uintE& v, const W& wgh) {
    return rank[u] < rank[v];
  };
  auto DG = filterGraph(G, pack_predicate);
  // auto DG = Graph::filterGraph(G, pack_predicate);
  gt.stop();
  //gt.next("build graph time");

  // 3. Count triangles on the digraph
  timer ct;
  ct.start();

  parlay::sequence<gbbs::uintE> triangle_degrees = parlay::sequence<gbbs::uintE>::from_function(
    DG.m, [](std::size_t i){return 0;});
  auto f = [&](size_t uv, size_t uw, size_t vw){
    // Add to triangle_degrees() --> but remember it is on the directed graph
    // TODO(jeshi): Is there something better we can do than write_add?
    gbbs::write_add(&triangle_degrees[uv], 1);
    gbbs::write_add(&triangle_degrees[uw], 1);
    gbbs::write_add(&triangle_degrees[vw], 1);
  };

  sequence<uintE> offset = sequence<uintE>::from_function(n, [&](size_t i){
    return DG.get_vertex(i).out_degree();
  });
  parlay::scan_inplace(make_slice(offset));

  size_t count = CountDirectedBalancedEdge(DG, counts.begin(), f, offset);
  std::cout << "### Num triangles = " << count << "\n";
  ct.stop();
  //ct.next("count time");
  gbbs::free_array(rank, G.n);
  return Triangle_union_find(G, DG, triangle_degrees, threshold, offset);
}

template <class Graph, class O>
inline sequence<uintE> Triangle_degeneracy_ordering_edge(Graph& G, double threshold,
                                           O ordering_fn) {
  using W = typename Graph::weight_type;
  timer gt;
  gt.start();
  uintT n = G.n;
  auto counts = sequence<size_t>::uninitialized(n);
  parallel_for(0, n, kDefaultGranularity, [&](size_t i) { counts[i] = 0; });

  timer rt;
  rt.start();
  auto ordering = ordering_fn(G);
  rt.stop();
  //rt.next("rank time");
  auto pack_predicate = [&](const uintE& u, const uintE& v, const W& wgh) {
    return (ordering[u] < ordering[v]);
  };

  auto DG = filterGraph(G, pack_predicate);
  // auto DG = Graph::filterGraph(G, pack_predicate);
  gt.stop();
  //gt.next("build graph time");

  // 3. Count triangles on the digraph
  timer ct;
  ct.start();

  parlay::sequence<gbbs::uintE> triangle_degrees = parlay::sequence<gbbs::uintE>::from_function(
    DG.m, [](std::size_t i){return 0;});
  auto f = [&](size_t uv, size_t uw, size_t vw){
    // Add to triangle_degrees() --> but remember it is on the directed graph
    // TODO(jeshi): Is there something better we can do than write_add?
    gbbs::write_add(&triangle_degrees[uv], 1);
    gbbs::write_add(&triangle_degrees[uw], 1);
    gbbs::write_add(&triangle_degrees[vw], 1);
  };

  sequence<uintE> offset = sequence<uintE>::from_function(n, [&](size_t i){
    return DG.get_vertex(i).out_degree();
  });
  parlay::scan_inplace(make_slice(offset));

  size_t count = CountDirectedBalancedEdge(DG, counts.begin(), f, offset);
  std::cout << "### Num triangles = " << count << "\n";
  ct.stop();
  //ct.next("count time");
  return Triangle_union_find(G, DG, triangle_degrees, threshold, offset);
}

}  // namespace gbbs

namespace research_graph {
namespace in_memory {

class TectonicClusterer : public InMemoryClusterer {
 public:
  Graph* MutableGraph() override { return &graph_; }

  absl::StatusOr<Clustering> Cluster(
      const ClustererConfig& config) const override;

 private:
  GbbsGraph graph_;
};

}  // namespace in_memory
}  // namespace research_graph

#endif  // PARALLEL_CLUSTERING_CLUSTERERS_KCORE_CLUSTERER_H_
