//
// Created by yutong pang on 7/14/15.
//

#include "GeneticAlgorithm.h"
#include <vector>
#include <random>

using std::vector;
using std::sort;
template <class PopulationPropertiesType>
GeneticAlgorithm<PopulationPropertiesType>::GeneticAlgorithm(PopulationPropertiesType population_properties, unsigned int population_number)
{
    populationNumber = population_number;
    gaStruct.populationProperties = population_properties;
    gaStruct.fitness = 0.0;
    population.assign(population_number, gaStruct);
    buffer.assign(population_number, gaStruct);
    populationP = &population;
    populationProperitesSize = population_properties.size();
    bufferP = &buffer;
    randMax = 0.0;
    randomMin = 0.0;
    srand(unsigned(time(NULL)));
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::initPopulation(double rand_min, double rand_max, double rand_base) {
    randBase = rand_base;
    for (GaStruct &popind:*populationP){
        size_t populationStructSize = popind.populationProperties.size();
        for (int i = 0; i < populationStructSize; ++i) {
            popind.populationProperties[i] = doubleRand(rand_min, rand_max);
        }
    }
}

template <class PopulationPropertiesType>
double GeneticAlgorithm<PopulationPropertiesType>::doubleRand(double rand_min, double rand_max) {
    randomMin = rand_min;
    randMax = rand_max;
    double randTemp = (double)(rand() % int(randBase) / randBase);
    return rand_min + randTemp * (rand_max - rand_min);
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::sortByFitness() {
    sort((*populationP).begin(), (*populationP).end(), [](GaStruct x, GaStruct y){ return x.fitness < y.fitness; });
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::mate(double elitrate, double mutationrate){
    int eSize = int(populationNumber * elitrate), i1, i2, crossposition;
    eltism(eSize);
    // Mate the rest
    for (int i = eSize; i < populationNumber; i++){
        i1 = rand()%(populationNumber / 2);
        i2 = rand()%(populationNumber / 2);
        crossposition = rand() % populationProperitesSize;
        for (int j = 0; j < populationProperitesSize; j++){
            if (j< crossposition){
                (*bufferP)[i].populationProperties[j] = (*populationP)[i1].populationProperties[j];
            }
            else{
                (*bufferP)[i].populationProperties[j] = (*populationP)[i2].populationProperties[j];
            }
        }
        if (rand() < int(mutationrate * RAND_MAX)){
            mutate((*bufferP)[i]);
        }
    }
    swap();
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::eltism(int e_size) {
    for (int i = 0; i < e_size; i++){
        (*bufferP)[i].populationProperties = (*populationP)[i].populationProperties;
        (*bufferP)[i].fitness = (*populationP)[i].fitness;
    }
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::mutate(GaStruct &mutatemember) {
    int mutateposition = rand()%populationProperitesSize;
    mutatemember.populationProperties[mutateposition] = doubleRand(randomMin, randMax);
}

template <class PopulationPropertiesType>
void GeneticAlgorithm<PopulationPropertiesType>::swap() {
    GaVector *temp = populationP; populationP = bufferP; bufferP = temp;
}

template class GeneticAlgorithm<vector<double>>;
template class GeneticAlgorithm<vector<int>>;
