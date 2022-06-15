licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "example_config_proto",
    srcs = [
        "example_config.proto",
    ],
)

cc_proto_library(
    name = "example_config_cc_proto",
    deps = [":example_config_proto"],
)

cc_library(
    name = "example-clusterer",
    srcs = ["example-clusterer.cc"],
    hdrs = ["example-clusterer.h"],
    deps = [
        ":example_config_cc_proto",
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
