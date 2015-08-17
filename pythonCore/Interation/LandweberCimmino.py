import numpy as np
from numpy import linalg as LA
class LandweberCimmino:
    def __init__(self, A, b):
        self.b = b
        self.A = A
        self.x = np.zeros(len(b))
        self._dointeration()

    def _dointeration(self):
        A = self.A
        b = self.b
        ohm = 1/LA.norm(np.dot(A.T, A))
        r = b - np.dot(A, self.x)
        rnorm = LA.norm(r)
        turningpoint = 0
        interationnumber = 0
        while turningpoint == 0:
            r = b - np.dot(A, self.x)
            self.x = self.x + ohm*np.dot(A.T, r)
            newnorm = LA.norm(r)
            print(newnorm)
            if newnorm < 1E-1:
                turningpoint = 1
            rnorm = LA.norm(r)
            interationnumber = interationnumber + 1
        print(interationnumber)