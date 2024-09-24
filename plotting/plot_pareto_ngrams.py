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
# plt.rcParams["text.usetex"] = True
# sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
fontsize = 25


def GetParetoDfs(df):
    dfs = {}
    clusterers = df["Clusterer Name"].unique()

    graphs = df["Input Graph"].unique()
    for graph in graphs:
        for clusterer in clusterers:
            filtered_df = df[df["Input Graph"] == graph]
            filtered_df = filtered_df[filtered_df["Clusterer Name"] == clusterer]

            filtered_df = filtered_df.sort_values(by=["Cluster Time"])
            pareto_frontier = []
            max_score = float("-inf")

            for _, row in filtered_df.iterrows():
                score = row["fScore_mean"]
                if score > max_score:
                    max_score = score
                    pareto_frontier.append(row)

            pareto_df = pd.DataFrame(pareto_frontier)

            dfs[(graph, clusterer)] = (filtered_df, pareto_df)
    return dfs, graphs


## only leave methods that's on the overall pareto frontier
def filterMethodsOnOverallPareto(df):
    graphs = df["Input Graph"].unique()
    dfs = []
    for graph in graphs:
        filtered_df = df[df["Input Graph"] == graph]

        filtered_df = filtered_df.sort_values(by=["Cluster Time"])
        pareto_frontier = []
        max_score = float("-inf")

        for _, row in filtered_df.iterrows():
            score = row["fScore_mean"]
            if score > max_score:
                max_score = score
                pareto_frontier.append(row)

        pareto_df = pd.DataFrame(pareto_frontier)
        methods = pareto_df["Clusterer Name"].unique()
        df_graph = filtered_df[filtered_df["Clusterer Name"].isin(methods)]
        dfs.append(df_graph)
    df = pd.concat(dfs)
    return df


# filter out methods on the overrall Pareto frontier of all methods
def FilterParetoPR(df, by_method=False):
    graphs = df["Input Graph"].unique()
    dfs = []
    for graph in graphs:
        filtered_df = df[df["Input Graph"] == graph]

        filtered_df = filtered_df.sort_values(
            by=["communityPrecision_mean"], ascending=False
        )
        pareto_frontier = []
        max_score = float("-inf")

        for _, row in filtered_df.iterrows():
            score = row["communityRecall_mean"]
            if score > max_score:
                max_score = score
                pareto_frontier.append(row)

        pareto_df = pd.DataFrame(pareto_frontier)
        methods = pareto_df["Clusterer Name"].unique()
        df_graph = filtered_df[filtered_df["Clusterer Name"].isin(methods)]
        if by_method:
            dfs.append(df_graph)
        else:
            dfs.append(pareto_df)
    dfnew = pd.concat(dfs)
    return dfnew


## for each method, find the pareto frontier of precision-recall line
def FilterParetoPRMethod(df):
    graphs = df["Input Graph"].unique()
    dfs = []
    for graph in graphs:
        methods = df["Clusterer Name"].unique()
        for method in methods:
            filtered_df = df[
                (df["Input Graph"] == graph) & (df["Clusterer Name"] == method)
            ]
            filtered_df = filtered_df.sort_values(
                by=["communityPrecision_mean"], ascending=False
            )
            pareto_frontier = []
            max_score = float("-inf")

            for _, row in filtered_df.iterrows():
                score = row["communityRecall_mean"]
                if score > max_score:
                    max_score = score
                    pareto_frontier.append(row)

            pareto_df = pd.DataFrame(pareto_frontier)
            #             methods = pareto_df["Clusterer Name"].unique()
            #             df_graph = filtered_df[filtered_df["Clusterer Name"].isin(methods)]
            dfs.append(pareto_df)
    dfnew = pd.concat(dfs)
    return dfnew


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

    if len(graphs) > 4:
        plt.rcParams.update({"font.size": 25})

        # Create subplots in a 2x3 grid
        fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(22, 15))
        graph_idx = 0

        lines = []  # To store the Line2D objects for the legend
        labels = []  # To store the corresponding labels for the Line2D objects

        for i in range(2):
            for j in range(3):
                if graph_idx < len(graphs):  # Ensure we have a graph to process
                    graph = graphs[graph_idx]
                    ax = axes[i][j]
                    plotParetoAxis(ax, dfs, graph, lines, labels, clusterers)
                    graph_idx += 1
                else:
                    axes[i][j].axis("off")  # Turn off axes without data
        # Create a single legend for the entire figure, at the top
        fig.legend(
            lines,
            labels,
            loc="upper center",
            ncol=ncol,
            bbox_to_anchor=(0.5, 1.1),
            frameon=False,
        )
    else:
        # Create subplots in a 2x3 grid
        plt.rcParams.update({"font.size": 20})

        fig, axes = plt.subplots(nrows=1, ncols=4, figsize=(25, 5))
        graph_idx = 0

        lines = []  # To store the Line2D objects for the legend
        labels = []  # To store the corresponding labels for the Line2D objects

        for graph_idx in range(4):
            if graph_idx < len(graphs):  # Ensure we have a graph to process
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

    if len(graphs) > 4:
        plt.rcParams.update({"font.size": 25})
        fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(30, 16))
        for i in range(2):
            for j in range(3):
                if graph_idx < len(graphs):  # Ensure we have a graph to process
                    graph = graphs[graph_idx]
                    ax = axes[i][j]

                    plotPRParetoAX(
                        ax, graph, df, clusterers, lines, labels, only_high_p
                    )

                    graph_idx += 1
                else:
                    axes[i][j].axis("off")  # Turn off axes without data

        fig.legend(
            lines,
            labels,
            loc="upper center",
            ncol=ncol,
            bbox_to_anchor=(0.5, 1),
            frameon=False,
        )
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


