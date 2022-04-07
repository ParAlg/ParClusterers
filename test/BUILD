licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_test(
  name = "test",
  size = "small",
  srcs = ["kdTree_test.cpp"],
  deps = [
        "//clusterers/hac_euclidean_clusterer:point",
        "//clusterers/hac_euclidean_clusterer:node",
        "//clusterers/hac_euclidean_clusterer:kdtree",
        "//clusterers/hac_euclidean_clusterer:finder",
        "//clusterers/hac_euclidean_clusterer:linkage_types",
        "//clusterers/hac_euclidean_clusterer:dist",
        ':uniform',
        "@com_google_googletest//:gtest_main",
        "@gbbs//gbbs:bridge"],
)

cc_library(
    name = "uniform",
    hdrs = ["uniform.h"],
    deps = [
      "//clusterers/hac_euclidean_clusterer:point",
    ],
)