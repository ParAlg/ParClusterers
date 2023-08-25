import pandas as pd
import sys
from sklearn.neighbors import kneighbors_graph
import os
import glob
import cv2

def convert_to_weighted(input, config):
    config = open(config,'r')
    config_info = config.readlines()
    col = -1
    remove = []
    faces = False
    for line in config_info:
        if line.startswith('label'):
            col = int(line.split(' ')[-1].strip())
        elif line.startswith('remove'):
           for elem in line.split(' ')[-1].strip().split(';'):
              remove.append(int(elem))
        elif line.startswith('faces'):
           if line.split(' ')[-1].strip() == 'true':
              faces = True
    data = pd.DataFrame()

    if not faces:
      data = pd.read_csv(input, header = None)
    else:
      data = []
      image_index = 0
      for folder_name in os.listdir(input):
          folder_path = os.path.join(input, folder_name)
          pgm_files = glob.glob(os.path.join(folder_path, '*.pgm'))
          for pgm_file in pgm_files:
              if not (pgm_file.split('.')[-2].endswith('2') or pgm_file.split('.')[-2].endswith('4')):
                img = cv2.imread(pgm_file, cv2.IMREAD_GRAYSCALE)
                img_data = img.flatten()  # Flatten the image data into a 1D array
                img_data = img_data.tolist()
                data.append(img_data + [image_index])
          image_index += 1
      data = pd.DataFrame(data)
      col = -1
      
      
    sub_data = data.drop(data.columns[[col]+remove], axis =1)

    res = kneighbors_graph(sub_data, 10, mode='distance', include_self='auto')
    name = input.split('/')[-1].split('.')[0]
    file = open(name + '.graph.txt','w')
    file.write(
'''# Undirected weighted graph: {input}
# {name}
# Nodes: {nodes} Edges: {edges}
# FromNodeId	ToNodeId    Weight
'''.format(input=input,name=name,nodes=data.shape[0], edges=res.count_nonzero()))

    for i in range(res.shape[0]):
        for j in res[i].nonzero()[1]:
            file.write(str(i) + '\t' + str(j) + '\t'+ str(res[i,j]) + '\n')
    file.close()

    file = open(name + '.cmty.txt','w')
    result = data.reset_index().groupby(data[data.columns[col]])['index'].apply(list).tolist()
    if not (result is None):
      for cluster_list in result:
        file.write("\t".join(str(x) for x in cluster_list) + "\n")
    file.close()



def main():
  args = sys.argv[1:]
  convert_to_weighted(args[0], args[1])

if __name__ == "__main__":
  main()