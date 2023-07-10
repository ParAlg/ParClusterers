licenses(["notice"])

cc_library(
    name = "scan",
    srcs = [
        "scan.cc",
        "scan_helpers.cc",
    ],
    hdrs = [
        "scan.h",
        "scan_helpers.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":similarity_measure",
        ":utils",
        "@gbbs//benchmarks/Connectivity/UnionFind:union_find_rules",
        "@gbbs//gbbs:bridge",
        "@gbbs//gbbs:graph",
        "@gbbs//gbbs:macros",
        "@gbbs//gbbs/helpers:sparse_table",
    ],
)

cc_library(
    name = "similarity_measure",
    srcs = [
        "similarity_measure.cc",
    ],
    hdrs = [
        "intersect.h",
        "similarity_measure.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@gbbs//benchmarks/TriangleCounting/ShunTangwongsan15:Triangle",
        "@gbbs//gbbs:bridge",
        "@gbbs//gbbs:graph_mutation",
        "@gbbs//gbbs:macros",
        "@gbbs//gbbs:vertex",
        "@gbbs//gbbs/helpers:assert",
    ],
)

cc_library(
    name = "utils",
    srcs = [
        "utils.cc",
    ],
    hdrs = [
        "utils.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@gbbs//gbbs:bridge",
        "@gbbs//gbbs:graph",
        "@gbbs//gbbs:macros",
    ],
)
