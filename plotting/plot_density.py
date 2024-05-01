import math
import pandas as pd
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import re
import numpy as np
import plotting_utils

plt.rcParams['ps.useafm'] = True
plt.rcParams['pdf.use14corefonts'] = True
plt.rcParams['text.usetex'] = True
# sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
fontsize=25

color_map = plotting_utils.color_map
style_map = plotting_utils.style_map

def plot_edge_density(df, x_axis, y_axis, y_label=""):
    clusterers = df["Clusterer Name"].unique()
  
    plt.rcParams.update({'font.size': 16})

    fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(10, 5))

    graph_idx = 0

    lines = []  # To store the Line2D objects for the legend
    labels = []  # To store the corresponding labels for the Line2D objects
    graphs = df["Input Graph"].unique()
    for i in range(1):
        for j in range(2):
            if graph_idx < len(graphs):  # Ensure we have a graph to process
                graph = graphs[graph_idx]
#                 ax = axes[i][j]
                ax = axes[j]
                ax.set_xscale('log')
                ax.set_yscale('log')

                for clusterer in clusterers:
                    # Extract the pareto_df for the current graph and clusterer combination
                    pareto_df = df[(df["Input Graph"]==graph) & (df["Clusterer Name"]==clusterer)]
                    pareto_df = pareto_df.sort_values(by="numberClusters")
                    if pareto_df.empty:
#                         print(graph, clusterer)
                        continue

                    # Plot the pareto_df with the appropriate marker
                    line, = ax.plot(pareto_df[x_axis], pareto_df[y_axis], label=clusterer, 
                                    color=color_map[clusterer], marker=style_map[clusterer], markersize=10, 
                                    linewidth=2)

                    shortened_clusterer = clusterer.replace("Clusterer", "")
                    # If the clusterer's line hasn't been added to lines, add it
                    if shortened_clusterer not in labels:
                        lines.append(line)
                        labels.append(shortened_clusterer)
                
                n = graph.split("_")[1]
                m = graph.split("_")[2]
                
                ax.set_title(f"n={n}, m={m}")
                ax.set_xlabel("Number of Clusters")
                ax.set_yticks([0.1, 0.8, 1])
                if j == 0:
                    ax.set_ylabel(y_label)
                else:
                    ax.set_ylabel("")

                graph_idx += 1
            else:
                axes[i][j].axis('off')  # Turn off axes without data

    fig.subplots_adjust(hspace=0.5)

    # Create a single legend for the entire figure, at the top
    fig.legend(lines, labels, loc='upper center', ncol=4, bbox_to_anchor=(0.5, 1.2))
    return axes


base_addr = "results/"
df = pd.read_csv(base_addr + "rmat_results/stats_rmat.csv")
plotting_utils.replace_graph_names(df)
df = plotting_utils.add_epsilon_to_hac(df)
clusterers = df["Clusterer Name"].unique()

x_axis = "numberClusters"
y_axis = "weightedEdgeDensityMean"
plot_edge_density(df, x_axis, y_axis, "Weighted Edge Density Mean")
plt.savefig("rmat.pdf", bbox_inches='tight')