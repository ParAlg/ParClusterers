import sys
import signal
import subprocess
import itertools
from dataclasses import dataclass
from typing import List, Optional


def signal_handler(signal, frame):
    print("bye\n")
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


@dataclass
class SystemConfig:
    """Configuration for system tools"""

    gplusplus_ver: str = ""
    python_ver: str = ""


@dataclass
class ClusterConfig:
    """Configuration for clustering runs"""

    input_directory: str = ""
    output_directory: str = ""
    csv_output_directory: str = ""
    clusterers: List[str] = None
    graphs: List[str] = None
    num_threads: List[str] = None
    clusterer_configs: List[List[str]] = None
    clusterer_config_names: List[str] = None
    num_rounds: int = 1
    timeout: str = ""
    gbbs_format: str = "false"
    weighted: str = "false"
    tigergraph_edges: Optional[str] = None
    tigergraph_nodes: Optional[str] = None
    postprocess_only: str = "false"
    write_clustering: str = "true"

    def __post_init__(self):
        if self.clusterers is None:
            self.clusterers = []
        if self.graphs is None:
            self.graphs = []
        if self.num_threads is None:
            self.num_threads = ["ALL"]
        if self.clusterer_configs is None:
            self.clusterer_configs = []
        if self.clusterer_config_names is None:
            self.clusterer_config_names = []


@dataclass
class StatsConfig:
    """Configuration for statistics computation"""

    communities: List[str] = None
    stats_config: str = ""
    deterministic: str = "false"

    def __post_init__(self):
        if self.communities is None:
            self.communities = []


@dataclass
class GraphConfig:
    """Configuration for graph plotting"""

    x_axis: List[str] = None
    x_axis_index: List[int] = None
    x_axis_modifier: List[str] = None
    y_axis: List[str] = None
    y_axis_index: List[int] = None
    y_axis_modifier: List[str] = None
    legend: List[str] = None
    output_graph_filename: List[str] = None

    def __post_init__(self):
        for attr in [
            "x_axis",
            "x_axis_index",
            "x_axis_modifier",
            "y_axis",
            "y_axis_index",
            "y_axis_modifier",
            "legend",
            "output_graph_filename",
        ]:
            if getattr(self, attr) is None:
                setattr(self, attr, [])


def shellGetOutput(str1):
    process = subprocess.Popen(
        str1,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True,
    )
    output, err = process.communicate()

    if len(err) > 0:
        print(str1 + "\n" + output + err)
    return output


def appendToFile(out, filename):
    with open(filename, "a+") as out_file:
        out_file.writelines(out)


def makeConfigCombos(current_configs):
    config_combos = itertools.product(*current_configs)
    config_combos_formatted = []
    for config in config_combos:
        config_combos_formatted.append(",".join(config))
    return config_combos_formatted


def makeConfigCombosModularity(current_configs):
    config_combos = itertools.product(*current_configs)
    config_combos_formatted = []
    for config in config_combos:
        config_txt = ""
        other_configs = []
        for config_item in config:
            if config_item.startswith("resolution"):
                config_txt += config_item
            else:
                other_configs.append(config_item)
        config_txt += ", correlation_config: {"
        config_txt += ",".join(other_configs)
        config_txt += "}"
        config_combos_formatted.append(config_txt)
    # for i in config_combos_formatted:
    #   print(i)
    # exit(1)
    return config_combos_formatted


def readSystemConfig(filename: str) -> SystemConfig:
    """Read system configuration from file"""
    config = SystemConfig()
    with open(filename, "r") as in_file:
        for line in in_file:
            line = line.strip()
            split = [x.strip() for x in line.split(":")]
            if split:
                if split[0].startswith("g++"):
                    config.gplusplus_ver = split[1]
                elif split[0].startswith("Python"):
                    config.python_ver = split[1]
    return config


