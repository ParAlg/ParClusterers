import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math


# Graph runtimes
def graph_runtime_overall(arg):
    data = pd.read_csv(arg + '/runtimes.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Threads'], dropna=False)['Cluster Time'].mean()
        sub_data = sub_data.reset_index(level=1)
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(sub_data.loc[index, 'Threads'], pd.Series(sub_data.loc[index, 'Cluster Time']).apply(np.log), label = index, marker = 'o')
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
        plt.ylabel('Cluster Time (Log Seconds)')
        plt.xlabel('Threads')
        plt.title(graph + ' Clustering Time')
        plt.savefig(arg + '/' + graph + '_' + arg + '_runtime_graph.png')


# Graph runtimes for each clusterer
def graph_runtime_individual(arg):
    data = pd.read_csv(arg + '/runtimes.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        
        for clusterer in data['Clusterer Name'].unique():
            clusterer_data = sub_data[sub_data['Clusterer Name'] == clusterer]
            clusterer_data = clusterer_data.groupby(['Config', 'Threads'], dropna=False)['Cluster Time'].mean()
            clusterer_data = clusterer_data.reset_index(1)
            plt.figure(figsize=(12,14))
            
            for index in sorted(clusterer_data.index.drop_duplicates()):
                plt.plot(clusterer_data.loc[index, 'Threads'], pd.Series(clusterer_data.loc[index, 'Cluster Time']).apply(np.log), label = index, marker = 'o')
            ax = plt.subplot(111)
            box = ax.get_position()
            ax.set_position([box.x0, box.y0 + box.height * 0.5,
                        box.width, box.height * 0.6])
            plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
            plt.ylabel('Cluster Time (Log Seconds)')
            plt.xlabel('Threads')
            plt.title(clusterer + ' ' + graph + ' Clustering Time')
            plt.savefig(arg + '/' + graph + '_' + arg + '_' + clusterer + '_runtime_graph.png')
            

# Graph fscore
def graph_fscore_overall(arg):
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Config', 'Threads'], dropna=False)[['Cluster Time', 'fScore_mean', 'fScoreParam']].mean()
        sub_data = sub_data.sort_values('Cluster Time')
        sub_data =  sub_data.reset_index((1,2))
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(pd.Series(sub_data.loc[index, 'Cluster Time']).apply(np.log), sub_data.loc[index, 'fScore_mean'], label = index, marker='o')
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
        plt.ylabel('F Score (param : ' + str(sub_data.iloc[0]['fScoreParam']) + ')')
        plt.xlabel('Cluster Time (Log Seconds)')
        plt.title(graph + ' F-Score')
        plt.savefig(arg + '/' + graph + '_' + arg + '_fscore_graph.png')

# Graph fscore for each clusterer
def graph_fscore_individual(arg):
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        
        sub_data = data[data['Input Graph'] == graph]
        for clusterer in data['Clusterer Name'].unique():
            clusterer_data = sub_data[sub_data['Clusterer Name'] == clusterer]    
            clusterer_data = clusterer_data.groupby(['Config', 'Threads'], dropna=False)[['Cluster Time', 'fScore_mean', 'fScoreParam']].mean()
            clusterer_data = clusterer_data.reset_index(-1)
            plt.figure(figsize=(12,14))

            for index in sorted(clusterer_data.index.drop_duplicates()):
                
                plt.plot(pd.Series(clusterer_data.loc[index, 'Cluster Time']).apply(np.log), clusterer_data.loc[index, 'fScore_mean'], label = index, marker = 'o')

            ax = plt.subplot(111)
            box = ax.get_position()
            ax.set_position([box.x0, box.y0 + box.height * 0.5,
                        box.width, box.height * 0.6])
            plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
            plt.ylabel('F Score (param : ' + str(sub_data.iloc[0]['fScoreParam']) + ')')
            plt.xlabel('Cluster Time (Log Seconds)')
            plt.title(clusterer + ' ' + graph + ' F-Score')
            plt.savefig(arg + '/' + graph + '_' + arg + '_' + clusterer + '_fscore_graph.png')            



# Graph precision recall for each clusterer
def graph_precision_recall_individual(arg):
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        
        for clusterer in data['Clusterer Name'].unique():
            clusterer_data = sub_data[sub_data['Clusterer Name'] == clusterer]
            clusterer_data = clusterer_data.groupby(['Config', 'Threads'], dropna=False)[['communityRecall_mean', 'communityPrecision_mean']].mean()
            clusterer_data = clusterer_data.reset_index(-1)
            clusterer_data = clusterer_data.sort_values('communityRecall_mean')
            plt.figure(figsize=(12,14))

            for index in sorted(clusterer_data.index.drop_duplicates()):
                plt.plot(clusterer_data.loc[index, 'communityRecall_mean'], clusterer_data.loc[index, 'communityPrecision_mean'], label = index, marker = 'o')
            
            ax = plt.subplot(111)
            box = ax.get_position()
            ax.set_position([box.x0, box.y0 + box.height * 0.5,
                        box.width, box.height * 0.6])
            plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
            plt.ylabel('Average Precision')
            plt.xlabel('Average Recall')
            plt.xlim(0,1)
            plt.ylim(0,1)
            plt.title(clusterer + ' ' + graph + ' Precision-Recall')
            plt.savefig(arg + '/' + graph + '_' +  arg + '_' + clusterer +'_precisionrecall_graph.png')
            


# Graph precision recall
def graph_precision_recall_overall(arg):
    data = pd.read_csv(arg + '/stats.csv')
    data = data.fillna('None')
    for graph in data['Input Graph'].unique():
        sub_data = data[data['Input Graph'] == graph]
        sub_data = sub_data.groupby(['Clusterer Name', 'Config', 'Threads'], dropna=False)[['communityRecall_mean', 'communityPrecision_mean']].mean()
        sub_data = sub_data.reset_index((1,2))
        sub_data = sub_data.sort_values('communityRecall_mean')
        plt.figure(figsize=(12,14))

        for index in sorted(sub_data.index.drop_duplicates()):
            plt.plot(sub_data.loc[index, 'communityRecall_mean'], sub_data.loc[index, 'communityPrecision_mean'], label = index, marker = 'o')
        ax = plt.subplot(111)
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.5,
                    box.width, box.height * 0.6])
        plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), title = 'Config')
        plt.ylabel('Average Precision')
        plt.xlabel('Average Recall')
        plt.xlim(0,1)
        plt.ylim(0,1)
        plt.title(graph + ' Precision-Recall')
        plt.savefig(arg + '/' + graph + '_' + arg +'_precisionrecall_graph_overall.png')

def graph_file(arg, config):
    config = open(config,'r')
    config_info = config.readlines()
    for line in config_info:
        if line.startswith('graph_precision_recall_overall:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_precision_recall_overall(arg)
        elif line.startswith('graph_precision_recall_individual:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_precision_recall_individual(arg)
        elif line.startswith('graph_fscore_overall:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_fscore_overall(arg)
        elif line.startswith('graph_fscore_individual:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_fscore_individual(arg)
        elif line.startswith('graph_runtime_overall:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_runtime_overall(arg)
        elif line.startswith('graph_runtime_individual:'):
            if line.split(' ')[-1].strip() == 'true':
                graph_runtime_individual(arg)
  

def main():
  args = sys.argv[1:]
  graph_file(args[0], args[1])

if __name__ == "__main__":
  main()