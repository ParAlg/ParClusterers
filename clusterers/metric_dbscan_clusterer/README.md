### build

bazel build //clusterers:cluster-in-memory-metric_main

### run

bazel run //clusterers:cluster-in-memory-metric_main -- --input_points=/home/yiqiuw/datasets/2D_UniformFill_10M.lines --clusterer_name='MetricDBSCANClusterer' --clusterer_config='metric_dbscan_clusterer_config {epsilon: 1, min_pts: 2}'
