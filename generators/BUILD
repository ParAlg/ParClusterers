licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sbm",
    hdrs = ["sbm.h"],
    deps = [
        "//clusterers/metric_example_clusterer:metric-example-clusterer",
        "@gbbs//gbbs:macros",
        "@gbbs//gbbs:bridge",
    ],
    alwayslink = 1,
)


cc_library(
    name = "sbm_lib",
    srcs = ["sbm.cc"],
    deps = [
        ":sbm",
        "//external:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_binary(
    name = "sbm_main",
    deps = [
        ":sbm_lib",
        "//external:gflags",
    ],
)