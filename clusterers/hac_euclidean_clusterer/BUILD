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
        ':finder',
        ":clusterer",
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
    name = "clusterer",
    hdrs = ["clusterer.h"],
    deps = [
        ":unionfind",
        ":linkage_types",
        ':finder',
    ],
)



cc_library(
    name = "finder",
    hdrs = ["finder.h"],
    deps = [
        ":unionfind",
        ":linkage_types",
        ':point',
        ':kdtree',
    ],
)


cc_library(
    name = "dist",
    hdrs = ["dist.h"],
    deps = [
        ":kdtree",
        ":treeUtilities"
    ],
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
    name = "treeUtilities",
    hdrs = ["treeUtilities.h"],
    deps = [
        ":point",
        ":unionfind",
        ":linkage_types",
        ":cacheUtilities",
        ":utils"
    ],
)

cc_library(
    name = "cacheUtilities",
    hdrs = ["cacheUtilities.h"],
    deps = [
        ':node',
        "hashtable_parallel"
    ],
)


cc_library(
    name = "hashtable_parallel",
    hdrs = ["hashtable_parallel.h"],
    deps = [
        ":utils"
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
