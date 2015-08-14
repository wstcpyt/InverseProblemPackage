__author__ = 'yutongpang'
import sys
sys.path.append('/Users/yutongpang/PycharmProjects/InverseProblemPackage')

from pythonCore.CEAs import CEAs
from pythonCore.GeoPhysics import GeoPhysicsBase
from pythonCore.svd import SVDRegularization
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

def printBestValue(population):
    sortresult = np.sort(population, order=['fitness', 'population_properties'], axis=None)
    print(str(sortresult[0]['fitness']) + " " + str(sortresult[0]['population_properties'][0]) + " "+ str(sortresult[0]['population_properties'][1])+ " " + str(sortresult[0]['population_properties'][2]))



def singlegenerationprocess(i, j, cea):
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

def testprint(i):
    print("test" + str(i))

def halfplanprocessing1(q1, cea, populationnumer_length):
    for i in range(0, populationnumer_length):
        for j in range(0, int(populationnumer_length/2)):
            singlegenerationprocess(i, j, cea)
    q1.put(cea)

def halfplanprocessing2(q2, cea, populationnumer_length):
    for i in range(0, populationnumer_length):
        for j in range(int(populationnumer_length/2), populationnumer_length):
            singlegenerationprocess(i, j, cea)
    q2.put(cea)

from multiprocessing import Process, Queue
if __name__ == '__main__':
    populationnumer_length = 100
    cea = CEAs(3, populationnumer_length)
    cea.init_individual(-1.0, 1.0, 10)
    for loop in range(0, 1000):
        # for each node in the population
        import copy
        q1 = Queue()
        q2 = Queue()
        process1 = Process(target=halfplanprocessing1, args=(q1, cea, populationnumer_length,))
        process2 = Process(target=halfplanprocessing2, args=(q2, cea, populationnumer_length,))
        process1.start()
        process2.start()
        cea1 = q1.get()
        cea2 = q2.get()
        for i in range(0, populationnumer_length):
            for j in range(0, int(populationnumer_length/2)):
                cea.population[i][j] = cea1.population[i][j]
        for i in range(0, populationnumer_length):
            for j in range(int(populationnumer_length/2), populationnumer_length):
                cea.population[i][j] = cea2.population[i][j]
        printBestValue(cea.population)

