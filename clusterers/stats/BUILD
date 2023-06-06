licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "stats_utils",
    hdrs = ["stats_utils.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
    name = "stats_communities",
    hdrs = ["stats_communities.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
    name = "stats_correlation",
    hdrs = ["stats_correlation.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
    name = "stats_diameter",
    hdrs = ["stats_diameter.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
        "@gbbs//benchmarks/GeneralWeightSSSP/BellmanFord:BellmanFord",
        "@gbbs//benchmarks/Connectivity/SimpleUnionAsync:Connectivity",
    ],
)

cc_library(
    name = "stats_density",
    hdrs = ["stats_density.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
        "@gbbs//benchmarks/TriangleCounting/ShunTangwongsan15:Triangle",
    ],
)

cc_library(
    name = "stats_ari",
    hdrs = ["stats_ari.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
        "@parlaylib//parlay:hash_table",
    ],
)

cc_library(
    name = "stats_nmi",
    hdrs = ["stats_nmi.h"],
    deps = [
        "//clusterers:clustering_stats_cc_proto",
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
        "@parlaylib//parlay:hash_table",
    ],
)