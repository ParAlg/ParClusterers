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



namespace research_graph::in_memory {
  
absl::Status Main() {
  std::string out_f = absl::GetFlag(FLAGS_output_file);
  using uintE = gbbs::uintE;
  uintE n = 5;
  std::vector<uintE> membership{0, 0, 0, 1, 1};
  std::vector<std::vector<double>> affinity{{1, 0},
                                              {0, 1}};

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