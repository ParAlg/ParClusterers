licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_test(
  name = "test",
  size = "small",
  srcs = ["kdTree_test.cpp"],
  deps = [
        ':point',
        ':kdtree',
        ':uniform',
        ":node",
        "@com_google_googletest//:gtest_main",
        "@gbbs//gbbs:bridge"],
)

cc_library(
    name = "point",
    hdrs = ["point.h"],
    deps = [
    ],
)

cc_library(
    name = "kdtree",
    hdrs = ["kdtree/kdtree.h"],
    deps = [
        ':treeImpl',
        ':rangeSearchImpl',
        ':dualTreeImpl',
    ],
)

cc_library(
    name = "treeImpl",
    hdrs = ["kdtree/treeImpl.h"],
    deps = [
    ],
)

cc_library(
    name = "rangeSearchImpl",
    hdrs = ["kdtree/rangeSearchImpl.h"],
    deps = [
    ],
)

cc_library(
    name = "dualTreeImpl",
    hdrs = ["kdtree/dualTreeImpl.h"],
    deps = [
    ],
)

cc_library(
    name = "uniform",
    hdrs = ["uniform.h"],
    deps = [
      ":point"
    ],
)

cc_library(
    name = "node",
    hdrs = ["node.h"],
    deps = [
       ":point"
    ],
)