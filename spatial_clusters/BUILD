licenses(["notice"])

package(default_visibility = ["//visibility:public"])

# All clusterers.

cc_library(
    name = "all-spatial-clusterers",
    deps = [
        "//spatial_clusterers/general_hac:general-hac",
    ],
    alwayslink = 1,
)

cc_library(
    name = "cluster-in-memory_main_lib",
    srcs = ["cluster-in-memory_main.cc"],
    deps = [
        ":all-spatial-clusterers",
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
        ":all-spatial-clusterers",
        "//external:gflags",
    ],
)
