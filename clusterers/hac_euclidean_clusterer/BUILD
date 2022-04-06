licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hac-euclidean-clusterer",
    srcs = ["hac-euclidean-clusterer.cc"],
    hdrs = ["hac-euclidean-clusterer.h"],
    deps = [
        ':point',
        ':linkage',
        ':unionfind',
        ':linkage_types',
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:datapoint",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@parcluster//parcluster/api:in-memory-metric-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
    ],
    alwayslink = 1,
)


cc_library(
    name = "linkage",
    hdrs = ["linkage.h"],
    deps = [
    ],
)

cc_library(
    name = "unionfind",
    hdrs = ["unionfind.h"],
    deps = [
    ],
)

cc_library(
    name = "linkage_types",
    hdrs = ["linkage_types.h"],
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
    name = "node",
    hdrs = ["node.h"],
    deps = [
       ":point",
       ":utils"
    ],
)

cc_library(
    name = "utils",
    hdrs = ["utils.h"],
    deps = [
    ],
)


cc_library(
    name = "point",
    hdrs = ["point.h"],
    deps = [
    ],
)