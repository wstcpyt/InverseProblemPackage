__author__ = 'yutongpang'
import numpy as np
import matplotlib.pyplot as plt
map = np.loadtxt("generation3.txt")
map = np.log10(map)
print(map)
plt.imshow(map, vmin=-12, vmax=-4)
plt.title("generation3", fontsize=18)
plt.tick_params(axis='both', which='major', labelsize=16)
plt.colorbar()
plt.show()
