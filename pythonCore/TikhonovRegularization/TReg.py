__author__ = 'yutongpang'
import numpy as np
from numpy.linalg import inv

class TReg:
    def __init__(self, A, g, lambdavalue):
        self.A = A
        self.g = g
        self.lambdavalue = lambdavalue

    def getf(self):
        A = self.A
        g = self.g
        lambdavalue = self.lambdavalue
        identity = np.identity(len(g))
        firstterm = inv(np.dot(A.T, A) + lambdavalue**2*identity)
        secondterm = np.dot(A.T, g)
        xarray = np.dot(firstterm, secondterm)
        return xarray