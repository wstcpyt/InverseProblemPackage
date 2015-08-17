__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')
from pythonCore.Interation import LandweberCimmino
from pythonCore.GeoPhysics import GeoPhysicsBase
import matplotlib.pyplot as plt
import numpy as np

def plot_tsvd_plot(matrix_size):
    geophysics = GeoPhysicsBase(matrix_size, 0.25)
    interation = LandweberCimmino(geophysics.A, geophysics.g)
    x = np.arange(matrix_size)
    plt.plot(x, geophysics.fexact, 'r', linewidth=2, label='pre defined f')
    plt.plot(x, interation.x, 'bs', linewidth=2, label='reconstructed f')
    plt.tick_params(axis='both', which='major', labelsize=18)
    plt.legend(loc='upper right', shadow=True, fontsize='18')
    plt.show()

if __name__ == '__main__':
    plot_tsvd_plot(60)