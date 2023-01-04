licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "kcore_config_proto",
    srcs = [
        "kcore_config.proto",
    ],
)

cc_proto_library(
    name = "kcore_config_cc_proto",
    deps = [":kcore_config_proto"],
)

cc_library(
    name = "kcore-clusterer",
    srcs = ["kcore-clusterer.cc"],
    hdrs = ["kcore-clusterer.h"],
    deps = [
        ":kcore_config_cc_proto",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        "@gbbs//benchmarks/Connectivity/SimpleUnionAsync:Connectivity",
        "@gbbs//benchmarks/KCore/JulienneDBS17:KCore",
    ],
    alwayslink = 1,
)
