import pandas as pd
import hypertools as hyp
import numpy as np
import cfg

data_filename = '../data_used/0{}.npz'

data_set = []
num_individual = 3
for i in range(1, 1 + num_individual):
    zip_data = np.load(data_filename.format(i))
    files_in_data = zip_data.keys()
    individual = []
    for j in range(len(files_in_data)):    
        fn = files_in_data[j]
        tmp = np.asarray(zip_data[fn])
        tmp = tmp.transpose(1,0,2)
        tmp = tmp.reshape(tmp.shape[0],-1)
        individual.append(tmp)
    individual = np.vstack(individual)
    label = i *np.ones((individual.shape[0], 1))
    individual = np.hstack((individual, label))
    data_set.append(individual)

data_set = np.vstack(data_set)
data = pd.DataFrame(data_set)
individual_labels = data.pop(310)

geo_tsne = hyp.plot(data, '.', group=individual_labels, legend=list(set(individual_labels)), reduce='TSNE', ndims=3)