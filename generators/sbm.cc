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

#include "gbbs/macros.h"
#include "gbbs/bridge.h"

ABSL_FLAG(std::string, output_file, "",
          "Output file");

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
  
absl::Status Main() {
  std::string out_f = absl::GetFlag(FLAGS_output_file);
  using uintE = gbbs::uintE;
  uintE n = 5;
  uintE m = n * n;
  std::vector<uintE> membership{0, 0, 0, 1, 1};
   std::vector<std::vector<double>> affinity{{1, 0},
                                              {0, 1}};

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
            std::cout << u << " " << v << std::endl;
          }
        }
    });
  });

  E = parlay::filter(E, [](const std::tuple<uintE, uintE>& e){
    if(std::get<0>(e) == UINT_E_MAX) return false;
    return true;
    });

  std::cout << "Num edges: " << E.size() << std::endl;
  auto C = parlay::sequence_to_string_tab(E);
  size_t nn = C.size();
  std::ofstream file(out_f.c_str(), std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    std::cout << "Unable to open file for writing: " << out_f << std::endl;
    return absl::InternalError("Unable to open file");
  }
   file << "# COO Format" << std::endl;
   file << "# n = " << n << std::endl;
   file << "# m = " << E.size() << std::endl;

  file.write(C.begin(), nn);
  file.close();

  std::cout << "done" << std::endl;
  return absl::OkStatus();
}
}


int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);
  auto status = research_graph::in_memory::Main();
  if (!status.ok()) {
    std::cerr << status << std::endl;
    return EXIT_FAILURE;
  }
}