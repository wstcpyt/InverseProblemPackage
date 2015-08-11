__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')
import matplotlib.pyplot as plt
from pythonCore.GeoPhysics import GeoPhysicsBase
import numpy as np

def plot_geophysics_fexact(discretization_size):
    geo_physics1 = GeoPhysicsBase(discretization_size, 0.25)
    geo_physics2 = GeoPhysicsBase(discretization_size, 0.5)
    geo_physics3 = GeoPhysicsBase(discretization_size, 1)
    x = np.arange(discretization_size) / discretization_size
    y1 = geo_physics1.g
    y2 = geo_physics2.g
    y3 = geo_physics3.g
    plt.plot(x, y1, 'r', linewidth=2, label='d = 0.25')
    plt.plot(x, y2, 'bs', linewidth=2, label='d = 0.5')
    plt.plot(x, y3, 'g^', linewidth=2, label='d = 1.0')
    #plt.title("g(s)", fontsize=16)
    plt.xlabel("s (Normalized Unit)", fontsize=14)
    plt.ylabel("the gravity field", fontsize=14)
    #plt.axis([0, 1, 0.5, 2.2])
    plt.tick_params(axis='both', which='major', labelsize=14)
    plt.legend(loc='upper right', shadow=True, fontsize='14')
    plt.show()


if __name__ == '__main__':
    plot_geophysics_fexact(40)
