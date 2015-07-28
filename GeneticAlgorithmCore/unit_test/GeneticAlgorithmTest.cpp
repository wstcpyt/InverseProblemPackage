//
// Created by yutong pang on 7/14/15.
//
#include <gmock/gmock.h>
#include "GeneticAlgorithmCore/GeneticAlgorithm.h"
#include <vector>
#include <iostream>
#include <time.h>

using namespace testing;
using std::vector;
using std::cout;


class GeneticAlgorithmTest: public Test{
public:
    vector<double> populationproperties = {0.0, 1.0, 2.0};
    unsigned int populationNumber = 10;
    GeneticAlgorithm<vector<double>> geneticAlgorithm = GeneticAlgorithm<vector<double>>::GeneticAlgorithm(populationproperties, populationNumber);
};
TEST_F(GeneticAlgorithmTest, PopulationConstructor){
    ASSERT_THAT(geneticAlgorithm.getGaStruct().fitness, 0);
    ASSERT_THAT(geneticAlgorithm.getGaStruct().populationProperties[0], 0.0);
    ASSERT_THAT(geneticAlgorithm.getGaStruct().populationProperties[1], 1.0);
    ASSERT_THAT(geneticAlgorithm.getGaStruct().populationProperties[2], 2.0);
    ASSERT_THAT(geneticAlgorithm.getPopulationNumber(), 10);
    ASSERT_THAT((*geneticAlgorithm.bufferP).size(), populationNumber);
    ASSERT_THAT((*geneticAlgorithm.populationP).size(), populationNumber);
    ASSERT_THAT(geneticAlgorithm.getPopulationproperitessize(), populationproperties.size());
}

TEST_F(GeneticAlgorithmTest, PopulationInit){
    double rand_min = 1.0;
    double rand_max = 4.0;
    geneticAlgorithm.initPopulation(rand_min, rand_max, 100);
    ASSERT_THAT((*geneticAlgorithm.populationP).size(), populationNumber);
    ASSERT_THAT((*geneticAlgorithm.populationP)[0].populationProperties[0], Ge(rand_min));
    ASSERT_THAT((*geneticAlgorithm.populationP)[0].populationProperties[0], Le(rand_max));
}

TEST_F(GeneticAlgorithmTest, sortByFitness){
    geneticAlgorithm.initPopulation(1.0, 5.0, 100);
    for (auto &popind:(*geneticAlgorithm.populationP)){
       popind.fitness = popind.populationProperties[0];
    }
    geneticAlgorithm.sortByFitness();
    ASSERT_THAT((*geneticAlgorithm.populationP)[0].fitness, Le((*geneticAlgorithm.populationP)[1].fitness));
    ASSERT_THAT((*geneticAlgorithm.populationP)[1].fitness, Le((*geneticAlgorithm.populationP)[2].fitness));
}

TEST_F(GeneticAlgorithmTest, mate){
    auto bufferP_before = geneticAlgorithm.bufferP;
    auto populationP_before = geneticAlgorithm.populationP;
    geneticAlgorithm.mate(0.1,0.2);
    ASSERT_THAT(geneticAlgorithm.populationP, bufferP_before);
    ASSERT_THAT(geneticAlgorithm.bufferP, populationP_before);
}