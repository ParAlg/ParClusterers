import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np
from plotting_utils import *

plt.rcParams["ps.useafm"] = True
plt.rcParams["pdf.use14corefonts"] = True
plt.rcParams["text.usetex"] = True
fontsize = 30

import matplotlib.colors as mcolors

default_blue = mcolors.to_hex(mcolors.TABLEAU_COLORS["tab:blue"])
default_orange = mcolors.to_hex(mcolors.TABLEAU_COLORS["tab:orange"])


def plot_runtime_compare(df, base, filename, title, ncol=5, height=5, p=False):
    df_avg = df.groupby(["Clusterer Name", "Input Graph"])["Cluster Time"].mean().reset_index()

    ## compute "speedup", actually the slowdown
    df_base = df_avg[df_avg["Clusterer Name"] == base]
    df_base = df_base.rename(columns={"Cluster Time": "Base Time"})
    df_base = df_base.drop(columns=["Clusterer Name"])
    df_avg = df_avg.merge(df_base, on=["Input Graph"])
    df_avg["Speedup"] = df_avg["Cluster Time"] / df_avg["Base Time"]
    if p:
        print(df_avg)

    plt.figure(figsize=(12, height))
    # order the bars, so the base method is the first one
    methods = df_avg["Clusterer Name"].unique().tolist()
    methods.remove(base)
    methods = [base] + methods
    ax = sns.barplot(
        data=df_avg,
        x="Input Graph",
        y="Speedup",
        hue="Clusterer Name",
        hue_order=methods,
        palette=sns.color_palette("Set2"),
        order=["AM", "YT", "DB", "LJ", "OK", "FS"],
    )

    ax.axhline(y=1, linestyle="--", color="black")

    hatch_patterns = ["/", "x", "o", "\\", ".", "+", "O", "*"]
    num_methods = len(methods)
    num_graphs = len(df_avg["Input Graph"].unique().tolist())

    for i, bar in enumerate(ax.patches):
        hatch_pattern = hatch_patterns[i // num_graphs]
        bar.set_hatch(hatch_pattern)

    plt.ylabel("Slowdown", fontsize=fontsize - 5)
    plt.xlabel("")

    plt.yscale("log")
    #     plt.yticks([1, 10, 100, 1e3, 1e4])
    #     plt.legend(loc='upper right')
    if ncol == 5:
        y_anchor = 1.2
    elif ncol == 4:
        y_anchor = 1.5
    elif ncol == 3:
        y_anchor = 1.9

    handles, labels = plt.gca().get_legend_handles_labels()
    plt.legend(
        handles,
        labels,
        loc="upper center",
        ncol=ncol,
        bbox_to_anchor=(0.5, y_anchor),
        frameon=False,
        fontsize=fontsize - 5,
        handletextpad=0.3,
        columnspacing=0.6,
    )
    set_ax_ticks(ax)
    plt.title(title, fontsize=fontsize - 5, y=y_anchor - 0.05)

    plt.tight_layout()
    plt.savefig(filename)


if __name__ == "__main__":
    base_addr = "results/"
    our_name = "PCBS"
    directories = ["nk", "pcbs", "snap", "nk_fs", "neo4j", "tg"]
    df = pd.concat([pd.read_csv(base_addr + f"out_time_{f}_csv/runtimes.csv") for f in directories])
    replace_graph_names(df)

    ##  "Connectivity"
    df_wcc = df[(df["Clusterer Name"].str.contains("Connectivity")) | (df["Clusterer Name"].str.contains("WCC"))]
    df_wcc.loc[:, "Clusterer Name"] = df_wcc["Clusterer Name"].str.replace("ConnectivityClusterer", our_name)
    df_wcc.loc[:, "Clusterer Name"] = df_wcc["Clusterer Name"].str.replace("Connectivity", "")
    plot_runtime_compare(df_wcc, our_name, base_addr + "out_wcc_slowdown.pdf", "Connectivity", 5)

    ## KCore
    df_kcore = df[df["Clusterer Name"].str.contains("KCore")]
    df_kcore.loc[:, "Clusterer Name"] = df_kcore["Clusterer Name"].str.replace("KCoreClusterer", our_name)
    df_kcore.loc[:, "Clusterer Name"] = df_kcore["Clusterer Name"].str.replace("KCore", "")
    plot_runtime_compare(df_kcore, our_name, base_addr + "out_kcore_slowdown.pdf", "KCore")

    ## LP
    df_lp = df[(df["Clusterer Name"].str.contains("Label")) | (df["Clusterer Name"].str.contains("PLP"))]
    df_lp = df_lp[~df_lp["Clusterer Name"].str.contains("SLLabelProp")]  # remove tigergraph SLPA
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelPropagationClusterer", our_name)
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelPropagation", "")
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("PLP", "")
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelProp", "")
    plot_runtime_compare(df_lp, our_name, base_addr + "out_lp_slowdown.pdf", "Label Propagation", p=True)

    ## SLPA
    df_slpa = df[(df["Clusterer Name"].str.contains("SLPA")) | (df["Clusterer Name"].str.contains("SLLabel"))]
    df_slpa.loc[:, "Clusterer Name"] = df_slpa["Clusterer Name"].str.replace("SLPAClusterer", our_name)
    df_slpa.loc[:, "Clusterer Name"] = df_slpa["Clusterer Name"].str.replace("SLPA", "")
    df_slpa.loc[:, "Clusterer Name"] = df_slpa["Clusterer Name"].str.replace("SLLabelProp", "")
    plot_runtime_compare(df_slpa, our_name, base_addr + "out_slpa_slowdown.pdf", "Speaker-Listern Label Propagation")

    ## Modularity
    df_md = df[
        (df["Clusterer Name"].str.contains("Modularity"))
        | (df["Clusterer Name"].str.contains("PLM"))
        | (df["Clusterer Name"].str.contains("Louvain"))
        | (df["Clusterer Name"].str.contains("Leiden"))
    ]
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("ParallelModularityClusterer", our_name)
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("ModularityOptimization", "MO")
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("Parallel", "")
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("PLM", "Louvain")
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("Louvain", "LV")
    df_md.loc[:, "Clusterer Name"] = df_md["Clusterer Name"].str.replace("Leiden", "LD")
    plot_runtime_compare(df_md, our_name, base_addr + "out_md_slowdown.pdf", "Modularity Clustering", 4, 6)
