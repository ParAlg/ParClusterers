import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np
import plotting_utils 
from plotting_utils import *

plt.rcParams["ps.useafm"] = True
plt.rcParams["pdf.use14corefonts"] = True
plt.rcParams["text.usetex"] = True
# sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
fontsize = 25


def plotParetoAxis(ax, dfs, graph, lines, labels, clusterers):
    for clusterer in clusterers:
        # Extract the pareto_df for the current graph and clusterer combination
        _, pareto_df = dfs[(graph, clusterer)]
        if pareto_df.empty:
            #                         print(graph, clusterer)
            continue

        # Plot the pareto_df with the appropriate marker
        (line,) = ax.plot(
            pareto_df["Cluster Time"],
            pareto_df["fScore_mean"],
            label=clusterer,
            color=color_map[clusterer],
            marker=style_map[clusterer],
            markersize=16,
            linewidth=2,
        )

        shortened_clusterer = clusterer.replace("Clusterer", "")
        # If the clusterer's line hasn't been added to lines, add it
        if shortened_clusterer not in labels:
            lines.append(line)
            labels.append(shortened_clusterer)

    ax.set_xscale("log")
    ax.set_title(f"{graph}")
    ax.set_xlabel("Clustering Time")
    ax.set_ylabel("Mean $F_{0.5}$ Score")


def plotPareto(dfs, graphs, clusterers, draw_legend=True, ncol=6):
    num_graphs = len(graphs)
    if num_graphs < 4:
        plt.rcParams.update({"font.size": 25})

        fig, axes = plt.subplots(nrows=1, ncols=num_graphs, figsize=(16, 8))
        graph_idx = 0

        lines = []  # To store the Line2D objects for the legend
        labels = []  # To store the corresponding labels for the Line2D objects

        for graph_idx in range(num_graphs):
            graph = graphs[graph_idx]
            ax = axes[graph_idx]
            plotParetoAxis(ax, dfs, graph, lines, labels, clusterers)
            graph_idx += 1

        if draw_legend:
            # Create a single legend for the entire figure, at the top
            fig.legend(
                lines,
                labels,
                loc="upper center",
                ncol=ncol,
                bbox_to_anchor=(0.5, 1.15),
                frameon=False,
            )
    else:
        plt.rcParams.update({"font.size": 20})

        fig, axes = plt.subplots(nrows=1, ncols=4, figsize=(25, 5))
        graph_idx = 0

        lines = []  # To store the Line2D objects for the legend
        labels = []  # To store the corresponding labels for the Line2D objects

        for graph_idx in range(4):
            if graph_idx < num_graphs:  # Ensure we have a graph to process
                graph = graphs[graph_idx]
                ax = axes[graph_idx]
                plotParetoAxis(ax, dfs, graph, lines, labels, clusterers)
                graph_idx += 1
            else:
                axes[graph_idx].axis("off")  # Turn off axes without data
        if draw_legend:
            # Create a single legend for the entire figure, at the top
            fig.legend(
                lines,
                labels,
                loc="upper center",
                ncol=6,
                bbox_to_anchor=(0.5, 1.15),
                frameon=False,
            )

    return fig


def plotPRParetoAX(ax, graph, df, clusterers, lines, labels, only_high_p=False):
    for clusterer in clusterers:
        # Extract the pareto_df for the current graph and clusterer combination
        pareto_df = df[
            (df["Clusterer Name"] == clusterer) & (df["Input Graph"] == graph)
        ]
        if pareto_df.empty:
            continue

        # Plot the pareto_df with the appropriate marker
        (line,) = ax.plot(
            pareto_df["communityPrecision_mean"],
            pareto_df["communityRecall_mean"],
            label=clusterer,
            color=color_map[clusterer],
            marker=style_map[clusterer],
            markersize=16,
            linewidth=2,
        )

        shortened_clusterer = clusterer.replace("Clusterer", "")
        # If the clusterer's line hasn't been added to lines, add it
        if shortened_clusterer not in labels:
            lines.append(line)
            labels.append(shortened_clusterer)

    ax.set_title(f"{graph}")
    ax.set_xlabel("Precision")
    ax.set_ylabel("Recall")
    if only_high_p:
        ax.set_xlim((0.5, 1))


