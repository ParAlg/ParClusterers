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

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACEuclideanClusterer --input_points=/ubuntu/sy/datasets/2D_GaussianDisc_1K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: AVERAGE, distance: EUCLIDEAN_SQ}"

Ward:
bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/Users/sy/Desktop/MIT/PAPERS/clustering/datasets/small/example2.pbbs --clusterer_config="hac_clusterer_config {linkage_method: WARD, distance: EUCLIDEAN}"

bazel run //clusterers:cluster-in-memory-metric_main -- --clusterer_name=HACClusterer --input_points=/home/ubuntu/datasets/2D_GaussianDisc_100K.pbbs --clusterer_config="hac_clusterer_config {linkage_method: WARD, distance: EUCLIDEAN}"
23.846692

./linkage -method ward -r 1 -d 2 -matrix /home/ubuntu/datasets/2D_GaussianDisc_100K.pbbs
20.144

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

Point set: /Users/sy/Desktop/MIT/PAPERS/clustering/datasets/10D_UCI1_19K.pbbs
Calling clustering.
Distance: Euclidean Square
Linkage method: average linkage
dednrogram output file: /Users/sy/Desktop/MIT/clusterer/ParClusterers/outputs/hac_uci1
Cost:  20366870.42
Num clusters = 19020
Cluster Time: 8.36288




 ./linkage -method avgsq -r 1 -d 2 -cachesize 1 /home/ubuntu/datasets/2D_GaussianDisc_100K.pbbs
+++++++++++++++
eps = 9.999999999999999451532715e-21
use matrix = 0
use matrix range = 0
naive_thresh = 5
cache_size/2 = 0
average Linkage of 100000, dim 2 points, sqeuclidean
dist Avg 4, use centers and variances, norm = 2
========= CHAIN TREE =========
num workers 96
hash table size 0
::initialize: 0.034797

[CLINK] Round = 1
[CLINK] Comp Num = 100000
[Chain] # = 100000
::init-chains: 0.0047789
::link-update: 0.019216
::update-clusters: 0.0064199

Round 1
Parlay time: find-nn: 0.9517
Parlay time: link-update: 0.0439
Parlay time: update-clusters: 0.0122

[CLINK] Round = 2
[CLINK] Comp Num = 68907
[Chain] # = 58646
::find-nn: 0.003824
::link-update: 0.011757
::update-clusters: 0.0043139

Round 2
Parlay time: find-nn: 0.0078
Parlay time: link-update: 0.0316
Parlay time: update-clusters: 0.0051

[CLINK] Round = 3
[CLINK] Comp Num = 50761
[Chain] # = 39165
::find-nn: 0.002579
::link-update: 0.008425
::update-clusters: 0.0038879

Round 3
Parlay time: find-nn: 0.0063
Parlay time: link-update: 0.0380
Parlay time: update-clusters: 0.0016

[CLINK] Round = 4
[CLINK] Comp Num = 38471
[Chain] # = 27973
::find-nn: 0.0019531
::link-update: 0.0057509
::update-clusters: 0.003794

Round 4
Parlay time: find-nn: 0.0039
Parlay time: link-update: 0.0315
Parlay time: update-clusters: 0.0081


[CLINK] rounds = 55
::format-dendro: 0.002044
::CLINK: 0.18838
[CLINK] Edge Num = 99999
[CLINK] COST = 1512346556.4683246613
PBBS-time: 0.189

ward

./linkage /home/ubuntu/datasets/2D_GaussianDisc_100K.pbbs 2
./linkage -method ward -r 1 -d 2 -cachesize 1 /home/ubuntu/datasets/2D_GaussianDisc_100K.pbbs

build 0.0218651294708251953125
Parlay time: build: 0.0058 -- build is faster in general
[CLINK] Round = 1
[CLINK] Comp Num = 100000
[Chain] # = 100000
::init-chains: 0.010894
::link-update: 0.012254
1  0.0001049
pack  0.00022388
build  0.0048568
::update-clusters: 0.0061822

Parlay time: find-nn: 1.0897  - find is slow!!
Parlay time: link-update: 0.0031 
Parlay time: 1: 0.0018
Parlay time: pack: 0.0003
Parlay time: build: 0.0120   - tree build is slow, compared to rebuild
Parlay time: update-clusters: 0.0159


[CLINK] Round = 2
[CLINK] Comp Num = 68907
[Chain] # = 58646
::find-nn: 0.003891
::link-update: 0.0072181
1  7.7963e-05
pack  0.00011301
build  0.003799
::update-clusters: 0.0046899

Parlay time: find-nn: 0.0088
Parlay time: link-update: 0.0014
Parlay time: 1: 0.0000
Parlay time: pack: 0.0001
Parlay time: build: 0.0074
Parlay time: update-clusters: 0.0050

[CLINK] Round = 3
[CLINK] Comp Num = 50786
[Chain] # = 39485
::find-nn: 0.0029531
::link-update: 0.0050359
::update-clusters: 0.0042109

Parlay time: find-nn: 0.0065
Parlay time: link-update: 0.0013
Parlay time: update-clusters: 0.0059



[CLINK] rounds = 53
::format-dendro: 0.002146
::CLINK: 0.15413
[CLINK] Edge Num = 99999
[CLINK] COST = 18524184.6591872908
PBBS-time: 0.155

Parlay time: done: 1.5430
Parlay time: finish: 1.6868

Parlay time: done: 0.0685
Parlay time: finish: 0.2837
Cost: 18524184.66