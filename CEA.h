//
// Created by yutong pang on 8/4/15.
//

#ifndef CEA_CEA_H
#define CEA_CEA_H

#include <vector>
using namespace std;
class CEA{
public:
    struct PopulationStruct {
        vector<double> populationProperties;
        double fitness;
    };
    CEA(vector<double> population_properties, unsigned int population_number);
    void initIndividual(double rand_min, double rand_max, double rand_base);
    vector<PopulationStruct> produceOffSpring(int x, int y);
    typedef vector<vector<PopulationStruct>> PopulationVector;
private:
    PopulationStruct populationStruct;
    PopulationVector buuffer;
    int populationNumberLength;
    size_t populationPropertiesSize;
    double randBase;
    double randMin;
    double randMax;
    double getDoubleRand(double rand_min, double rand_max);
    void mate(PopulationStruct parent_A, PopulationStruct& parent_B, double mutaion_rate);

public:
    PopulationVector population;
};

#endif //CEA_CEA_H
