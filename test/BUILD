licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_test(
  name = "clusterer_test",
  size = "small",
  srcs = ["clusterer_test.cpp"],
  deps = [
        "//clusterers/hac_euclidean_clusterer:clusterer",
        "//clusterers/hac_euclidean_clusterer:utils",
        "//clusterers/hac_euclidean_clusterer:dist",
        "//clusterers/hac_euclidean_clusterer:treeUtilities",
        ":pointIO",
        "@com_google_googletest//:gtest_main",
        "@gbbs//gbbs:bridge"],
)


cc_test(
  name = "test",
  size = "small",
  srcs = ["kdTree_test.cpp", "clusterer_test.cpp"],
  deps = [
        "//clusterers/hac_euclidean_clusterer:utils",
        "//clusterers/hac_euclidean_clusterer:kdtree",
        "//clusterers/hac_euclidean_clusterer:finder",
        "//clusterers/hac_euclidean_clusterer:dist",
        "//clusterers/hac_euclidean_clusterer:clusterer",
        ':uniform',
        ":pointIO",
        "@com_google_googletest//:gtest_main",
        "@gbbs//gbbs:bridge"],
)

cc_library(
    name = "uniform",
    hdrs = ["uniform.h"],
    deps = [
      "//clusterers/hac_euclidean_clusterer:utils",
    ],
)

cc_library(
    name = "pointIO",
    hdrs = ["pointIO.h"],
    deps = [
      ":IO"
    ],
)

cc_library(
    name = "IO",
    hdrs = ["IO.h"],
    deps = [
    ],
)

