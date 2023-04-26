licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "ari_test",
    srcs = ["test_stats_ari.cc"],
    deps = ["@com_google_googletest//:gtest_main",     
            "//clusterers/stats:stats_ari",
            "@com_google_protobuf//:protobuf",
            # "//clusterers/stats:stats_utils",
            "//external:gflags",
            # "//clusterers:clustering_stats_cc_proto",
            # "@com_google_absl//absl/base",
            # "@com_google_absl//absl/flags:flag",
            # "@com_google_absl//absl/flags:parse",
            # "@com_google_absl//absl/status",
            # "@com_google_absl//absl/status:statusor",
            # "@com_google_absl//absl/strings",
            # "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "ari_test2",
    srcs = ["test_stats_ari2.cc"],
    deps = ["@com_google_googletest//:gtest_main",     
            "//clusterers/stats:stats_ari2",
    ],
)

cc_test(
  name = "hello_test",
  size = "small",
  srcs = ["naive_test.cc"],
  deps = ["@com_google_googletest//:gtest_main",],
)