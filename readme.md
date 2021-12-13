To compile:


git submodule update --init --recursive
run `git submodule init` and  `git submodule update` in ParClusterers/ and ParClusterers/external/gbbs
run `bazel build //clusterers:all-clusterers` in ParClusterers/