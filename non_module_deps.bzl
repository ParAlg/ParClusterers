load("@@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
# -- load statements -- #

def _non_module_deps_impl(ctx):
  git_repository(
    name = "gbbs",
    remote = "https://github.com/ParAlg/gbbs.git",
    branch = "master",
  )
  git_repository(
    name = "com_github_graph_mining",
    remote = "https://github.com/google/graph-mining.git",
    commit = "3fbcb0af2352b459cde2ba104cddd5f07214c584",
  )
  git_repository(
    name = "parlaylib",
    remote = "https://github.com/ParAlg/parlaylib.git",
    commit = "6b4a4cdbfeb3c481608a42db0230eb6ebb87bf8d",
    strip_prefix = "include/",
  )
  git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    tag = "v2.2.2"
)
# -- repo definitions -- #

non_module_deps = module_extension(implementation = _non_module_deps_impl)
