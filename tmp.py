import pandas as pd

df = pd.read_csv("results/out_snap_tmp_csv/stats.csv")
df['num_iterations'] = df['Config'].str.extract(r'num_iterations: (\d+)')

print(df[["Input Graph", 'num_iterations','communityRecall_mean', 
                                           'communityPrecision_mean', "fScore_mean",'numberClusters',
                                          'Ground Truth']])