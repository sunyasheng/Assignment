#!/usr/bin/env python3
import numpy as np 
# import matplotlib.pyplot as plt
# data = np.random.normal(size = (5,100000))*100
# fig,ax = plt.subplots()
# ax.hist(data[0], 40, normed = 1, histtype = 'bar', facecolor = 'yellowgreen', alpha = 0.5)
# ax.set_title('pdf')
# plt.show()
temperature_file = '/home/hadoop/homework/Parallel-Computing/project4/data/Temperature.txt'
city_number = 5
time_step = 10000000
fo = open(temperature_file, "w")
 
for city in range(city_number):
    for _ in range(time_step):
        fo.write("%d\t %.5f\n"%(city, np.random.normal(1)*100))

fo.close()