def plotPRPareto(df, only_high_p=False, ncol=6):
    graphs = df["Input Graph"].unique()
    clusterers = df["Clusterer Name"].unique()

    graph_idx = 0

    lines = []  # To store the Line2D objects for the legend
    labels = []  # To store the corresponding labels for the Line2D objects

    if len(graphs) < 4:
        plt.rcParams.update({"font.size": 25})
        fig, axes = plt.subplots(nrows=1, ncols=len(graphs), figsize=(16, 8))
        for graph_idx in range(len(graphs)):
            graph = graphs[graph_idx]
            ax = axes[graph_idx]

            plotPRParetoAX(ax, graph, df, clusterers, lines, labels, only_high_p)

            graph_idx += 1

        plt.tight_layout()
        fig.subplots_adjust(hspace=0.4)
        fig.legend(
            lines,
            labels,
            loc="upper center",
            ncol=4,
            bbox_to_anchor=(0.5, 1.2),
            frameon=False,
        )
        return axes

    else:
        plt.rcParams.update({"font.size": 20})
        fig, axes = plt.subplots(nrows=1, ncols=4, figsize=(25, 5))
        for graph_idx in range(len(graphs)):
            graph = graphs[graph_idx]
            ax = axes[graph_idx]

            plotPRParetoAX(ax, graph, df, clusterers, lines, labels, only_high_p)

            graph_idx += 1

        fig.legend(
            lines,
            labels,
            loc="upper center",
            ncol=ncol,
            bbox_to_anchor=(0.5, 1.15),
            frameon=False,
        )
        return axes


def load_all_dfs():
    df_mnist = pd.read_csv(base_addr + "weighted_results/stats_mnist.csv")
    df_mnist_100 = pd.read_csv(base_addr + "weighted_results/stats_mnist_100.csv")
    df_imagenet = pd.read_csv(base_addr + "weighted_results/stats_imagenet.csv")
    df_10 = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_10.csv")
    df_50 = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_50.csv")
    df_100 = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_100.csv")
    df_more = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_more.csv")
    df_mod = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_mod.csv")
    df_mod_more = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_mod_more.csv")


    df_nk = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_50_nk.csv")
    df_neo4j = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_neo4j.csv")
    df_tg = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_tg.csv")
    df_snap = pd.read_csv(base_addr + "weighted_results/stats_large_weighted_snap.csv")

    df = pd.concat([df_mnist, df_mnist_100, df_imagenet, df_10, df_50, df_mod, df_mod_more,
                    df_100, df_more, df_nk, df_neo4j, df_tg, df_snap])
    # kcore does not work well because it's knn graph
    df = df[df["Clusterer Name"]!="KCoreClusterer"]

    df = df.dropna(how='all')
    replace_graph_names(df)
    df = add_epsilon_to_hac(df)
    return df


def plot_weighted():
    df = load_all_dfs()
    our_methods = plotting_utils.get_our_methods()
    
    datasets = ["imagenet_k50", "reddit-clustering_k50"]
    subset = "_subset"
    # "mnist_k50", "stackexchange-clustering_k50"

    df_50 = df[df["Input Graph"].isin(datasets)]
    df_ours = df_50[df_50["Clusterer Name"].isin(our_methods)]

    df_pr_pareto = FilterParetoPRMethod(df_ours)
    # getAUCTable(df_ours, df_pr_pareto)
    axes = plotPRPareto(df_pr_pareto, True, ncol=6)
    axes[1].set_ylim((0, 0.7))
    plt.savefig(f"./results/pr_large_weighted{subset}.pdf", bbox_inches='tight')
    print(f"plotted ./results/pr_large_weighted{subset}.pdf")

    clusterers = df_ours["Clusterer Name"].unique()
    dfs, graphs = GetParetoDfs(df_ours)
    plotPareto(dfs, graphs, clusterers, False)
    plt.tight_layout()
    plt.savefig(f"./results/time_f1_large_weighted{subset}.pdf", bbox_inches='tight')    
    print(f"plotted ./results/time_f1_large_weighted{subset}.pdf")

    baseline_methods = get_baseline_methods()
    df_compare = df_50[df_50["Clusterer Name"].isin(["ParallelModularityClusterer", 
                                            "ParallelCorrelationClusterer"]+baseline_methods)]
    df_compare = df_compare[df_compare["Input Graph"].isin(datasets)]
    clusterers = df_compare["Clusterer Name"].unique()
    dfs, graphs = GetParetoDfs(df_compare)
    plotPareto(dfs, graphs, clusterers, True, ncol=4)
    plt.tight_layout()
    plt.savefig(f"./results/time_f1_weighted_modularity{subset}.pdf", bbox_inches='tight')
    print(f"plotted ./results/time_f1_weighted_modularity{subset}.pdf")

if __name__ == "__main__":
    base_addr = "/Users/sy/Desktop/MIT/clusterer/csv/"
    plot_weighted()
