__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')
from pythonCore.svd import SVDRegularization
from pythonCore.GeoPhysics import GeoPhysicsBase
import matplotlib.pyplot as plt
import numpy as np

def plot_picard_plot(matrix_size):
    geophysics = GeoPhysicsBase(matrix_size, 0.25)
    noise = np.random.normal(0,0.0001, matrix_size)
    g = geophysics.g + noise
    svdregularization = SVDRegularization(geophysics.A, g)
    s, utb, utbs = svdregularization.get_picatd_parameter()
    x = np.arange(matrix_size)
    plt.plot(x, s, 'r*', linewidth=2, label='${\sigma _i}$')
    plt.plot(x, abs(utb), 'bs', linewidth=2, label='$|u_i^Tb|$')
    plt.plot(x, abs(utbs), 'g^', linewidth=2, label='$|u_i^Tb|/{\sigma _i}$')
    plt.tick_params(axis='both', which='major', labelsize=18)
    plt.legend(loc='lower left', shadow=True, fontsize='18')
    plt.yscale("log")
    plt.show()

if __name__ == '__main__':
    plot_picard_plot(60)