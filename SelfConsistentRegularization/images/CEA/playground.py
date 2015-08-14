__author__ = 'yutongpang'
import sys
from numpy import linalg as LA
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')

from pythonCore.CEAs import CEAs
from pythonCore.GeoPhysics import GeoPhysicsBase
from pythonCore.SVD import SVDRegularization
from pythonCore.TikhonovRegularization import TReg
import numpy as np

matrix_size = 40

def singlepopulationfitness(singlepopulation):
    geophysicsbase = GeoPhysicsBase(matrix_size, 0.25)
    geophysicsbase.g[0] = singlepopulation['population_properties'][0] + geophysicsbase.g[0]
    geophysicsbase.g[1] = singlepopulation['population_properties'][1] + geophysicsbase.g[1]
    geophysicsbase.g[2] = singlepopulation['population_properties'][2] + geophysicsbase.g[2]
    svd_regularization = SVDRegularization(geophysicsbase.A, geophysicsbase.g)
    f = svd_regularization.reconstruct_with_tikhonov(1E-11)
    return svd_regularization.getGCValue(geophysicsbase.A, geophysicsbase.g, f)

def singlepopulationfintessleastsquare(singlepopulation):
    geophysicsbase = GeoPhysicsBase(matrix_size, 0.25)
    geophysicsbase.g[0] = singlepopulation['population_properties'][0] + geophysicsbase.g[0]
    geophysicsbase.g[1] = singlepopulation['population_properties'][1] + geophysicsbase.g[1]
    geophysicsbase.g[2] = singlepopulation['population_properties'][2] + geophysicsbase.g[2]
    leastsquare = TReg(geophysicsbase.A, geophysicsbase.g, 1E-11)
    f = leastsquare.getf()
    ro = np.dot(geophysicsbase.A, f)
    ro = LA.norm(ro - geophysicsbase.g)**2
    return ro

def printBestValue(population):
    sortresult = np.sort(population, order=['fitness', 'population_properties'], axis=None)
    print(str(sortresult[0]['fitness']) + " " + str(sortresult[0]['population_properties'][0]) + " " + str(sortresult[0]['population_properties'][1])+ " " + str(sortresult[0]['population_properties'][2]))


if __name__ == '__main__':
    populationnumer_length = 100
    cea = CEAs(3, populationnumer_length)
    cea.init_individual(-1.0, 1.0, 100)
    for i in range(0, 1000):
        # for each node in the population
        for i in range(0, populationnumer_length):
            for j in range(0, populationnumer_length):
                # evaluate individual at node
                cea.population[i][j]['fitness'] = singlepopulationfitness(cea.population[i][j])
                # produce offspring
                offsprings = cea.produceOffSpring(i, j)
                # evaluate offspring
                for offspring in offsprings:
                    offspring['fitness'] = singlepopulationfitness(offspring)
                #assign one of the offspring to node according to a given criterion using binary tournament
                tournament_offspring = np.array([], dtype=cea.populationStruct)
                offspring_A_index = np.random.random_integers(0, len(offsprings)-1)
                tournament_offspring = np.append(tournament_offspring, offsprings[offspring_A_index])
                offspring_B_index = np.random.random_integers(0, len(offsprings)-1)
                tournament_offspring = np.append(tournament_offspring, offsprings[offspring_B_index])
                if tournament_offspring[0]['fitness'] <= tournament_offspring[1]['fitness']:
                    if tournament_offspring[0]['fitness'] < cea.population[i][j]['fitness']:
                        cea.population[i][j] = tournament_offspring[0]
                else:
                    if tournament_offspring[1]['fitness'] < cea.population[i][j]['fitness']:
                        cea.population[i][j] = tournament_offspring[1]
        printBestValue(cea.population)

