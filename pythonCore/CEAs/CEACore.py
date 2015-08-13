__author__ = 'yutongpang'
import numpy as np


class CEAs:
    rand_min = 0
    rand_max = 0
    rand_base = 0

    def __init__(self, population_paroperties_size, population_nubmer_length):
        self.populationStruct = [('fitness', 'float64'), ('population_properties', 'float64', (population_paroperties_size,))]
        self.population = np.zeros((population_nubmer_length, population_nubmer_length),
                                   dtype=self.populationStruct
                                   )
        self.population_numer_length = population_nubmer_length
        self.population_paroperties_size = population_paroperties_size

    def init_individual(self, rand_min, rand_max, rand_base):
        self.rand_min = rand_min
        self.rand_max = rand_max
        self.rand_base = rand_base
        for populationRow in self.population:
            for individual in populationRow:
                for i in range(0, self.population_paroperties_size):
                    individual['population_properties'][i] = self._getDoubleRand()

    def _getDoubleRand(self):
        randTemp = np.random.random_integers(0, 100000) % self.rand_base / float(self.rand_base)
        return self.rand_min + randTemp * (self.rand_max - self.rand_min)

    def produceOffSpring(self, x, y):
        import copy
        # select individuals in the neigberhood
        parentA = self.population[x][y]
        off_springs = np.array([], self.populationStruct)
        # up offspring
        if y - 1 >= 0:
            parentB = copy.deepcopy(self.population[x][y - 1])
            self._mate(parentA, parentB, 0.1)
            off_springs = np.append(off_springs, parentB)
        # down offspring
        if y + 1 < self.population_numer_length:
            parentB = copy.deepcopy(self.population[x][y+1])
            self._mate(parentA, parentB, 0.1)
            off_springs = np.append(off_springs, parentB)
        # left offspring
        if x - 1 >= 0:
            parentB = copy.deepcopy(self.population[x-1][y])
            self._mate(parentA, parentB, 0.1)
            off_springs = np.append(off_springs, parentB)
        # right offspring
        if x + 1 < self.population_numer_length:
            parentB = copy.deepcopy(self.population[x+1][y])
            self._mate(parentA, parentB, 0.1)
            off_springs = np.append(off_springs, parentB)
        return off_springs


    def _mate(self, parentA, parentB, mutation_rate):
        # crossover
        crosspositon = np.random.random_integers(0, self.population_paroperties_size-1)
        for i in range(0, crosspositon):
            parentB['population_properties'][i] = parentA['population_properties'][i]
        # mutate
        if np.random.random_integers(0, 1000) / 1000.0 < mutation_rate:
            mutateposition = np.random.random_integers(0, self.population_paroperties_size-1)
            parentB['population_properties'][mutateposition] = self._getDoubleRand()
