
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

# Flags

`include_zero_deg_v`: default to false. Use this flag if zero degree vertices should be included in the *output flat clustering*.


# Output
Num clusters: the number of clusters. Note that this number also includes singleton zero-degree node clusters, even if `include_zero_deg_v` is set to true.


# Config Files

Should not have dangling `;` at the end.


The parameters for each Clusterers

ConenctivityClusterer
- `threshold`: edges with weight higher (or lower if `upper_bound` is set) than threshold are excluded.
- `upper_bound`: A boolean variable. If true, the threshold is used as an upperbound instead of a lower bound.
- `ratio`: edges with weight larger than `ratio`* heaviest edge are excluded. The other parameters are ignored. Should be between 0 and 1.