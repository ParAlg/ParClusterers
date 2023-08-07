#include "gtest/gtest.h"

#include <vector>

#include "generators/sbm.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"

template<class Seq>
void printE(Seq& E){
    for(auto e : E){
      std::cout << std::get<0>(e) << " " << std::get<1>(e) << std::endl;
    }
}


TEST(SBMTest, zeroprob) {
    int n = 10;
    std::vector<gbbs::uintE> membership = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    std::vector<std::vector<double>> affinity = {{1.0, 0.0}, {0.0, 1.0}};
    auto E = research_graph::in_memory::SBM(n, membership, affinity);
    EXPECT_EQ(20u, E.size());
}


TEST(SBMTest, decimalprob) {
    int n = 10;
    std::vector<gbbs::uintE> membership = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    std::vector<std::vector<double>> affinity = {{0.5, 0.0}, {0.0, 1.0}};
    auto E = research_graph::in_memory::SBM(n, membership, affinity);
    EXPECT_EQ(15u, E.size());
    // printE(E);
}
