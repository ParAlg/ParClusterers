#include "rmat.h"

#include "gbbs/gbbs.h"

#include <fstream>
#include <iostream>

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

namespace gbbs {

int BuildRMAT(int argc, char* argv[]) {
  commandLine P(argc, argv, "");

  size_t n = P.getOptionLongValue("-n", 1UL << 20);
  size_t m = P.getOptionLongValue("-m", 1UL << 23);

  double a = P.getOptionDoubleValue("-a", 0.5);
  double b = P.getOptionDoubleValue("-b", 0.1);
  double c = P.getOptionDoubleValue("-c", 0.1);

  auto out_f = P.getOptionValue("-outfile", "");

  if (out_f == "") {
    std::cout << "specify a valid outfile using -outfile" << std::endl;
    abort();
  }

  uintE seed = 4;
  std::cout << "Generating updates" << std::endl;
  auto updates = rmat::generate_updates(n, m, seed, a, b, c);
  std::cout << "Generated updates" << std::endl;

  if (n != (1UL << (parlay::log2_up(n)))) {
    std::cout << "n must be a power of two" << std::endl;
    abort();
  }

  std::cout << "Num updates: " << updates.size() << std::endl;
  // Remove duplicates
  updates = parlay::filter(updates, [](std::tuple<uintE, uintE>& e){
    if (std::get<0>(e) == std::get<1>(e)) return false;
    return true;
  });
  auto updates_ordered = parlay::delayed_seq<std::tuple<uintE, uintE>>(updates.size(), [&updates](size_t i){
    auto [u, v] = updates[i];
    if(u < v) return updates[i];
    return std::make_tuple(v, u);
  });
  updates = parlay::remove_duplicates_ordered(updates_ordered);
  std::cout << "Num unique updates: " << updates.size() << std::endl;


  auto C = parlay::sequence_to_string_tab(updates);
  std::cout << "======== first 5 edges\n";
  for (size_t i = 0; i < 5; i++) {
    std::cout << std::get<0>(updates[i]) << "\t" << std::get<1>(updates[i])
              << std::endl;
  }

  size_t nn = C.size();
  std::ofstream file(out_f.c_str(), std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    std::cout << "Unable to open file for writing: " << out_f << std::endl;
    return -1;
  }
   file << "# COO Format" << std::endl;
   file << "# n = " << n << std::endl;
   file << "# m = " << updates.size() << std::endl;

  file.write(C.begin(), nn);
  file.close();

  std::cout << "done" << std::endl;
  return 0;
}

}  // namespace gbbs

int main(int argc, char* argv[]) { return gbbs::BuildRMAT(argc, argv); }
