import os
import runner_utils
import json
import pandas as pd
import argparse

from stats_precision_recall_pair import compute_precision_recall_pair


def getRunTime(clusterer, out_prefix):
    cluster_time = -1
    out_filename = out_prefix + ".out"

    if clusterer.startswith("TigerGraph"):
        with open(out_filename, "r") as f:
            run_info = f.readlines()
            for elem in run_info[1:]:
                if elem.startswith("Total Time:"):
                    cluster_time = elem.split(" ")[-1].strip()
    elif clusterer.startswith("Snap"):
        with open(out_filename, "r") as f:
            run_info = f.readlines()
            for elem in run_info[1:]:
                if (
                    elem.startswith("Wealy Connected Component Time:")
                    or elem.startswith("KCore Time:")
                    or elem.startswith("Cluster Time:")
                ):
                    cluster_time = elem.split(" ")[-1].strip()
    elif clusterer.startswith("Neo4j"):
        with open(out_filename, "r") as f:
            run_info = f.readlines()
            for elem in run_info[1:]:
                if elem.startswith("Time:"):
                    cluster_time = elem.split(" ")[-1].strip()
    else:  # our cluseterer and Networkit and Tectonic
        with open(out_filename, "r") as f:
            run_info = f.readlines()
            for elem in run_info[1:]:
                if elem.startswith("Cluster Time:"):
                    cluster_time = elem.split(" ")[-1].strip()

    return cluster_time


def runStats(
    out_prefix,
    graph,
    graph_idx,
    stats_dict,
    cluster_config: runner_utils.ClusterConfig,
    stats_config: runner_utils.StatsConfig,
):
    out_statistics = out_prefix + ".stats"
    out_statistics_pair = out_prefix + ".pair.stats"
    in_clustering = out_prefix + ".cluster"
    if not os.path.exists(in_clustering) or not os.path.getsize(in_clustering) > 0:
        # Either an error or a timeout happened
        runner_utils.appendToFile("ERROR", out_statistics)
        return
    use_input_graph = cluster_config.input_directory + graph
    input_communities = (
        cluster_config.input_directory + stats_config.communities[graph_idx]
    )
    if "precision_recall_pair_thresholds" in stats_config.stats_config:
        compute_precision_recall_pair(
            in_clustering,
            input_communities,
            out_statistics_pair,
            stats_config.stats_config,
            stats_dict,
        )
        return
    use_input_communities = (
        ""
        if not stats_config.communities
        else "--input_communities=" + input_communities
    )
    ss = (
        "bazel run //clusterers:stats-in-memory_main -- "
        "--input_graph=" + use_input_graph + " "
        "--is_gbbs_format=" + cluster_config.gbbs_format + " "
        "--float_weighted=" + cluster_config.weighted + " "
        "--input_clustering=" + in_clustering + " "
        "--output_statistics=" + out_statistics + " " + use_input_communities + " "
        "--statistics_config='" + stats_config.stats_config + "'"
    )
    if cluster_config.postprocess_only == "false":
        print(ss)
        out = runner_utils.shellGetOutput(ss)

    out_statistics_file = open(out_statistics, "r")
    out_statistics_string = out_statistics_file.read()
    out_statistics_file.close()
    parse_out_statistics = json.loads(out_statistics_string)
    for k in parse_out_statistics:
        v = parse_out_statistics[k]
        if type(v) != dict:
            stats_dict[k] = v
        else:
            for elem2 in v:
                stats_dict[k + "_" + elem2] = v[elem2]


def runAll(
    cluster_config: runner_utils.ClusterConfig, stats_config: runner_utils.StatsConfig
):
    stats = []
    for clusterer_idx, clusterer in enumerate(cluster_config.clusterers):
        if clusterer == "SKIP":
            continue
        for graph_idx, graph in enumerate(cluster_config.graphs):
            if graph == "SKIP":
                continue
            if clusterer.startswith("Snap"):
                for i in range(cluster_config.num_rounds):
                    if stats_config.deterministic == "true" and i != 0:
                        continue
                    out_prefix = (
                        cluster_config.output_directory
                        + clusterer
                        + "_"
                        + str(graph_idx)
                        + "_"
                        + str(i)
                    )
                    stats_dict = {}
                    stats_dict["Clusterer Name"] = clusterer
                    stats_dict["Input Graph"] = graph
                    stats_dict["Threads"] = 1
                    stats_dict["Config"] = None
                    stats_dict["Round"] = i
                    stats_dict["Cluster Time"] = getRunTime(clusterer, out_prefix)
                    runStats(
                        out_prefix,
                        graph,
                        graph_idx,
                        stats_dict,
                        cluster_config,
                        stats_config,
                    )
                    stats_dict["Ground Truth"] = stats_config.communities[graph_idx]
                    stats.append(stats_dict)
                continue
            for thread_idx, thread in enumerate(cluster_config.num_threads):
                if (
                    stats_config.deterministic == "true"
                    and thread != cluster_config.num_threads[0]
                ):
                    continue
                configs = (
                    cluster_config.clusterer_configs[clusterer_idx]
                    if cluster_config.clusterer_configs[clusterer_idx] is not None
                    else [""]
                )
                config_prefix = (
                    cluster_config.clusterer_config_names[clusterer_idx] + "{"
                    if cluster_config.clusterer_configs[clusterer_idx] is not None
                    else ""
                )
                config_postfix = (
                    "}"
                    if cluster_config.clusterer_configs[clusterer_idx] is not None
                    else ""
                )
                for config_idx, config in enumerate(configs):
                    for i in range(cluster_config.num_rounds):
                        if stats_config.deterministic == "true" and i != 0:
                            continue
                        out_prefix = (
                            cluster_config.output_directory
                            + clusterer
                            + "_"
                            + str(graph_idx)
                            + "_"
                            + thread
                            + "_"
                            + str(config_idx)
                            + "_"
                            + str(i)
                        )
                        try:
                            stats_dict = {}
                            stats_dict["Clusterer Name"] = clusterer
                            stats_dict["Input Graph"] = graph
                            stats_dict["Threads"] = thread
                            stats_dict["Config"] = config
                            stats_dict["Round"] = i
                            stats_dict["Cluster Time"] = getRunTime(
                                clusterer, out_prefix
                            )
                            runStats(
                                out_prefix,
                                graph,
                                graph_idx,
                                stats_dict,
                                cluster_config,
                                stats_config,
                            )
                            stats_dict["Ground Truth"] = stats_config.communities[
                                graph_idx
                            ]
                            stats.append(stats_dict)
                        except FileNotFoundError:
                            print("Failed because file not found, ", out_prefix)
    stats_dataframe = pd.DataFrame(stats)
    if not os.path.exists(cluster_config.csv_output_directory):
        os.makedirs(cluster_config.csv_output_directory)
    stats_dataframe.to_csv(cluster_config.csv_output_directory + "/stats.csv", mode="a")


def main():
    parser = argparse.ArgumentParser(
        description="Compute statistics for clustering results"
    )
    parser.add_argument("config_file", help="Path to the cluster configuration file")
    parser.add_argument(
        "stats_config_file", help="Path to the stats configuration file"
    )

    args = parser.parse_args()

    # Read configurations
    cluster_config = runner_utils.readConfig(args.config_file)
    stats_config = runner_utils.readStatsConfig(args.stats_config_file)

    runAll(cluster_config, stats_config)


if __name__ == "__main__":
    main()
