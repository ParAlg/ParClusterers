import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np
from scipy.stats import gmean
from plotting_utils import *

plt.rcParams["ps.useafm"] = True
plt.rcParams["pdf.use14corefonts"] = True
plt.rcParams["text.usetex"] = True
fontsize = 30
our_name = "PCBS"

import matplotlib.colors as mcolors
default_blue = mcolors.to_hex(mcolors.TABLEAU_COLORS['tab:blue'])
default_orange = mcolors.to_hex(mcolors.TABLEAU_COLORS['tab:orange'])

def plot_data(df, ax, color_dict=None):
    if color_dict is not None:
        sns.lineplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", palette=color_dict, linewidth=3, ax=ax)
        sns.scatterplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", palette=color_dict, s=30, ax=ax, legend=False)
    else:
        sns.lineplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", linewidth=3, ax=ax)
        sns.scatterplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", s=30, ax=ax, legend=False)
    ax.legend(loc='upper left', ncol=1, bbox_to_anchor=(1, 0.8), frameon=False, fontsize=fontsize-5)



def compute_speedups(df):
    df_4 = df[df["Threads"] == 4]
    df_60 = df[df["Threads"] == 60]

    ### NetworKit
    # compute average speedup over TigerGraph

    speedups = []

    # WCC
    ours_time = float(df_60[df_60["Clusterer Name"]=="ConnectivityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="NetworKitConnectivity"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # KCore
    ours_time = float(df_60[df_60["Clusterer Name"]=="KCoreClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="NetworKitKCore"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # Modularity
    # For Neo4j, we use the fastest time
    # for ours, we use modularity 
    ours_time = float(df_60[df_60["Clusterer Name"]=="ParallelModularityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time =float(df_60[df_60["Clusterer Name"]=="NetworKitParallelLeiden"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # LP
    ours_time = float(df_60[df_60["Clusterer Name"]=="LabelPropagationClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="NetworKitPLP"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    print("Geomean speedup over NetworKit", gmean(speedups))

    ### TigerGraph
    # compute average speedup over TigerGraph

    speedups = []

    # WCC
    ours_time = float(df_60[df_60["Clusterer Name"]=="ConnectivityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="TigerGraphWCC"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # KCore
    ours_time = float(df_60[df_60["Clusterer Name"]=="KCoreClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="TigerGraphKCore"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # Modularity
    # For Neo4j, we use the fastest time
    # for ours, we use modularity 
    ours_time = float(df_60[df_60["Clusterer Name"]=="ParallelModularityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time =float(df_60[df_60["Clusterer Name"]=="TigerGraphLouvain"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # LP
    ours_time = float(df_60[df_60["Clusterer Name"]=="LabelPropagationClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_60[df_60["Clusterer Name"]=="TigerGraphLabelProp"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    print("Geomean speedup over TigerGraph", gmean(speedups))


    ### Neo4j
    # compute average speedup over Neo4j 

    speedups = []

    # WCC
    ours_time = float(df_4[df_4["Clusterer Name"]=="ConnectivityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_4[df_4["Clusterer Name"]=="Neo4jConnectivity"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # KCore
    ours_time = float(df_4[df_4["Clusterer Name"]=="KCoreClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_4[df_4["Clusterer Name"]=="Neo4jKCore"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # Modularity
    # For Neo4j, we use the fastest time
    # for ours, we use modularity 
    ours_time = float(df_4[df_4["Clusterer Name"]=="ParallelModularityClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_4[df_4["Clusterer Name"]=="Neo4jLeiden"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    # LP
    ours_time = float(df_4[df_4["Clusterer Name"]=="LabelPropagationClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_4[df_4["Clusterer Name"]=="Neo4jLabelPropagation"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    #SLPA
    ours_time = float(df_4[df_4["Clusterer Name"]=="SLPAClusterer"]["Cluster Time"].iloc[0])
    neo4j_time = float(df_4[df_4["Clusterer Name"]=="Neo4jSLPA"]["Cluster Time"].iloc[0])
    speedups.append(neo4j_time / ours_time)

    print("Geomean speedup over Neo4j", gmean(speedups))


if __name__ == "__main__":
    base_addr = "results/"
    fig, axs = plt.subplots(1, 3, constrained_layout=False, figsize =(25,4))#, gridspec_kw=gs_kw)

    directories = ["nk", "pcbs", "tectonic", "neo4j", "tg"]
    df = pd.concat([pd.read_csv(base_addr + f"out_scalability_{f}_csv/runtimes.csv") for f in directories])
    replace_graph_names(df)
    df = df[df["Round"] != 0] # remove first round
    df = df.dropna(how='all')
    df['Config'].fillna('none', inplace=True)
    fontsize=25
    # Average over rounds
    df = df.groupby(['Clusterer Name', 'Input Graph', 'Threads', 'Config'])['Cluster Time'].mean().reset_index()
    compute_speedups(df)

    ##  "Connectivity"
    ax = axs[0]
    df_wcc = df[(df["Clusterer Name"].str.contains("Connectivity")) | (df["Clusterer Name"].str.contains("WCC"))]
    df_wcc.loc[:, "Clusterer Name"] = df_wcc["Clusterer Name"].str.replace("ConnectivityClusterer", our_name)
    df_wcc.loc[:, "Clusterer Name"] = df_wcc["Clusterer Name"].str.replace("Connectivity", "")
    df_wcc.loc[:, "Clusterer Name"] = df_wcc["Clusterer Name"].str.replace("WCC", "")
    plot_data(df_wcc, ax)
    common_ax_style(ax)
    ax.set_title("Connectivity", fontsize=fontsize)
    ax.set_ylabel("Time (s)", fontsize = fontsize)

    ax = axs[1]
    ## KCore
    df_kcore = df[df["Clusterer Name"].str.contains("KCore")]
    df_kcore.loc[:, "Clusterer Name"] = df_kcore["Clusterer Name"].str.replace("KCoreClusterer", our_name)
    df_kcore.loc[:, "Clusterer Name"] = df_kcore["Clusterer Name"].str.replace("KCore", "")
    plot_data(df_kcore, ax)
    common_ax_style(ax)
    ax.set_title("Kcore", fontsize=fontsize)
    ax.set_ylabel("")

    ## TECTONIC
    ax = axs[2]
    color_dict = {
        "TECTONIC-orig": default_orange,
        our_name:default_blue
    }
    tectonic_data = df[df["Clusterer Name"].str.contains("TECTONIC")]
    tectonic_data.loc[:, "Clusterer Name"] = tectonic_data["Clusterer Name"].str.replace("TECTONICClusterer", our_name)
    tectonic_data.loc[:, "Clusterer Name"] = tectonic_data["Clusterer Name"].str.replace("TECTONIC", "TECTONIC-orig")
    plot_data(tectonic_data, ax, color_dict)
    common_ax_style(ax)
    ax.set_title("Tectonic", fontsize=fontsize)
    ax.set_ylabel("")

    plt.tight_layout()
    plt.savefig(base_addr + "runtime.pdf")

    plt.clf()

    fig, axs = plt.subplots(1, 3, constrained_layout=False, figsize =(25,4))#, gridspec_kw=gs_kw)
    ax = axs[0]
    df_lp = df[(df["Clusterer Name"].str.contains("Label")) | (df["Clusterer Name"].str.contains("PLP"))]
    df_lp = df_lp[~df_lp["Clusterer Name"].str.contains("SLLabelProp")]  # remove tigergraph SLPA
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelPropagationClusterer", our_name)
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelPropagation", "")
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LabelProp", "")
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("PLP", "")
    df_lp.loc[:, "Clusterer Name"] = df_lp["Clusterer Name"].str.replace("LP", "")

    plot_data(df_lp, ax)
    common_ax_style(ax)
    ax.set_title("Label Propagation", fontsize=fontsize)
    ax.set_ylabel("Time (s)", fontsize = fontsize)

    ax = axs[1]
    df_slpa = df[(df["Clusterer Name"].str.contains("SLPA")) | (df["Clusterer Name"].str.contains("SLLabel"))]
    color_dict = {
        "Neo4j": default_orange,
        our_name :default_blue
    }
    df_slpa.loc[:, "Clusterer Name"] = df_slpa["Clusterer Name"].str.replace("SLPAClusterer", our_name)
    df_slpa.loc[:, "Clusterer Name"] = df_slpa["Clusterer Name"].str.replace("SLPA", "")
    plot_data(df_slpa, ax, color_dict)
    common_ax_style(ax)
    ax.set_title("Speaker-Listerner Label Propagation", fontsize=fontsize)
    ax.set_ylabel("")

    ax = axs[2]
    df_other_ours = df[df["Clusterer Name"].isin(["LDDClusterer", "ParHACClusterer", 
                                                  "ParallelAffinityClusterer", "SCANClusterer"])]
    plot_data(df_other_ours , ax)
    common_ax_style(ax)
    ax.set_title("Other %s Clusterers" % our_name, fontsize=fontsize)
    ax.set_ylabel("")

    plt.tight_layout()
    plt.savefig(base_addr + "runtime_lp.pdf")


    plt.clf()

    df = df[df["Clusterer Name"].isin(['Neo4jLeiden', 'Neo4jLouvain', 'Neo4jModularityOptimization', 
                                              'NetworKitPLM', 'NetworKitParallelLeiden', 'TigerGraphLouvain', 
                                              "ParallelModularityClusterer"])]

    fig, ax = plt.subplots(1, 1, constrained_layout=False, figsize =(9,5))#, gridspec_kw=gs_kw)

    colors = sns.color_palette("Set2")
    algorithm_names = [
        our_name,
        'Neo4jLeiden',
        'Neo4jLouvain',
        'Neo4jModularity',
        'NetworKitPLM',
        'NetworKitLeiden',
        'TigerGraphLouvain',
    ]

    # Initialize an empty color dictionary
    color_dict = {}

    # Iterate over algorithm names and assign colors from the "Set2" palette
    for i, name in enumerate(algorithm_names):
        color_dict[name] = colors[i]

    df["Clusterer Name"] = df["Clusterer Name"].str.replace("ParallelModularityClusterer", our_name)
    df["Clusterer Name"] = df["Clusterer Name"].str.replace("ModularityOptimization", "Modularity")
    df["Clusterer Name"] = df["Clusterer Name"].str.replace("Parallel", "")

    sns.lineplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", linewidth=5, palette=color_dict, ax=ax)
    sns.scatterplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", s=30, palette=color_dict, ax=ax, legend=False)
    common_ax_style(ax)
    ax.set_ylabel("Time (s)",  fontsize=fontsize)
    plt.legend(loc='center left', ncol=1, bbox_to_anchor=(1, 0.5),
                  frameon=False, fontsize=fontsize-5,  handletextpad=0.3,
                  columnspacing=0.6)
    plt.tight_layout()
    plt.savefig(base_addr + "runtime_md.pdf")




