__author__ = 'yutongpang'
import numpy as np
from numpy import linalg as LA


class SVDRegularization():
    def __init__(self, AM, g):
        self.AM = AM
        self.g = g
        self.phivector = np.zeros(len(g))
        self._decompse_matrix_with_svd()

    def _decompse_matrix_with_svd(self):
        U, s, V = np.linalg.svd(self.AM, full_matrices=True)
        self.U = U
        self.s = s
        self.V = V

    def get_picatd_parameter(self):
        ut = self.U.T
        utb = np.dot(ut, self.g)
        utbs = utb/self.s
        return self.s, utb, utbs

    def reconstruct_with_tsvd(self, truncate_number):
        f_tsvd = 0
        s, utb, utbs = self.get_picatd_parameter()
        for i in range(0, truncate_number):
            f_tsvd += utbs[i] * self.V[i]
        return f_tsvd

    def reconstruct_with_tikhonov(self, lambda_value):
        f_tikhonov = 0
        s, utb, utbs = self.get_picatd_parameter()
        for i in range(0, len(self.g)):
            phi = self.s[i]**2/(self.s[i]**2+lambda_value**2)
            self.phivector[i] = phi
            f_tikhonov += phi*utbs[i]*self.V[i]
        return f_tikhonov

    def getGCValue(self, A, g, f):
        ro = np.dot(A, f)
        ro = LA.norm(ro - g)**2
        sumphi = 0
        for i in range(0, len(g)):
            sumphi += self.phivector[i]
        divisor = (len(g) - sumphi)**2
        return ro/divisor


