#include <iostream>
#include <vector>
#include "VisualCore/LineChart.h"
#include "GeneticAlgorithmCore/GeneticAlgorithm.h"
#include "TikhonovSVDCore/TikhonovSVD.h"
#define POPULATIONNUMBERLENGTH 100
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <math.h>
#include "CEA.h"
#define MATRIXSIZE 40
#define GA_POPSIZE 200
#define d 0.25f

using namespace std;

class GeoMeasure{
    gsl_vector* fexact;
    double b0d,b1d, b2d;
public:
    gsl_matrix* A;
    gsl_vector* b;
    GeoMeasure(double b0_d, double b1_d, double b2_d):b0d(b0_d), b1d(b1_d), b2d(b2_d){
        A = gsl_matrix_alloc(MATRIXSIZE, MATRIXSIZE);
        fexact = gsl_vector_alloc(MATRIXSIZE);
        b = gsl_vector_alloc(MATRIXSIZE);
        initA();
        initfexact();
        calculateb();
    }
    ~GeoMeasure(){
        gsl_matrix_free(A);
        gsl_vector_free(fexact);
        gsl_vector_free(b);
    }
private:
    void initA(){
        for (size_t i = 0; i < MATRIXSIZE; i++){
            for (size_t j = 0; j< MATRIXSIZE; j++){
                double s = 1.0/MATRIXSIZE * (double(j) + 0.5);
                double t = 1.0/MATRIXSIZE * (double(i) + 0.5);
                gsl_matrix_set (A, i, j, 1.0/MATRIXSIZE * (d)/pow(pow((d), 2.0) + pow(s-t, 2.0), 1.5));
            }
        }
    }
    void initfexact(){
        for (size_t i =0; i< MATRIXSIZE; i++){
            if (i<8){
                gsl_vector_set (fexact, i, 2);
            }
            else{
                gsl_vector_set (fexact, i, 1);
            }
        }
    }
    void plotfexact(){
        vector<double> x(MATRIXSIZE);
        vector<double> y(MATRIXSIZE);
        LineChart lineChart;
        for (int i = 0; i < MATRIXSIZE; i++){
            x[i] = i;
            y[i] = gsl_vector_get(fexact, i);
        }
        lineChart.drawchart(x, y,  1);
    }
    void plotbexact(){
        vector<double> x(MATRIXSIZE);
        vector<double> y(MATRIXSIZE);
        LineChart lineChart;
        for (int i = 0; i < MATRIXSIZE; i++){
            x[i] = i;
            y[i] = gsl_vector_get(b, i);
        }
        lineChart.drawchart(x, y,  1);
    }
    void calculateb(){
        gsl_blas_dgemv(CblasNoTrans, 1.0, A, fexact, 0.0, b);
        gsl_vector_set(b, 0, -0.2 + gsl_vector_get(b, 0));
        gsl_vector_set(b, 0, b0d + gsl_vector_get(b, 0));
        gsl_vector_set(b, 1, b1d + gsl_vector_get(b, 1));
        gsl_vector_set(b, 2, b2d + gsl_vector_get(b, 2));
    }
};

class FitessSVDFunc{
public:
    FitessSVDFunc(){

    }
    template<typename T>
    void singlepopulationfitness(T &singlepopulation){
        GeoMeasure geoMeasure(singlepopulation.populationProperties[0], singlepopulation.populationProperties[1], singlepopulation.populationProperties[2]);
        vector<double> lambdasweep {1E-16, 1E-15, 1E-14, 2E-14, 5E-14, 1E-13, 2E-13, 5E-13, 1E-12, 2E-12 ,5E-12, 1E-11, 2E-11, 5E-11, 1E-10, 1E-9, 1E-8};
        size_t lambdasize = lambdasweep.size();
        vector<double> gValuearray (lambdasize, 0.0);
        for (int k = 0; k < lambdasize; k++){
            TikhonovSVD tikhonovSVD(geoMeasure.A, geoMeasure.b);
            tikhonovSVD.getXtikhonovSVD(lambdasweep[k]);
            double Gvalue = tikhonovSVD.getGCVValue(geoMeasure.A,geoMeasure.b, tikhonovSVD.XTikhonovSVD);
            gValuearray[k] = Gvalue ;
        }
        double miniumvalue = *min_element(gValuearray.begin(), gValuearray.end());
        singlepopulation.fitness = miniumvalue;
    }

