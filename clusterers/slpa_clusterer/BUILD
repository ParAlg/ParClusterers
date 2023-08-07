licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "slpa_config_proto",
    srcs = [
        "slpa_config.proto",
    ],
)

cc_proto_library(
    name = "slpa_config_cc_proto",
    deps = [":slpa_config_proto"],
)

cc_library(
    name = "slpa-clusterer",
    srcs = ["slpa-clusterer.cc"],
    hdrs = ["slpa-clusterer.h"],
    deps = [
        ":slpa_config_cc_proto",
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        "@parlaylib//parlay:hash_table"
    ],
    alwayslink = 1,
)
