licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "dbscan-clusterer",
    srcs = ["dbscan-clusterer.cc"],
    hdrs = ["dbscan-clusterer.h"],
    deps = [
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
