import numpy as np
import matplotlib.pyplot as plt

def compute_distance(x):
    num = len(x)
    dis = np.zeros((num, num))
    for i in range(num):
        for j in range(num):
            dis[i][j] = np.sqrt((x[i][0] - x[j][0])*(x[i][0] - x[j][0]) +\
                                (x[i][1] - x[j][1])*(x[i][1] - x[j][1]))
    return dis

def compute_center(centers):
    new_centers = []
    for i in range(len(centers)):
        ans = [0,0]
        for j in range(len(centers[i])):
            ans[0] += centers[i][j][0]
            ans[1] += centers[i][j][1]
        ans[0] /= len(centers[i])
        ans[1] /= len(centers[i])
        new_centers.append(ans)
    return new_centers

def kmeans(x, old_centers, dis):
    num_x = len(x)
    num_centers = len(old_centers)
    x2center = np.zeros((num_x, num_centers))
    for i in range(num_x):
        for j in range(num_centers):
            x2center[i][j] = np.sqrt((x[i][0] - old_centers[j][0])*(x[i][0] - old_centers[j][0]) +\
                                     (x[i][1] - old_centers[j][1])*(x[i][1] - old_centers[j][1]))
    xcenter = np.argmin(x2center, 1)
    new_centers = [[]for i in range(num_centers)]
    for i in range(num_x):
        new_centers[xcenter[i]].append(x[i])
    centers = compute_center(new_centers)
    
    tmp_x = np.asarray(x)
    tmp_centers = np.asarray(centers)
    
    plt.xlim(xmin = 0, xmax = 10)
    plt.ylim(ymin = 0, ymax = 11)
    plt.scatter(tmp_x[:,0],tmp_x[:,1], c=xcenter[:])
    plt.scatter(tmp_centers[:,0], tmp_centers[:,1], marker='*')
    plt.savefig('foo.png')

    return centers

if __name__ == "__main__":
    x = [[2,10], [2,5], [8,4], [5,8], [7,5], [6,4], [1,2], [4,9]]
    centers = [[2,10], [5,8], [1,2]]
    dis = compute_distance(x)
    centers = kmeans(x, centers, dis)
    print(centers)