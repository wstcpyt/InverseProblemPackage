__author__ = 'yutongpang'
import numpy as np
import matplotlib.pyplot as plt


class GeoPhysicsBase:
    def __init__(self, discretization_size, d):
        self.discretization_size = discretization_size
        self.fexact = np.zeros(discretization_size)
        self.A = np.zeros((discretization_size, discretization_size))
        self.d = d;
        self._init_fexact()
        self._init_A()
        self._calculate_g()

    def _init_fexact(self):
        for i in range(self.discretization_size):
            if i < self.discretization_size * 0.4:
                self.fexact[i] = 2
            else:
                self.fexact[i] = 1

    def _init_A(self):
        for i in range(self.discretization_size):
            for j in range(self.discretization_size):
                d = self.d
                s = 1.0/self.discretization_size * (j + 0.5)
                t = 1.0/self.discretization_size * (i + 0.5)
                self.A[i][j] = 1.0/self.discretization_size * d / (d**2 + (s-t)**2)**1.5

    def _calculate_g(self):
        self.g = np.dot(self.A, self.fexact)
