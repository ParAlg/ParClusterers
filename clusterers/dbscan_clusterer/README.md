### build

bazel build //clusterers:cluster-in-memory-metric_main

### run

bazel run //clusterers:cluster-in-memory-metric_main -- --input_points=data.txt --clusterer_name='MetricDBSCANClusterer' --clusterer_config='metric_dbscan_clusterer_config {epsilon: 1, min_pts: 2}'
