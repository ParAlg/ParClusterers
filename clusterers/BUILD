licenses(["notice"])

package(default_visibility = ["//visibility:public"])

# All clusterers.

cc_library(
    name = "all-clusterers",
    deps = [
        "//clusterers/affinity:parallel-affinity",
        "//clusterers/connectivity_clusterer:connectivity-clusterer",
        "//clusterers/kcore_clusterer:kcore-clusterer",
        "//clusterers/example_clusterer:example-clusterer",
        "//clusterers/ldd_clusterer:ldd-clusterer",
        "//clusterers/tectonic_clusterer:tectonic-clusterer",
        "//clusterers/scan_clusterer:scan-clusterer",
        "//clusterers/labelprop_clusterer:labelprop-clusterer",
        "//clusterers/slpa_clusterer:slpa-clusterer",
        "@com_github_graph_mining//in_memory/clustering/affinity:parallel_affinity",
        "@com_github_graph_mining//in_memory/clustering/hac:parhac",
        "@com_github_graph_mining//in_memory/clustering/correlation:parallel_correlation",
        "@com_github_graph_mining//in_memory/clustering/correlation:parallel_modularity",
        "@com_github_graph_mining//in_memory/clustering:config_cc_proto"
    ],
    alwayslink = 1,
)

cc_library(
    name = "all-metric-clusterers",
    deps = [
        "//clusterers/metric_example_clusterer:metric-example-clusterer",
    ],
    alwayslink = 1,
)

cc_library(
    name = "gbbs_graph_io",
    srcs = ["gbbs_graph_io.cc"],
    hdrs = ["gbbs_graph_io.h"],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:gbbs-graph",
        "@com_github_graph_mining//in_memory/clustering:in_memory_clusterer"
    ],
)

proto_library(
    name = "clustering_stats_proto",
    srcs = [
        "clustering_stats.proto",
    ],
)

cc_proto_library(
    name = "clustering_stats_cc_proto",
    deps = [":clustering_stats_proto"],
)

cc_library(
    name = "clustering_stats",
    srcs = ["clustering_stats.cc"],
    hdrs = ["clustering_stats.h"],
    deps = [
        ":clustering_stats_cc_proto",
        "//clusterers/stats:stats_utils",
        "//clusterers/stats:stats_communities",
        "//clusterers/stats:stats_correlation",
        "//clusterers/stats:stats_diameter",
        "//clusterers/stats:stats_density",
        "//clusterers/stats:stats_ari",
        "//clusterers/stats:stats_nmi",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:status_macros",
        "//external:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:protobuf",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
    ],
)

cc_library(
    name = "cluster-in-memory_main_lib",
    srcs = ["cluster-in-memory_main.cc"],
    deps = [
        ":all-clusterers",
        ":gbbs_graph_io",
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
    name = "stats-in-memory_main_lib",
    srcs = ["stats-in-memory_main.cc"],
    deps = [
        ":clustering_stats",
        ":gbbs_graph_io",
        "//clusterers/stats:stats_utils",
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
    name = "stats-in-memory_main",
    deps = [
        ":stats-in-memory_main_lib",
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