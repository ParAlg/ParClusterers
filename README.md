
To compile the clusterers:

```bash
git submodule init
git submodule update
cd external/gbbs/
git submodule init
git submodule update
cd ../../

bazel build //clusterers:cluster-in-memory_main

```