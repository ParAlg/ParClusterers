import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


# Graph runtimes
def graph_runtime(arg):
    data = pd.read_csv(arg + '/runtimes.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Config', 'Threads'], dropna=False)['Cluster Time'].mean()
        sub_data = sub_data.reset_index(level=2)
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(sub_data.loc[index, 'Threads'], pd.Series(sub_data.loc[index, 'Cluster Time']).apply(np.log), label = index, marker = 'o')
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15))
        plt.ylabel('Cluster Time (Log Seconds)')
        plt.xlabel('Threads')
        plt.title(graph + ' Clustering Time')
        plt.savefig(arg + '/' + graph + '_runtime_graph.png')

def graph_fscore(arg):
   # Graph fscore
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Config', 'Threads'], dropna=False)[['Cluster Time', 'fScore_mean', 'fScoreParam']].mean()
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(pd.Series(sub_data.loc[index, 'Cluster Time']).apply(np.log), sub_data.loc[index, 'fScore_mean'], label = index, marker = 'o')
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15))
        plt.ylabel('F Score (param : ' + str(sub_data.iloc[0]['fScoreParam']) + ')')
        plt.xlabel('Cluster Time (Log Seconds)')
        plt.title(graph + ' F-Score')
        plt.savefig(arg + '/' + graph + '_fscore_graph.png')


def graph_precision_recall(arg):
    # Graph precision recall
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Config', 'Threads'], dropna=False)[['communityRecall_mean', 'communityPrecision_mean']].mean()
        sub_data = sub_data.reset_index(level=1)
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(sub_data.loc[index, 'communityRecall_mean'], sub_data.loc[index, 'communityPrecision_mean'], label = index, marker = 'o', linestyle='None')
            # sub_data.loc[index].apply(lambda row: plt.text(row['communityRecall_mean'], row['communityPrecision_mean'] * (1 + 0.01) , row['Config'], fontsize=12), axis =1)
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15))
        plt.ylabel('Average Precision')
        plt.xlabel('Average Recall')
        plt.xlim(0,1)
        plt.ylim(0,1)
        plt.title(graph + ' Precision-Recall')
        plt.savefig(arg + '/' + graph + '_precisionrecall_graph.png')

def graph_file(arg, config):
    config = open(config,'r')
    config_info = config.readlines()
    for line in config_info:
        if line.startswith('graph_precision_recall:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_precision_recall(arg)
        elif line.startswith('graph_fscore:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_fscore(arg)
        elif line.startswith('graph_runtime:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_runtime(arg)
  

  

  






def main():
  args = sys.argv[1:]
  graph_file(args[0], args[1])

if __name__ == "__main__":
  main()