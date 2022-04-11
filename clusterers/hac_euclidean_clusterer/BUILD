licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hac-euclidean-clusterer",
    srcs = ["hac-euclidean-clusterer.cc"],
    hdrs = glob(["*.h"]), #"hac-euclidean-clusterer.h"
    deps = [
        ":utils",
        ":kdtree",
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
        ":utils",
        ':finder',
    ],
)



cc_library(
    name = "finder",
    hdrs = ["finder.h"],
    deps = [
        ":utils",
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
    name = "kdtree",
    hdrs = glob(["kdtree/*.h"]),
    deps = [
        ":utils",
    ],
)

cc_library(
    name = "utils",
    hdrs = glob(["utils/*.h"]),
    deps = [
    ],
)


