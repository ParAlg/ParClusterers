import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
plt.rcParams['ps.useafm'] = True
plt.rcParams['pdf.use14corefonts'] = True
# plt.rcParams['text.usetex'] = True
# sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra


# Load the CSV into a DataFrame
df = pd.read_csv("./results/out_tectonic_csv/runtimes.csv", index_col=0)
df = df[df["Round"] != 0]
df['Config'] = df['Config'].str.replace(",match_real_tectonic: false", "", regex=False)
print(df.head())

# Average over rounds
df_avg = df.groupby(['Clusterer Name', 'Input Graph', 'Threads', 'Config'])['Cluster Time'].mean().reset_index()
print(df_avg.head())

# Create the plot
plt.figure(figsize=(10, 6))  # Set the figure size

sns.lineplot(data=df_avg, x="Threads", y="Cluster Time", hue="Config", style="Input Graph")


plt.yscale('log')

plt.title("TectonicClusterer")
plt.savefig("./results/out_tectonic_times.pdf")
