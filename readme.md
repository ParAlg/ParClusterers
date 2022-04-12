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

AVGSQ:
bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_simple'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k'}"

Ward:
bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config="hac_clusterer_config {linkage_method: WARD, distance: EUCLIDEAN}"


bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: WARD, distance: EUCLIDEAN, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k'}"

AVG:
bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN,output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_simple_avg'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/10D_UCI1_19K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_uci1'}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: COMPLETE, distance: EUCLIDEAN, output_dendro:'/Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k_comp'}"

./linkage /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_euc_1k 2

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs
avgsq: 60.1333
ward:  20.90836938
avg: 16.79327897
complete: 19.62780549

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs
Calling clustering.
Distance: Euclidean Square
Linkage method: average linkage
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k
Cost: 5984206.371

WARD: 98231.0161
avg: 31650.13896
complete: 48308.50497 
Note: if all neighbors are the same, but checksum is different, might be that the dual tree finds the right neighbor, but wrong distances

518.0883688976328 -282.80773366209644

Read Time: 0.004846
Num workers: 8
Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/2D_GaussianDisc_1K.pbbs
Calling clustering.
Distance: Euclidean
Linkage method: complete linkage

Round 1
Comp Num 1000
Chain # 1000

Round 2
Comp Num 700
Chain # 582

Round 3
Comp Num 519
Chain # 387

Round 4
Comp Num 407
Chain # 267

Round 5
Comp Num 326
Chain # 200

Round 6
Comp Num 263
Chain # 160

Round 7
Comp Num 221
Chain # 122

Round 8
Comp Num 183
Chain # 102

Round 9
Comp Num 152
Chain # 83

Round 10
Comp Num 128
Chain # 61

Round 11
Comp Num 111
Chain # 56

Round 12
Comp Num 100
Chain # 36

Round 13
Comp Num 86
Chain # 47

Round 14
Comp Num 72
Chain # 45

Round 15
Comp Num 60
Chain # 34

Round 16
Comp Num 48
Chain # 29

Round 17
Comp Num 41
Chain # 26

Round 18
Comp Num 33
Chain # 19


true: 18 999 988 360.783
false: 18 999 989 98.0296, but the distance should be 518.363

Round 19
Comp Num 26
Chain # 18

Round 20
Comp Num 21
Chain # 14

Round 21
Comp Num 17
Chain # 14

Round 22
Comp Num 13
Chain # 10

Round 23
Comp Num 8
Chain # 8

Round 24
Comp Num 7
Chain # 4

Round 25
Comp Num 5
Chain # 5

Round 26
Comp Num 3
Chain # 3

Round 27
Comp Num 2
Chain # 2
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_1k_comp
Cost: 48308.50497 
Num clusters = 1000
Cluster Time: 0.072127

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/10D_UCI1_19K.pbbs
Calling clustering.
Distance: Euclidean Square
Linkage method: average linkage
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_uci1
Cost:  20366870.42
Num clusters = 19020
Cluster Time: 8.36288