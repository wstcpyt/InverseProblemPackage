//
// Created by yutong pang on 8/4/15.
//

#include "CEA.h"
#include <random>

CEA::CEA(vector<double> population_properties, unsigned int population_number_length) {
    populationNumberLength = population_number_length;
    populationStruct.populationProperties = population_properties;
    vector<PopulationStruct> populationRow(population_number_length, populationStruct);
    population.assign(population_number_length, populationRow);
    buuffer.assign(population_number_length, populationRow);
    srand(unsigned(time(NULL)));
}


void CEA::initIndividual(double rand_min, double rand_max, double rand_base) {
    randBase = rand_base;
    for (auto& populationRow:population){
        for (auto& individual:populationRow){
            size_t populatioPropertiesLen = individual.populationProperties.size();
            populationPropertiesSize = populatioPropertiesLen;
            for (size_t i = 0; i < populatioPropertiesLen; i++){
                double randomnumber = getDoubleRand(rand_min, rand_max);
                individual.populationProperties[i] = randomnumber;
            }
        }
    }
}

double CEA::getDoubleRand(double rand_min, double rand_max) {
    randMin = rand_min;
    randMax = rand_max;
    double randTemp = (double)(rand() % int(randBase) / randBase);
    return rand_min + randTemp * (rand_max - rand_min);
}

vector<CEA::PopulationStruct> CEA::produceOffSpring(int x, int y) {
    //select individuals in the neiberhood
    //up offspring
    vector<PopulationStruct> offSprings{};
    PopulationStruct parentA = population[x][y];
    //up offspring
    if(y - 1 >= 0){
        PopulationStruct parentB = population[x][y-1];
        mate(parentA, parentB, 0.1);
        offSprings.push_back(parentB);
    }
    //down offspring
    if(y + 1 < populationNumberLength){
        PopulationStruct parentB = population[x][y+1];
        mate(parentA, parentB, 0.1);
        offSprings.push_back(parentB);
    }
    //left offspring
    if(x - 1 >= 0){
        PopulationStruct parentB = population[x][x-1];
        mate(parentA, parentB, 0.1);
        offSprings.push_back(parentB);
    }
    //right off spring
    if(x + 1 < populationNumberLength){
        PopulationStruct parentB = population[x][x+1];
        mate(parentA, parentB, 0.1);
        offSprings.push_back(parentB);
    }
    return offSprings;
}

void CEA::mate(PopulationStruct parent_A, PopulationStruct &parent_B, double mutaion_rate) {
    //crossover
    int crossposition = rand() % int(populationPropertiesSize);
    for (int i = 0; i < crossposition; i++){
        parent_B.populationProperties[i] = parent_A.populationProperties[i];
    }
    //mutate
    if (rand() < int(mutaion_rate * RAND_MAX)){
        int mutateposition = rand()% int(populationPropertiesSize);
        parent_B.populationProperties[mutateposition] = getDoubleRand(randMin, randMax);
    }
}