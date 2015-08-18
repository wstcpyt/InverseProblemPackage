__author__ = 'yutongpang'
import numpy as np
from numpy import linalg as LA
class CGLS:
    def __init__(self, A, b):
        self.A = A
        self.b = b
        self.x = np.zeros(len(b))
        self._doCGLSinteration()

    def _doCGLSinteration(self):
        r = self.b - np.dot(self.A, self.x)
        d = np.dot(self.A.T, r)
        for k in range(0, 1000):
            alpha = LA.norm(np.dot(self.A.T, r))**2/LA.norm(np.dot(self.A, d))**2
            self.x = self.x + alpha*d
            r = r - alpha*np.dot(self.A, d)
            beta = LA.norm(np.dot(self.A.T, r))**2/LA.norm(np.dot(self.A.T, r))**2
            d = np.dot(self.A.T, r) + beta*d
        ro = np.dot(self.A, self.x) - self.b
        print(LA.norm(ro))
