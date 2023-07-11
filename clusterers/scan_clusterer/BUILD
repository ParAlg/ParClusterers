licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "scan_config_proto",
    srcs = [
        "scan_config.proto",
    ],
)

cc_proto_library(
    name = "scan_config_cc_proto",
    deps = [":scan_config_proto"],
)

cc_library(
    name = "scan-clusterer",
    srcs = ["scan-clusterer.cc"],
    hdrs = ["scan-clusterer.h"],
    deps = [
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        "//clusterers/scan_clusterer/IndexBased:scan",
        "//clusterers/scan_clusterer/IndexBased:utils",
        "//clusterers/scan_clusterer/IndexBased:similarity_measure",
        "scan_config_cc_proto",
    ],
    alwayslink = 1,
)
