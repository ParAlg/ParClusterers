#include "gtest/gtest.h"

#include <vector>

#include "generators/sbm.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/repeated_field.h"


TEST(GeneratorsGTest, testStochasticBlockmodel) {
    int n = 10;
    std::vector<gbbs::uintE> membership = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    std::vector<std::vector<double>> affinity = {{1.0, 0.0}, {0.0, 1.0}};
    auto E = research_graph::in_memory::SBM(n, membership, affinity);
    EXPECT_EQ(20u, E.size());
}