    template<typename T>
    void calc_fitness(T &population, int PopulationSize){
        for (int i = 0; i< PopulationSize; i++){
            singlepopulationfitness(population[i]);
        }

    }
};

void geneticSVDcalculation(){
    vector<double> populationproperties(3, 0.0);
    GeneticAlgorithm<vector<double>> geneticAlgorithm(populationproperties,GA_POPSIZE);
    geneticAlgorithm.initPopulation(-1, 1, 10);
    FitessSVDFunc fitessSVDFunc;
    for (int i =0; i < 10000; i++) {
        fitessSVDFunc.calc_fitness((*geneticAlgorithm.populationP), GA_POPSIZE);
        geneticAlgorithm.sortByFitness();
        cout << "Best: " << " " << (*geneticAlgorithm.populationP)[0].populationProperties[0] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[1] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[2] << " (" << (*geneticAlgorithm.populationP)[0].fitness << ")\n";
        geneticAlgorithm.mate(0.1, 0.2);
    }
}

void getBestFitLambdaValue(GeoMeasure& geoMeasure){
    vector<double> lambdasweep {1E-12, 1E-11};
    size_t lambdasize = lambdasweep.size();
    vector<double> gValuearray (lambdasize, 0.0);
    for (int k = 0; k < lambdasize; k++){
        TikhonovSVD tikhonovSVD(geoMeasure.A, geoMeasure.b);
        tikhonovSVD.getXtikhonovSVD(lambdasweep[k]);
        double Gvalue = tikhonovSVD.getGCVValue(geoMeasure.A,geoMeasure.b, tikhonovSVD.XTikhonovSVD);
        gValuearray[k] = Gvalue ;
    }
    LineChart lineChart;
    lineChart.drawchart(lambdasweep, gValuearray, 1);

}

void reconstruct(){
    GeoMeasure geoMeasure(0.2, 0 , 0);
    TikhonovSVD tikhonovSVD(geoMeasure.A, geoMeasure.b);
    tikhonovSVD.getXtikhonovSVD(1E-12);
    LineChart lineChart;
    vector<double> x(MATRIXSIZE);
    vector<double> y(MATRIXSIZE);
    for (int i = 0; i < MATRIXSIZE; i++){
        x[i] = i;
        y[i] = gsl_vector_get(tikhonovSVD.XTikhonovSVD, i);
    }
    lineChart.drawchart(x, y, 1);
}

void distributedGeneticAlgorithm(){
    vector<GeneticAlgorithm<vector<double>>*> geneticworld = {};
    for (int world = 0; world < 5; world++){
        cout << "world:" + to_string(world) << endl;
        vector<double> populationproperties(3, 0.0);
        GeneticAlgorithm<vector<double>>* geneticAlgorithmp = new GeneticAlgorithm<vector<double>>(populationproperties,GA_POPSIZE);
        geneticworld.push_back(geneticAlgorithmp);
        (*geneticAlgorithmp).initPopulation(-1, 1, 10);
        FitessSVDFunc fitessSVDFunc;
        for (int i =0; i < 5; i++) {
            cout << "generation:" + to_string(i) << endl;
            fitessSVDFunc.calc_fitness((*(*geneticAlgorithmp).populationP), GA_POPSIZE);
            (*geneticAlgorithmp).sortByFitness();
            //cout << "Best: " << " " << (*geneticAlgorithm.populationP)[0].populationProperties[0] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[1] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[2] << " (" << (*geneticAlgorithm.populationP)[0].fitness << ")\n";
            (*geneticAlgorithmp).mate(0.1, 0.2);
        }
    }
    vector<double> populationproperties(3, 0.0);
    GeneticAlgorithm<vector<double>> geneticAlgorithm(populationproperties,GA_POPSIZE*5);
    geneticAlgorithm.randomMin = -1;
    geneticAlgorithm.randMax = 1;
    geneticAlgorithm.randBase = 10;

    int i = 0;
    for (auto geneticworldunit:geneticworld){
        for (int j = 0; j < GA_POPSIZE; j++){
            ((*geneticAlgorithm.populationP)[i]) = (*(*geneticworldunit).populationP)[j];
            i++;
        }
    }
    FitessSVDFunc fitessSVDFunc;
    for (int i =0; i < 10000; i++) {
        fitessSVDFunc.calc_fitness((*geneticAlgorithm.populationP), GA_POPSIZE*5);
        geneticAlgorithm.sortByFitness();
        cout << "Best: " << " " << (*geneticAlgorithm.populationP)[0].populationProperties[0] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[1] << " " << (*geneticAlgorithm.populationP)[0].populationProperties[2] << " (" << (*geneticAlgorithm.populationP)[0].fitness << ")\n";
        geneticAlgorithm.mate(0.1, 0.2);
    }

}
void printBestValue(CEA::PopulationVector population){
    vector<CEA::PopulationStruct> tournamentVector;
    for (auto populationRow:population){
        sort(populationRow.begin(), populationRow.end(),
             [](CEA::PopulationStruct x, CEA::PopulationStruct y){ return x.fitness < y.fitness; });
        tournamentVector.push_back(populationRow[0]);
    }
    sort(tournamentVector.begin(), tournamentVector.end(),
         [](CEA::PopulationStruct x, CEA::PopulationStruct y){ return x.fitness < y.fitness; });
    cout << "Best: " << " " << tournamentVector[0].populationProperties[0] << " " << tournamentVector[0].populationProperties[1] << " " << tournamentVector[0].populationProperties[2] << " (" << tournamentVector[0].fitness << ")\n";
}

