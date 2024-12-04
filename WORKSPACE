workspace(name = "ParClusterers")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "parcluster",
    remote = "git@github.com:ParAlg/InMemoryClusteringAPI.git",
    commit = "ccf763ad6dcf0e36c3b0b212d429ef19c2cbf837",
    strip_prefix = "include/",
)

git_repository(
    name = "gbbs",
    remote = "https://github.com/ParAlg/gbbs.git",
    branch = "master"
)

git_repository(
    name = "parlaylib",
    remote = "https://github.com/ParAlg/parlaylib.git",
    commit = "6b4a4cdbfeb3c481608a42db0230eb6ebb87bf8d",
    strip_prefix = "include/",
)

git_repository(
    name = "com_github_graph_mining",
    remote = "https://github.com/google/graph-mining.git",
    commit = "c4a057da0bf0ba5fb6f0a209869425cd38515c03"
)


FARMHASH_COMMIT = "0d859a811870d10f53a594927d0d0b97573ad06d"
FARMHASH_SHA256 = "18392cf0736e1d62ecbb8d695c31496b6507859e8c75541d7ad0ba092dc52115"

http_archive(
    name = "farmhash_archive",
    build_file = "@com_github_graph_mining//utils:farmhash.BUILD",
    sha256 = FARMHASH_SHA256,
    strip_prefix = "farmhash-{commit}".format(commit = FARMHASH_COMMIT),
    urls = ["https://github.com/google/farmhash/archive/{commit}.tar.gz".format(commit = FARMHASH_COMMIT)],
)

git_repository(
    name = "com_github_gbbs",
    remote = "https://github.com/ParAlg/gbbs.git",
    branch = "master"
)

git_repository(
    name = "com_github_gflags_gflags",
    commit = "f40e43a6288940efadd29c208085db05335a15d8",
    remote = "https://github.com/gflags/gflags.git",
)

bind(
    name   = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)

# rules_proto defines abstract rules for building Protocol Buffers.

http_archive(
    name = "rules_proto",
    sha256 = "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
    strip_prefix = "rules_proto-4.0.0",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
    ],
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

rules_cc_dependencies()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

git_repository(
    name = "com_google_protobuf",
    remote = "https://github.com/protocolbuffers/protobuf.git",
    tag = "v22.3",
)

git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp.git",
    tag = "20230125.2",
)

# Bazel platform rules.
http_archive(
    name = "platforms",
    sha256 = "b601beaf841244de5c5a50d2b2eddd34839788000fa1be4260ce6603ca0d8eb7",
    strip_prefix = "platforms-98939346da932eef0b54cf808622f5bb0928f00b",
    urls = ["https://github.com/bazelbuild/platforms/archive/98939346da932eef0b54cf808622f5bb0928f00b.zip"],
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
)

# rules_cc defines rules for generating C++ code from Protocol Buffers.
http_archive(
    name = "rules_cc",
    sha256 = "fa42eade3cad9190c2a6286a6213f07f1a83d26d9f082d56f526d014c6ea7444",
    strip_prefix = "rules_cc-02becfef8bc97bda4f9bb64e153f1b0671aec4ba",
    urls = [
        "https://github.com/bazelbuild/rules_cc/archive/02becfef8bc97bda4f9bb64e153f1b0671aec4ba.zip",
    ],
)
