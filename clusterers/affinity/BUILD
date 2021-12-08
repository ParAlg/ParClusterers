licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "parallel-affinity",
    srcs = ["parallel-affinity.cc"],
    hdrs = ["parallel-affinity.h"],
    deps = [
        ":parallel-affinity-internal",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
    ],
    alwayslink = 1,
)

# Private support libraries.

cc_library(
    name = "parallel-affinity-internal",
    srcs = ["parallel-affinity-internal.cc"],
    hdrs = ["parallel-affinity-internal.h"],
    deps = [
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@parcluster//parcluster/api/parallel:parallel-sequence-ops",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@gbbs//benchmarks/Connectivity/WorkEfficientSDB14:Connectivity",
        "@gbbs//gbbs",
        "@gbbs//gbbs:bridge",
        "@gbbs//gbbs:macros",
    ],
)
