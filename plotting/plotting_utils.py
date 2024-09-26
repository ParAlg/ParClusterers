import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np

fontsize=25

def replace_graph_names(df):
    df.loc[:, "Input Graph"] = df["Input Graph"].str.replace("com-", "", regex=True)\
                                    .str.replace(".gbbs.txt", "", regex=True)\
                                    .str.replace(".bin", "", regex=True)\
                                    .str.replace(".ungraph.txt", "", regex=True)\
                                    .str.replace(".graph.txt", "", regex=True)
    
    df.loc[:, "Input Graph"] = df["Input Graph"].str.replace("amazon", "AM", regex=True)\
                                    .str.replace("youtube", "YT", regex=True)\
                                    .str.replace("dblp", "DB", regex=True)\
                                    .str.replace("lj", "LJ", regex=True)\
                                    .str.replace("orkut", "OK", regex=True)\
                                    .str.replace("friendster", "FS", regex=True)

    df.loc[:, "Clusterer Name"] = df["Clusterer Name"].str.replace("Scan", "SCAN", regex=True)\
                                    .str.replace("Tectonic", "TECTONIC", regex=True)\
                                    .str.replace("Hac", "HAC", regex=True)

def set_face_grid_ax_ticks(g):
    """Set fontsize for x and y axis ticks for all axes in a FacetGrid."""
    for ax in g.axes.flat:
        set_ax_ticks(ax)


def set_ax_ticks(ax):
    for l in ax.yaxis.get_ticklabels():
        l.set_fontsize(fontsize)
    for l in ax.xaxis.get_ticklabels():
        l.set_fontsize(fontsize)
        
def common_ax_style(ax):
    ax.set_xticks([1, 4, 8, 16, 30, 60])
    ax.set_xticklabels([1, 4, 8, 16, 30, "30h"])
    
    ax.set_yscale('log')
    # plt.yticks([0.03, 0.1, 0.3, 1])
    # ax.get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

    set_ax_ticks(ax)
        
    ax.set_xlabel("Threads", fontsize = fontsize)

def get_larger_handles(g, marker_size, ncol):
    g.add_legend(loc='upper center', ncol=ncol, bbox_to_anchor=(0.5, 1.15), fontsize=fontsize)
    if g._legend:
        for leg_handle in g._legend.legendHandles:
            leg_handle._sizes = [marker_size]

# Function to extract epsilon value from the config
def extract_epsilon(config):
    parts = config.split(',')
    for part in parts:
        if 'epsilon' in part:
            return part.split(':')[-1].strip()
    return None

def postpend_epsilon(df):
    df.loc[:, 'Clusterer Name'] = df.apply(lambda row: f"{row['Clusterer Name']}_{extract_epsilon(row['Config'])}", axis=1)

def add_epsilon_to_hac(df, remove_zero_eps = True):
    df_no_parhac = df[df["Clusterer Name"] != "ParHACClusterer"]
    df_parhac = df[df["Clusterer Name"] == "ParHACClusterer"]

    # split parhac to different methods for different epsilon
    postpend_epsilon(df_parhac)
    
    if remove_zero_eps:
        df_parhac = df_parhac[df_parhac["Clusterer Name"] != "ParHACClusterer_0"]

    df_new = pd.concat([df_no_parhac, df_parhac])
    
    return df_new


color_map = {'LDDClusterer': (0.12156862745098039,
  0.4666666666666667,
  0.7058823529411765,
  1.0),
 'SCANClusterer': (0.6823529411764706,
  0.7803921568627451,
  0.9098039215686274,
  1.0),
 'LabelPropagationClusterer': (1.0,
  0.4980392156862745,
  0.054901960784313725,
  1.0),
 'SLPAClusterer': (1.0, 0.7333333333333333, 0.47058823529411764, 1.0),
 'TECTONICClusterer': (0.17254901960784313,
  0.6274509803921569,
  0.17254901960784313,
  1.0),
 'ParallelAffinityClusterer': (0.596078431372549,
  0.8745098039215686,
  0.5411764705882353,
  1.0),
 'NetworKitPLM': (0.8392156862745098,
  0.15294117647058825,
  0.1568627450980392,
  1.0),
 'NetworKitParallelLeiden': (1.0, 0.596078431372549, 0.5882352941176471, 1.0),
 'ConnectivityClusterer': (0.5803921568627451,
  0.403921568627451,
  0.7411764705882353,
  1.0),
 'ParallelModularityClusterer': (0.7725490196078432,
  0.6901960784313725,
  0.8352941176470589,
  1.0),
 'ParallelCorrelationClusterer': (0.5490196078431373,
  0.33725490196078434,
  0.29411764705882354,
  1.0),
 'Neo4jLouvain': (0.7686274509803922,
  0.611764705882353,
  0.5803921568627451,
  1.0),
 'Neo4jLeiden': (0.12156862745098039,
  0.4666666666666667,
  0.7058823529411765,
  1.0),
 'Neo4jModularityOptimization': (0.6823529411764706,
  0.7803921568627451,
  0.9098039215686274,
  1.0),
 'SnapCNM': (1.0, 0.4980392156862745, 0.054901960784313725, 1.0),
 'SnapGirvanNewman': (1.0, 0.7333333333333333, 0.47058823529411764, 1.0),
 'SnapInfomap': (0.17254901960784313,
  0.6274509803921569,
  0.17254901960784313,
  1.0),
 'TigerGraphLouvain': (0.596078431372549,
  0.8745098039215686,
  0.5411764705882353,
  1.0),
 'ParHACClusterer_0.01': (0.8392156862745098,
  0.15294117647058825,
  0.1568627450980392,
  1.0),
 'ParHACClusterer_0.1': (1.0, 0.596078431372549, 0.5882352941176471, 1.0),
 'ParHACClusterer_1': (0.5803921568627451,
  0.403921568627451,
  0.7411764705882353,
  1.0),
 'KCoreClusterer': (0.8392156862745098,
  0.15294117647058825,
  0.1568627450980392,
  1.0)}

style_map = {'LDDClusterer': 'o',
 "KCoreClusterer": 'D',
 'SCANClusterer': 's',
 'LabelPropagationClusterer': '^',
 'SLPAClusterer': 'P',
 'TECTONICClusterer': 'd',
 'ConnectivityClusterer': 'X',
 'ParallelAffinityClusterer': 'v',
 'ParallelCorrelationClusterer': '<',
 'ParallelModularityClusterer': '>',
 'NetworKitPLM': 'o',
 'NetworKitParallelLeiden': 's',
 'Neo4jLouvain': '^',
 'Neo4jLeiden': 'P',
 'Neo4jModularityOptimization': 'd',
 'SnapCNM': 'X',
 'SnapGirvanNewman': 'v',
 'SnapInfomap': 'D',
 'TigerGraphLouvain': 'o',
 'ParHACClusterer_0.01': '*',
 'ParHACClusterer_0.1': '*',
 'ParHACClusterer_1': '*'}


def get_our_methods():
    return [
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

def get_baseline_methods():
    return ["NetworKitPLM", 
            "NetworKitParallelLeiden", 
            "Neo4jLouvain",
            "Neo4jLeiden",
            "Neo4jModularityOptimization",
        'SnapCNM','SnapGirvanNewman', 'SnapInfomap', "TigerGraphLouvain"]


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
