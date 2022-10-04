licenses(["notice"])

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "tectonic_config_proto",
    srcs = [
        "tectonic_config.proto",
    ],
)

cc_proto_library(
    name = "tectonic_config_cc_proto",
    deps = [":tectonic_config_proto"],
)

cc_library(
    name = "tectonic-clusterer",
    srcs = ["tectonic-clusterer.cc"],
    hdrs = ["tectonic-clusterer.h"],
    deps = [
        "@parcluster//parcluster/api:config_cc_proto",
        "@parcluster//parcluster/api:gbbs-graph",
        "@parcluster//parcluster/api:in-memory-clusterer-base",
        "@parcluster//parcluster/api:status_macros",
        "@parcluster//parcluster/api/parallel:parallel-graph-utils",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:statusor",
        ":tectonic_config_cc_proto",
        "@gbbs//benchmarks/Connectivity/SimpleUnionAsync:Connectivity",
        "@gbbs//benchmarks/KCore/JulienneDBS17:KCore",
        "@gbbs//benchmarks/TriangleCounting/ShunTangwongsan15:Triangle",
        "@gbbs//benchmarks/DegeneracyOrder/GoodrichPszona11:DegeneracyOrder",
        "@gbbs//benchmarks/DegeneracyOrder/BarenboimElkin08:DegeneracyOrder",
    ],
    alwayslink = 1,
)
