licenses(["notice"])

package(default_visibility = ["//visibility:public"])

# All clusterers.

cc_library(
    name = "all-clusterers",
    deps = [
        "//clusterers/affinity:parallel-affinity",
        "//clusterers/connectivity_clusterer:connectivity-clusterer",
        "//clusterers/example_clusterer:example-clusterer",
        "//clusterers/ldd_clusterer:ldd-clusterer",
    ],
    alwayslink = 1,
)

cc_library(
    name = "all-metric-clusterers",
    deps = [
        "//clusterers/metric_example_clusterer:metric-example-clusterer",
        "//clusterers/dbscan_clusterer:dbscan-clusterer",
    ],
    alwayslink = 1,
)

cc_library(
    name = "cluster-in-memory_main_lib",
    srcs = ["cluster-in-memory_main.cc"],
    deps = [
        ":all-clusterers",
        "//external:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:protobuf",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
    ],
)

cc_binary(
    name = "cluster-in-memory_main",
    deps = [
        ":cluster-in-memory_main_lib",
        ":all-clusterers",
        "//external:gflags",
    ],
)

cc_library(
    name = "cluster-in-memory-metric_main_lib",
    srcs = ["cluster-in-memory-metric_main.cc"],
    deps = [
        ":all-metric-clusterers",
        "//external:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:protobuf",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:datapoint",
        "@parcluster//parcluster/api:in-memory-metric-clusterer-base",
    ],
)

cc_binary(
    name = "cluster-in-memory-metric_main",
    deps = [
        ":cluster-in-memory-metric_main_lib",
        ":all-metric-clusterers",
        "//external:gflags",
    ],
)
