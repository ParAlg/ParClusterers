workspace(name = "ParClusterers")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

local_repository(
    name = "parcluster",
    path = "external/InMemoryClusteringAPI/include",
)

local_repository(
    name = "gbbs",
    path = "external/gbbs",
)

local_repository(
    name = "parlaylib",
    path = "external/gbbs/external/parlaylib/include",
)

local_repository(
    name = "PAM",
    path = "external/gbbs/external/PAM/include",
)

#http_archive(
#    name = "com_github_gflags_gflags",
#    strip_prefix = "gflags-master",
#    urls = ["https://github.com/gflags/gflags/archive/master.zip"],
#)

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


##http_archive(
#git_repository(
#    name = "rules_proto",
##    sha256 = "37d32b789be90fead9ab108dbe4fe4df463d26c122dc896dc1bf134252d3c49a",
##    strip_prefix = "rules_proto-40298556293ae502c66579620a7ce867d5f57311",
#    commit = "cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2",
#    remote = "https://github.com/bazelbuild/rules_proto.git",
##    urls = [
##        "https://github.com/bazelbuild/rules_proto/archive/40298556293ae502c66579620a7ce867d5f57311.zip",
##    ],
#)
#load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
#protobuf_deps()

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

rules_cc_dependencies()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

#http_archive(
#    name = "com_google_absl",
#    strip_prefix = "abseil-cpp-master",
#    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
#)
#
#http_archive(
#    name = "com_google_protobuf",
#    strip_prefix = "protobuf-3.13.0",
#    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.13.0.tar.gz"],
#)

#load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
#protobuf_deps()

#git_repository(
#    name = "com_google_googletest",
#    remote = "https://github.com/google/googletest.git",
#    tag = "release-1.8.1",
#)

#git_repository(
#     name = "rules_cc",
#    commit = "daf6ace7cfeacd6a83e9ff2ed659f416537b6c74",
#    remote = "https://github.com/bazelbuild/rules_cc.git",
# )
#
#
#load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
#rules_cc_dependencies()
#
## rules_proto defines abstract rules for building Protocol Buffers.
#
#git_repository(
#     name = "rules_proto",
#    commit = "cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2",
#    remote = "https://github.com/bazelbuild/rules_proto.git",
# )
#
#load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
#rules_proto_dependencies()
#rules_proto_toolchains()
#
git_repository(
    name = "com_google_protobuf",
    remote = "https://github.com/protocolbuffers/protobuf.git",
    tag = "v3.9.2",
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
)

# Bazel platform rules.
http_archive(
    name = "platforms",
    sha256 = "b601beaf841244de5c5a50d2b2eddd34839788000fa1be4260ce6603ca0d8eb7",
    strip_prefix = "platforms-98939346da932eef0b54cf808622f5bb0928f00b",
    urls = ["https://github.com/bazelbuild/platforms/archive/98939346da932eef0b54cf808622f5bb0928f00b.zip"],
)

#http_archive(
#  name = "com_google_absl",
#  urls = ["https://github.com/abseil/abseil-cpp/archive/98eb410c93ad059f9bba1bf43f5bb916fc92a5ea.zip"],
#  strip_prefix = "abseil-cpp-98eb410c93ad059f9bba1bf43f5bb916fc92a5ea",
#)

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest.git",
    tag = "release-1.10.0",
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
