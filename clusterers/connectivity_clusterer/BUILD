licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "connectivity-clusterer",
    srcs = ["connectivity-clusterer.cc"],
    hdrs = ["connectivity-clusterer.h"],
    deps = [
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        "@gbbs//benchmarks/Connectivity/SimpleUnionAsync:Connectivity",
    ],
    alwayslink = 1,
)
