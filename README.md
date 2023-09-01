
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


# Input

Input can be in either edge list format or GBBS format. SNAP edge list format can be converted to GBBS format like this;

```bash
cd ParClusterers/external/gbbs
bazel run //utils:snap_converter -- -s -i com-friendster.ungraph.txt -o com-friendster.gbbs.txt
```

# Config Files

Should not have dangling `;` at the end.

`Write clustering`: if set to false, do not write clustering out.

The parameters for each Clusterers


# networkit

(TODO): networkit output currently uses `https://github.com/yushangdi/networkit/tree/master`. the output is much faster. We should add a flag that also supports normal networkit outputting.

## ConenctivityClusterer
- `threshold`: edges with weight higher (or lower if `upper_bound` is set) than threshold are excluded.
- `upper_bound`: A boolean variable. If true, the threshold is used as an upperbound instead of a lower bound.
- `ratio`: edges with weight larger than `ratio`* heaviest edge are excluded. The other parameters are ignored. Should be between 0 and 1.

## KCoreClusterer
- `threshold`: if (u,v) both have core number >= threshold, they are connected.

## Label Propagation

- max_iteration: maximum iteration to run. If labels converges, can also stop early.
- update_threshold: stop if less than update_threshold number of nodes need to be updated.

TODO: delete par_threshold in the end if we actually don't use it.
TODO: remove async? async seeems to be always better.

#### Neo4j
- minCommunitySize: Only nodes inside communities larger or equal the given value are returned. We set it to 2, so singleton communities are not returned.

#### NetWorkIt LPDegreeOrdered
 Stop when < n/1e5 nodes need to be upadted.


 ## SLLP

 `remove_nested` is slow. Will be automatically skipped if `prune_threshold` > 0.5