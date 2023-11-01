import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

base_addr = "/home/sy/ParClusterers/results"
df = pd.read_csv(base_addr + "/out_uci_tmp_csv/stats.csv")

# print(df[["Config","Cluster Time" ,"numberClusters", "fScore_mean", "communityPrecision_mean", "communityRecall_mean"]])

g = sns.FacetGrid(df, col="Input Graph", col_wrap=3, height = 5, xlim=(0, 1.1), ylim=(0,1.1)) # col_wrap decides the number of plots per row
g.map_dataframe(sns.scatterplot, x="communityPrecision_mean", y="communityRecall_mean", hue="Clusterer Name",
                s=300, alpha = 0.8)

plt.savefig("tmp.png", bbox_inches='tight')

#[df["Input Graph"] == "wdbc.graph.txt"]
print(df[["Input Graph", "Config","Cluster Time" ,"numberClusters", "fScore_mean", "communityPrecision_mean", "communityRecall_mean"]])
