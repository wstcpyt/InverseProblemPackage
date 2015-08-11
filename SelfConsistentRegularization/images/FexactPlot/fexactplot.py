__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')
import matplotlib.pyplot as plt
from pythonCore.GeoPhysics import GeoPhysicsBase
import numpy as np

def plot_geophysics_fexact(discretization_size):
    geo_physics = GeoPhysicsBase(discretization_size, 0.25)
    x = np.arange(discretization_size) / discretization_size
    y = geo_physics.fexact
    plt.plot(x, y, linewidth=2)
    #plt.title("f(t)", fontsize=16)
    plt.xlabel("t (Normalized Unit)", fontsize=14)
    plt.ylabel("the mass density distribution", fontsize=14)
    plt.axis([0, 1, 0.5, 2.2])
    plt.tick_params(axis='both', which='major', labelsize=14)
    plt.show()


if __name__ == '__main__':
    plot_geophysics_fexact(40)
