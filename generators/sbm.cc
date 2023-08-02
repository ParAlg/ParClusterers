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

#include "sbm.h"

ABSL_FLAG(std::string, output_file, "",
          "Output file");

ABSL_FLAG(std::string, cmty_output_file, "",
          "Output file for community defaults");

ABSL_FLAG(double, p_in, 1.0,
          "Probability inside community");

ABSL_FLAG(double, p_out, 0.0,
          "Probability between communities");


namespace{

template<class Seq>
void printVec(Seq& E){
    for(auto e : E){
      std::cout << e << std::endl;
    }
}

template<class M>
void printMatrix(M& affinity){
  auto nBlocks = affinity.size();
  for (int i = 0; i < nBlocks; ++i) {
      for (int j = 0; j < affinity[i].size(); ++j) {
          std::cout << affinity[i][j] << "\t";
      }
      std::cout << std::endl;
  }

}

absl::Status WriteClustering(const char* filename,
                             std::vector<std::vector<int>>& clustering) {
  std::ofstream file{filename};
  if (!file.is_open()) {
    return absl::NotFoundError("Unable to open file.");
  }
  for (gbbs::uintE i = 0; i < clustering.size(); i++) {
    for (auto node_id : clustering[i]) {
      file << node_id << "\t";
    }
    file << std::endl;
  }
  return absl::OkStatus();
}

}

namespace research_graph::in_memory {
  
absl::Status Main() {
  std::string out_f = absl::GetFlag(FLAGS_output_file);
  std::string out_f_cmty = absl::GetFlag(FLAGS_cmty_output_file);
  double p_in = absl::GetFlag(FLAGS_p_in);
  double p_out = absl::GetFlag(FLAGS_p_out);

  using uintE = gbbs::uintE;
  uintE n = 100;
  int nBlocks = 5;
  // std::vector<int> blockSizes{n/nBlocks, n/nBlocks, n/nBlocks, n/nBlocks, n/nBlocks};
  std::vector<int> blockSizes{10, 20, 30, 15, 25};

  assert(nBlocks == blockSizes.size());

  std::vector<uintE> membership(n);
  std::vector<std::vector<double>> affinity(nBlocks, std::vector<double>(nBlocks));
  std::vector<std::vector<int>> cmty(nBlocks);

  blockSizes.push_back(0);
  parlay::scan_inplace(blockSizes);
  assert(n == blockSizes[nBlocks]);
  parlay::parallel_for(0, nBlocks, [&](size_t i){
    auto s = blockSizes[i];
    auto e = blockSizes[i+1];
    cmty[i] = std::vector<int>(e-s);
    parlay::parallel_for(s, e, [&](size_t j){
      membership[j] = i;
      cmty[i][j-s] = j;
    });
  });

  parlay::parallel_for(0, nBlocks, [&](size_t i){
    parlay::parallel_for(0, nBlocks, [&](size_t j){
      if(i==j){
        affinity[i][j] = p_in;
      } else {
        affinity[i][j] = p_out;
      }
    });
  });

  // printVec(membership);
  // printMatrix(affinity);
  // std::cout << "======\n";
  // printMatrix(cmty);


  // std::vector<uintE> membership{0, 0, 0, 1, 1};
  // std::vector<std::vector<double>> affinity{{1, 0},
  //                                             {0, 1}};
  // std::vector<uintE> membership{0, 0, 0, 1, 1};
  // std::vector<std::vector<double>> affinity{{1, 0},
  //                                             {0, 1}};

  auto E = SBM(n, membership, affinity);
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

  auto status = WriteClustering(out_f_cmty.c_str(), cmty);
  if(!status.ok()) return status;

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