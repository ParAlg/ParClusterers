licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "labelprop_config_proto",
    srcs = [
        "labelprop_config.proto",
    ],
)

cc_proto_library(
    name = "labelprop_config_cc_proto",
    deps = [":labelprop_config_proto"],
)

cc_library(
    name = "labelprop-clusterer",
    srcs = ["labelprop-clusterer.cc"],
    hdrs = ["labelprop-clusterer.h"],
    deps = [
        ":labelprop_config_cc_proto",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        "@gbbs//benchmarks/LowDiameterDecomposition/MPX13:LowDiameterDecomposition",
    ],
    alwayslink = 1,
)