def plotPRParetoSingle(df, graph):
    plt.rcParams.update({"font.size": 20})
    clusterers = df["Clusterer Name"].unique()

    lines = []  # To store the Line2D objects for the legend
    labels = []  # To store the corresponding labels for the Line2D objects

    fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8, 8))

    plotPRParetoAX(ax, graph, df, clusterers, lines, labels)

    fig.subplots_adjust(hspace=0.4)
    fig.legend(
        lines,
        labels,
        loc="upper left",
        ncol=1,
        bbox_to_anchor=(0.9, 0.8),
        frameon=False,
    )


# compute the area under the precision recall pareto curve, for precision >= 0.5.
def computeAUC(df_pr_pareto, clusterer, graph):
    df = df_pr_pareto[
        (df_pr_pareto["Clusterer Name"] == clusterer)
        & (df_pr_pareto["Input Graph"] == graph)
    ][["communityPrecision_mean", "communityRecall_mean"]]

    # Filter the DataFrame to include only precision values in the range [0.5, 1]
    filtered_df = df[df["communityPrecision_mean"] >= 0.5]

    if len(filtered_df) == 0:
        return 0

    # Find the row with the smallest precision in the filtered DataFrame
    min_precision_row = filtered_df[
        filtered_df["communityPrecision_mean"]
        == filtered_df["communityPrecision_mean"].min()
    ]

    # Extract the recall value corresponding to the smallest precision
    recall_value = min_precision_row["communityRecall_mean"].values[0]

    # Create a new row with precision = 0.5 and recall = recall_value
    new_row = pd.DataFrame(
        {"communityPrecision_mean": [0.5, 1], "communityRecall_mean": [recall_value, 0]}
    )

    # Concatenate the new row to the DataFrame
    filtered_df = pd.concat([filtered_df, new_row], ignore_index=True)

    # Sort the filtered DataFrame by 'communityPrecision_mean'
    filtered_df.sort_values(by="communityPrecision_mean", inplace=True)

    # Calculate the area under the curve using the trapezoidal rule
    area = np.trapz(
        filtered_df["communityRecall_mean"], filtered_df["communityPrecision_mean"]
    )

    return area


def getAUCTable(df, df_pr_pareto, print_table=False):
    graphs = df["Input Graph"].unique()
    methods = df["Clusterer Name"].unique()
    data = {}
    for graph in graphs:
        aucs = []
        for method in methods:
            auc = computeAUC(df_pr_pareto, method, graph)
            aucs.append(2 * auc)  ## times 2 to make the score between 0 and 1
        data[graph] = aucs
    data["method"] = [
        method.replace("Clusterer", "")
        .replace("_", "-")
        .replace("Parallel", "")
        .replace("LabelPropagation", "LP")
        for method in methods
    ]
    df_auc = pd.DataFrame(data)

    # Set the 'method' column as the index
    df_auc.set_index("method", inplace=True)
    df_auc["avg"] = df_auc.mean(axis=1)
    df_auc = df_auc.sort_values(by="avg", ascending=False)

    bold_df = df_auc.apply(
        lambda x: [
            (
                "\\textbf{{ {:.2f} }}".format(val)
                if val == x.max()
                else "{:.2f}".format(val)
            )
            for val in x
        ]
    )

    # Convert the DataFrame with bold formatting to LaTeX
    latex_table = bold_df.to_latex(escape=False)
    if print_table:
        print(bold_df)

    # Print the LaTeX table
    print(latex_table)


base_addr = "./results/"


def plot_ngrams():
    threshold = 0.92
    df_pcbs = pd.read_csv(base_addr + f"out_ngrams_{threshold}_pcbs_csv/stats.csv")
    df = df_pcbs

    df = df.dropna(how="all")
    replace_graph_names(df)
    df = add_epsilon_to_hac(df)

    our_methods = [
        "KCoreClusterer",
        "LDDClusterer",
        "SCANClusterer",
        "LabelPropagationClusterer",
        "SLPAClusterer",
        "TECTONICClusterer",
        "ParallelAffinityClusterer",
        "ConnectivityClusterer",
        "ParallelModularityClusterer",
        "ParallelCorrelationClusterer",
        "ParHACClusterer_0.01",
        "ParHACClusterer_0.1",
        "ParHACClusterer_1",
    ]

    df_pcbs = df[df["Clusterer Name"].isin(our_methods)]

    # Get AUC table
    df_pr_pareto = FilterParetoPRMethod(df_pcbs)
    getAUCTable(df_pcbs, df_pr_pareto)

    # Plot Precision Recall Pareto frontier for PCBS methods
    plotPRPareto(df_pr_pareto)
    plt.savefig(base_addr + f"pr_uci_{threshold}.pdf", bbox_inches="tight")
    print("plotted pr_uci.pdf")

    # Plot F_0.5 runtime Pareto frontier for PCBS methods
    clusterers = df_pcbs["Clusterer Name"].unique()
    dfs, graphs = GetParetoDfs(df_pcbs)
    plotPareto(dfs, graphs, clusterers)
    plt.tight_layout()
    plt.savefig(base_addr + f"time_f1_uci_{threshold}.pdf", bbox_inches="tight")
    print("plotted time_f1_uci.pdf")

if __name__ == "__main__":
    base_addr = "results/"
    plot_ngrams()
