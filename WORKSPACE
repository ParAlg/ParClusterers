workspace(name = "ParClusterers")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "gbbs",
    remote = "https://github.com/ParAlg/gbbs.git",
    commit = "d385adac3bda3671016574a67c8387c6c8b486b6",
)
# should InMemoryClusteringAPI be a git repository below?
#git_repository(
#)

local_repository(
    name = "InMemoryClusteringAPI",
    path = "InMemoryClusteringAPI",
#    repo_mapping = {"@gbbs" : "@gbbs-local"},
)

http_archive(
    name = "com_github_gflags_gflags",
    strip_prefix = "gflags-master",
    urls = ["https://github.com/gflags/gflags/archive/master.zip"],
)

bind(
    name   = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
)

http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-3.13.0",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.13.0.tar.gz"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest.git",
    tag = "release-1.8.1",
)

git_repository(
     name = "rules_cc",
    commit = "daf6ace7cfeacd6a83e9ff2ed659f416537b6c74",
    remote = "https://github.com/bazelbuild/rules_cc.git",
 )


load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()

# rules_proto defines abstract rules for building Protocol Buffers.

git_repository(
     name = "rules_proto",
    commit = "cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2",
    remote = "https://github.com/bazelbuild/rules_proto.git",
 )

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()