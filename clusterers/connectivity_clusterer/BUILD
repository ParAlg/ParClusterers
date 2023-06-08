licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "connectivity_config_proto",
    srcs = [
        "connectivity_config.proto",
    ],
)

cc_proto_library(
    name = "connectivity_config_cc_proto",
    deps = [":connectivity_config_proto"],
)

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
        "connectivity_config_cc_proto",
    ],
    alwayslink = 1,
)
