cc_library(
    name = "sbm_lib",
    srcs = ["sbm.cc"],
    deps = [
        "//external:gflags",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:protobuf",
        "@gbbs//gbbs:macros",
        "@gbbs//gbbs:bridge",
    ],
)

cc_binary(
    name = "sbm",
    deps = [
        ":sbm_lib",
        "//external:gflags",
    ],
)