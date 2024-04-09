# The ParClusterers Benchmark Suite (PCBS)

# Compilation
To compile the clusterers:

```bash
git submodule init
git submodule update

# compile the clustering algorithms
bazel build //clusterers:cluster-in-memory_main
# compile the stats computation
bazel build //clusterers:stats-in-memory_main
```

<!-- # Flags

`include_zero_deg_v`: default to false. Use this flag if zero degree vertices should be included in the *output flat clustering*.

# Output
Num clusters: the number of clusters. Note that this number also includes singleton zero-degree node clusters, even if `include_zero_deg_v` is set to true. -->


# Input

Input can be in either edge list format or GBBS (CSR) format. SNAP edge list format can be converted to GBBS format using [GBBS](https://github.com/ParAlg/gbbs).
```bash
# In GBBS
bazel run //utils:snap_converter -- -s -i com-friendster.ungraph.txt -o com-friendster.gbbs.txt
```

# Quick Start

The commands below runs clustering algorithms on the two graphs in `data/` and compute stats on the resulting clusterings.

```bash
python3 cluster.py cluster.config
python3 stats.py cluster.config stats.config
```


# Configuration Files

### cluster.config
This config specifies what clustering algorithms to run, along with the corresponding config proto for each algorithm. The script will run all combinations of the specified config protos, on all given graphs. 

`Iutput directory`: the directory where `Graphs` files are located.

`Output directory`: where the clustering files and log fiels will be stored.

`CSV output directory` after cluster.py and stats.py are run, a `runtime.csv` and `stats.csv` file will be automatically generated into this directory.

`postprocess only`: If it is set to true, when cluster.py or stats.py are run, thhey only generate the csv files from the output files, but do not re-compute the clustering/stats again.

Note that the lines in configurations files should not have dangling `;` at the end.

For example:

```yaml
Input directory: /home/ubuntu/ParClusterers/data/
Output directory: /home/ubuntu/ParClusterers/out/
CSV Output directory: /home/ubuntu/ParClusterers/out_csv/
Clusterers: ConnectivityClusterer;ParallelCorrelationClusterer
Graphs: iris.graph.txt;digits.graph.txt
GBBS format: false
Weighted: true
Number of threads: 10; ALL
Number of rounds: 1
Timeout: 7h
Postprocess only: false

ParallelCorrelationClusterer:
  correlation_clusterer_config:
    resolution: 0; 0.1;0.3; 0.5; 0.7; 0.9; 1
    louvain_config: {num_iterations: 10, num_inner_iterations: 10}; {num_iterations: 20, num_inner_iterations: 20}
    use_refinement: true; false
    clustering_moves_method: LOUVAIN

ConnectivityClusterer:
  connectivity_config:
    threshold: 0.50; 0.98
    upper_bound: false
```


### stats.config

This config specifies what statistics to compute, given that you have already run a set of clustering algorithms using `cluster.config`. Like cluster.config, the stats.config parses the set of flags desired at the top of the file, and the stats config protos at the bottom of the file, and runs all combinations, which are then stored into the same output directory as specified by cluster.config. 

`Deterministic`: if it’s true, the stats will only be computed for a single round and a single thread number, since the clustering algorithm is deterministic, the stats should be the same for all rounds and all thread numbers. 

For example:

```yaml
Input communities: iris.cmty.txt; digits.cmty.txt
Deterministic: false

statistics_config:
  compute_precision_recall: true
  f_score_param: 0.5
```




<!-- The parameters for each Clusterers


# networkit

(TODO): networkit output currently uses `https://github.com/yushangdi/networkit/tree/master`. the output is much faster. We should add a flag that also supports normal networkit outputting.

Input: if .bin filename is used, will use NetworkitBinary file reader. Otherwise use EdgeListReader.

## ConenctivityClusterer
- `threshold`: edges with weight higher (or lower if `upper_bound` is set) than threshold are excluded.
- `upper_bound`: A boolean variable. If true, the threshold is used as an upperbound instead of a lower bound.

## KCoreClusterer
- `threshold`: if (u,v) both have core number >= threshold, they are connected.

## Label Propagation

- max_iteration: maximum iteration to run. If labels converges, can also stop early.
- update_threshold: stop if less than update_threshold number of nodes need to be updated.

TODO: delete par_threshold in the end if we actually don't use it.
TODO: remove async? async seeems to be always better.

#### Neo4j
- run `python3 tests/test_neo4j_installation.py` to test if neo4j is successfully installed.
- minCommunitySize: Only nodes inside communities larger or equal the given value are returned. We set it to 2, so singleton communities are not returned.

#### NetWorkIt LPDegreeOrdered
 Stop when < n/1e5 nodes need to be upadted.


 ## SLLP

 `remove_nested` is slow. Will be automatically skipped if `prune_threshold` > 0.5 -->




# Additional


### Running original sequential Tectonic
If your clustering config file includes the original Tectonic, you also need the `system.config` file. This config lists where g++ and Python can be found. You can modify it to use your preferred compiler and python version. This is used for the original Tectonic, not our parallel Tectonic (TectonicClusterer). For example:
```bash
python3 cluster.py cluster.config system.config
```