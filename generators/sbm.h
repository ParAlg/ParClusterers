#pragma once
#include <chrono>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "gbbs/macros.h"
#include "gbbs/bridge.h"


namespace {
  double hashDouble(gbbs::uintE i) {
    return ((double)(parlay::hash32(i)) / ((double)UINT_E_MAX));
  }
}
namespace parlay {
template <class A, class B>
inline void type_to_string_tab(char* s, std::tuple<A, B> a) {
  int l = t_to_stringlen(std::get<0>(a));
  type_to_string(s, std::get<0>(a));
  s[l] = '\t';
  type_to_string(s + l + 1, std::get<1>(a));
}

template <class TSeq>
sequence<char> sequence_to_string_tab(TSeq const& T) {
  size_t n = T.size();
  auto S = sequence<size_t>::from_function(n, [&](size_t i) {
    return t_to_stringlen(T[i]) + 1;  // +1 for \n
  });
  size_t m = parlay::scan_inplace(make_slice(S), plus<size_t>());

  auto C = sequence<char>::from_function(m, [&](size_t i) { return (char)0; });
  parallel_for(0, n - 1, [&](size_t i) {
    type_to_string_tab(C.begin() + S[i], T[i]);
    C[S[i + 1] - 1] = '\n';
  });
  type_to_string_tab(C.begin() + S[n - 1], T[n - 1]);
  C[m - 1] = '\n';
  // for(char c: C){
  //   std::cout << c;
  // }
  return parlay::filter(make_slice(C), [&](char A) { return A > 0; });
}
}

namespace research_graph::in_memory {


parlay::sequence<std::tuple<gbbs::uintE, gbbs::uintE>> SBM(gbbs::uintE n, std::vector<gbbs::uintE>& membership, 
                                                std::vector<std::vector<double>>& affinity){
  using uintE = gbbs::uintE;
  uintE m = n * n;

  auto E = parlay::sequence<std::tuple<uintE, uintE>>(m, std::make_tuple(UINT_E_MAX, UINT_E_MAX));
  parlay::parallel_for(0, n, [&](uintE u){
    parlay::parallel_for(0, u, [&](uintE v){
        uintE a = membership.at(u);
        uintE b = membership.at(v);
        assert(a < affinity.size());
        assert(b < affinity.size());
        double p = affinity.at(a).at(b);
        if (p > 0){
          auto hash1 = parlay::hash64(u);
          auto hash2 = parlay::hash64(v);
          parlay::hash_combine(hash1, hash2);
          double r = hashDouble(hash1);
          if (r <= p) {
            E[u*n + v] = std::make_tuple(u, v);
            // std::cout << u << " " << v << std::endl;
          }
        }
    });
  });

  E = parlay::filter(E, [](const std::tuple<uintE, uintE>& e){
    if(std::get<0>(e) == UINT_E_MAX) return false;
    return true;
    });

  return E;
}
}