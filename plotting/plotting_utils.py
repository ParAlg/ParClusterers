import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np

fontsize=25

def replace_graph_names(df):
    df["Input Graph"] = df["Input Graph"].str.replace("com-", "", regex=True)\
                                    .str.replace(".gbbs.txt", "", regex=True)\
                                    .str.replace(".bin", "", regex=True)\
                                    .str.replace(".ungraph.txt", "", regex=True)\
                                    .str.replace(".graph.txt", "", regex=True)
    
    df["Input Graph"] = df["Input Graph"].str.replace("amazon", "AM", regex=True)\
                                    .str.replace("youtube", "YT", regex=True)\
                                    .str.replace("dblp", "DB", regex=True)\
                                    .str.replace("lj", "LJ", regex=True)\
                                    .str.replace("orkut", "OK", regex=True)\
                                    .str.replace("friendster", "FS", regex=True)

    df["Clusterer Name"] = df["Clusterer Name"].str.replace("Scan", "SCAN", regex=True)\
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

def plot_data(df, ax):
    sns.lineplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", linewidth = 3, ax = ax)
    sns.scatterplot(data=df, x="Threads", y="Cluster Time", hue="Clusterer Name", s = 30, ax = ax, legend=False)

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
