__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')
from pythonCore.svd import SVDRegularization
from pythonCore.GeoPhysics import GeoPhysicsBase
import matplotlib.pyplot as plt
import numpy as np

def plot_tsvd_plot(matrix_size):
    geophysics = GeoPhysicsBase(matrix_size, 0.25)
    noise = np.random.normal(0,0.0001, matrix_size)
    for i in range(3, matrix_size):
        noise[i] = 0
    g = geophysics.g + noise
    svdregularization = SVDRegularization(geophysics.A, g)
    s, utb, utbs = svdregularization.get_picatd_parameter()
    x = np.arange(matrix_size)
    plt.plot(x, geophysics.fexact, 'r', linewidth=2, label='pre defined f')
    plt.plot(x, svdregularization.reconstruct_with_tsvd(16), 'bs', linewidth=2, label='reconstructed f')
    plt.tick_params(axis='both', which='major', labelsize=18)
    plt.legend(loc='upper right', shadow=True, fontsize='18')
    plt.show()

if __name__ == '__main__':
    plot_tsvd_plot(60)