class FitessSVDFuncCEA{
public:
    FitessSVDFuncCEA(){

    }
    template<typename T>
    double singlepopulationfitness(T singlepopulation){
        GeoMeasure geoMeasure(singlepopulation.populationProperties[0], singlepopulation.populationProperties[1], singlepopulation.populationProperties[2]);
        vector<double> lambdasweep {1E-11};
        size_t lambdasize = lambdasweep.size();
        vector<double> gValuearray (lambdasize, 0.0);
        for (int k = 0; k < lambdasize; k++){
            TikhonovSVD tikhonovSVD(geoMeasure.A, geoMeasure.b);
            tikhonovSVD.getXtikhonovSVD(lambdasweep[k]);
            double Gvalue = tikhonovSVD.getGCVValue(geoMeasure.A,geoMeasure.b, tikhonovSVD.XTikhonovSVD);
            gValuearray[k] = Gvalue ;
        }
        double miniumvalue = *min_element(gValuearray.begin(), gValuearray.end());
        return miniumvalue;
    }

};


void calculateCEA(){
    srand(unsigned(time(NULL)));
    vector<double> populationProperties(3, 0.0);
    CEA cea(populationProperties, POPULATIONNUMBERLENGTH);
    cea.initIndividual(-1, 1, 100);
    FitessSVDFuncCEA fitessSVDFunc;
    for (int loop = 0; loop < 1000; loop++){
        // for each node in the population
        for (int i = 0; i < POPULATIONNUMBERLENGTH; i++){
            for (int j = 0; j < POPULATIONNUMBERLENGTH; j++){
                //evaluate individual at node
                cea.population[i][j].fitness = fitessSVDFunc.singlepopulationfitness(cea.population[i][j]);
                //produce offspring
                auto offsprings = cea.produceOffSpring(i, j);
                //evaluate offspring
                for (auto& offspring:offsprings){
                    offspring.fitness = fitessSVDFunc.singlepopulationfitness(offspring);
                }
                //assign one of the offspring to node according to a given criterion using binary tournament
                vector<CEA::PopulationStruct> Tournamentoffspring;
                int offspringAindex = rand() % int(offsprings.size());
                Tournamentoffspring.push_back(offsprings[offspringAindex]);
                offsprings.erase(offsprings.begin() + offspringAindex);
                int offspringBindex = rand() % int(offsprings.size());
                Tournamentoffspring.push_back(offsprings[offspringBindex]);
                if (Tournamentoffspring[0].fitness <= Tournamentoffspring[0].fitness){
                    if (Tournamentoffspring[0].fitness < cea.population[i][j].fitness){
                        cea.population[i][j] = Tournamentoffspring[0];
                    }
                }
            }
        }
        printBestValue(cea.population);
    }
}


int main() {
    calculateCEA();
    return 0;
}
