To compile:


git submodule update --init --recursive
run `git submodule init` and  `git submodule update` in ParClusterers/ and ParClusterers/external/gbbs
run `bazel build //clusterers:all-clusterers` in ParClusterers/

bazel-3.0.0 run //clusterers:cluster-in-memory_main

bazel-3.0.0 build //... -c dbg --config=serial
./bazel-bin/clusterers/cluster-in-memory_main --clusterer_name=ExampleClusterer --input_graph=../datasets/simplegraph

bazel-3.0.0 run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=MetricExampleClusterer --input_points=/home/ubuntu/datasets/simple


bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config='hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ}'

bazel build //clusterers:cluster-in-memory-metric_main -c dbg --config=serial 

bazel build //clusterers:cluster-in-memory-metric_main -c dbg --config=serial 

bazel run //clusterers:cluster-in-memory-metric_main -c dbg --config=serial -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_simple'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/10D_UCI1_19K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_uci1'}"

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs
60.1333

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs
Calling clustering.
Distance: Euclidean Square
Linkage method: average linkage
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k
Cost: 5.98421e+06

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/10D_UCI1_19K.pbbs
Calling clustering.
Distance: Euclidean Square
Linkage method: average linkage
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_uci1
Cost:  20366870.42
Num clusters = 19020
Cluster Time: 8.36288