def readConfig(filename: str) -> ClusterConfig:
    """Read cluster configuration from file"""
    config = ClusterConfig()

    with open(filename, "r") as in_file:
        for line in in_file:
            line = line.strip()
            split = [x.strip() for x in line.split(":")]
            if split:
                if split[0].startswith("Input directory"):
                    config.input_directory = split[1]
                elif split[0].startswith("Output directory"):
                    config.output_directory = split[1]
                elif split[0].startswith("CSV Output directory"):
                    config.csv_output_directory = split[1]
                elif split[0].startswith("Clusterers"):
                    config.clusterers = [x.strip() for x in split[1].split(";")]
                    config.clusterer_configs = len(config.clusterers) * [None]
                    config.clusterer_config_names = len(config.clusterers) * [None]
                elif split[0].startswith("Graphs"):
                    config.graphs = [x.strip() for x in split[1].split(";")]
                elif split[0].startswith("Number of threads") and len(split) > 1:
                    config.num_threads = [x.strip() for x in split[1].split(";")]
                elif split[0].startswith("Number of rounds") and len(split) > 1:
                    config.num_rounds = 1 if split[1] == "" else int(split[1])
                elif split[0].startswith("Timeout") and len(split) > 1:
                    config.timeout = split[1]
                elif split[0].startswith("GBBS format") and len(split) > 1:
                    config.gbbs_format = split[1]
                elif split[0].startswith("TigerGraph files") and len(split) > 1:
                    tigergraph_files = [x.strip() for x in split[1].split(";")]
                    config.tigergraph_edges = tigergraph_files[0]
                    config.tigergraph_nodes = tigergraph_files[1]
                elif split[0].startswith("TigerGraph nodes") and len(split) > 1:
                    config.tigergraph_nodes = [x.strip() for x in split[1].split(";")]
                elif split[0].startswith("TigerGraph edges") and len(split) > 1:
                    config.tigergraph_edges = [x.strip() for x in split[1].split(";")]
                elif split[0].startswith("Wighted") and len(split) > 1:
                    config.weighted = split[1]
                elif split[0].startswith("Postprocess only"):
                    config.postprocess_only = split[1]
                elif split[0].startswith("Write clustering"):
                    config.write_clustering = split[1]
                else:
                    for index, clusterer_name in enumerate(config.clusterers):
                        if split[0] == clusterer_name:
                            config.clusterer_config_names[index] = (
                                in_file.readline().strip()
                            )
                            current_configs = []
                            next_line = in_file.readline().strip()
                            while next_line != "":
                                arg_name = next_line.split(":", 1)
                                arg_name[0] = arg_name[0].strip()
                                args = [x.strip() for x in arg_name[1].split(";")]
                                current_configs.append(
                                    [arg_name[0] + ": " + x for x in args]
                                )
                                try:
                                    next_line = in_file.readline().strip()
                                except StopIteration as err:
                                    break
                            if clusterer_name == "ParallelModularityClusterer":
                                config.clusterer_configs[index] = (
                                    makeConfigCombosModularity(current_configs)
                                )
                            else:
                                config.clusterer_configs[index] = makeConfigCombos(
                                    current_configs
                                )
                            break

    # Set defaults
    config.num_threads = (
        ["ALL"]
        if config.num_threads is None or not config.num_threads
        else config.num_threads
    )
    config.timeout = (
        ""
        if (config.timeout is None or config.timeout == "" or config.timeout == "NONE")
        else "timeout " + config.timeout
    )
    config.num_rounds = 1 if (config.num_rounds is None) else config.num_rounds
    config.gbbs_format = (
        "false"
        if (config.gbbs_format is None or config.gbbs_format == "")
        else config.gbbs_format
    )
    config.weighted = (
        "false"
        if (config.weighted is None or config.weighted == "")
        else config.weighted
    )

    return config


def readStatsConfig(filename: str) -> StatsConfig:
    """Read stats configuration from file"""
    config = StatsConfig()
    stats_config_list = []

    with open(filename, "r") as in_file:
        for line in in_file:
            line = line.strip()
            split = [x.strip() for x in line.split(":")]
            if split:
                if split[0].startswith("Input communities") and len(split) > 1:
                    config.communities = [x.strip() for x in split[1].split(";")]
                elif split[0].startswith("Deterministic") and len(split) > 1:
                    config.deterministic = split[1]
                elif split[0].startswith("statistics_config"):
                    next_line = in_file.readline().strip()
                    while next_line != "":
                        stats_config_list.append(next_line)
                        try:
                            next_line = in_file.readline().strip()
                        except StopIteration as err:
                            break
                    config.stats_config = ",".join(stats_config_list)

    return config


def readGraphConfig(filename: str) -> GraphConfig:
    """Read graph configuration from file"""
    config = GraphConfig()

    with open(filename, "r") as in_file:
        for line in in_file:
            line = line.strip()
            split = [x.strip() for x in line.split(":")]
            if split:
                if split[0].startswith("x axis"):
                    split = [x.strip() for x in split[1].split(" ")]
                    config.x_axis.append(split[0])
                    if len(split) > 1:
                        config.x_axis_modifier.append(split[1])
                    else:
                        config.x_axis_modifier.append("")
                elif split[0].startswith("y axis"):
                    split = [x.strip() for x in split[1].split(" ")]
                    config.y_axis.append(split[0])
                    if len(split) > 1:
                        config.y_axis_modifier.append(split[1])
                    else:
                        config.y_axis_modifier.append("")
                elif split[0].startswith("Legend"):
                    config.legend.append(split[1])
                elif split[0].startswith("Graph filename"):
                    config.output_graph_filename.append(split[1])

    return config
