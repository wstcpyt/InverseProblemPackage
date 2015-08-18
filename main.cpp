#include <iostream>
#include <vector>
#include "VisualCore/LineChart.h"
#include "GeneticAlgorithmCore/GeneticAlgorithm.h"
#include "TikhonovSVDCore/TikhonovSVD.h"
#define POPULATIONNUMBERLENGTHPERTHREAD 10
#define NUMBEROFTHREAD 30
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <math.h>
#include <future>
#include "CEA.h"
#define MATRIXSIZE 40
#define GA_POPSIZE 200
#define d 0.25f

using namespace std;


class GeoMeasure{
    gsl_vector* fexact;
public:
    gsl_matrix* A;
    gsl_vector* b;
    GeoMeasure(vector<double> blist){
        A = gsl_matrix_alloc(MATRIXSIZE, MATRIXSIZE);
        fexact = gsl_vector_alloc(MATRIXSIZE);
        b = gsl_vector_alloc(MATRIXSIZE);
        initA();
        initfexact();
        calculateb();
        for (int i = 0; i < blist.size(); i++){
            gsl_vector_set(b, i, blist[i] + gsl_vector_get(b, i));
        }
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
    }
};



#include <fstream>

void writeToFiile(int generation, CEA::PopulationVector population){
    ofstream myfile;
    myfile.open("populationgrid/generation"+ to_string(generation) + ".txt");

    for (auto populationRow:population){
        for (int i = 0; i < POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD; i++){
            myfile << populationRow[i].fitness << " ";
        }
        myfile << "\n";
    }
    myfile.close();
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
    cout << "Best: " << " "  << " (" << tournamentVector[0].fitness << ") ";
    for (int i = 0; i < tournamentVector[0].populationProperties.size(); i++){
        cout << tournamentVector[0].populationProperties[i] << " ";
    }
    cout << endl;
}



class FitessSVDFuncCEA{
public:
    FitessSVDFuncCEA(){

    }
    template<typename T>
    double singlepopulationfitness(T singlepopulation){
        vector<double> blist {};
        for (double propertiesvalue:singlepopulation.populationProperties){
            blist.push_back(propertiesvalue);
        }
        GeoMeasure geoMeasure(blist);
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

void singlegenerationprocess(int i, int j, CEA& cea){
    FitessSVDFuncCEA fitessSVDFunc;
    //evaluate individual at node
    double fitnessvalue = fitessSVDFunc.singlepopulationfitness(cea.population[i][j]);
    cea.population[i][j].fitness = fitnessvalue;
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
    double result;
    if (Tournamentoffspring[0].fitness <= Tournamentoffspring[1].fitness){
        if (Tournamentoffspring[0].fitness < cea.population[i][j].fitness){
            cea.population[i][j] = Tournamentoffspring[0];
        }
    }
    else{
        if (Tournamentoffspring[1].fitness < cea.population[i][j].fitness){
            cea.population[i][j] = Tournamentoffspring[1];
        }
    }
}
void calculateCEA(){
    srand(unsigned(time(NULL)));
    vector<double> populationProperties(3, 0.0);
    CEA cea(populationProperties, POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD);
    cea.initIndividual(-1, 1, 100);
    FitessSVDFuncCEA fitessSVDFunc;
    for (int loop = 0; loop < 1000; loop++){
        // for each node in the population
        for (int i = 0; i < POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD; i++){
            for (int j = 0; j < POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD; j++){
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
                if (Tournamentoffspring[0].fitness <= Tournamentoffspring[1].fitness){
                    if (Tournamentoffspring[0].fitness < cea.population[i][j].fitness){
                        cea.population[i][j] = Tournamentoffspring[0];
                    }
                }
                else{
                    if (Tournamentoffspring[1].fitness < cea.population[i][j].fitness){
                        cea.population[i][j] = Tournamentoffspring[1];
                    }
                }
            }
        }
        writeToFiile(loop, cea.population);
        printBestValue(cea.population);
    }
}
void calculateCEAmultithreading(){
    int numberofthread = 2;
    srand(unsigned(time(NULL)));
    vector<double> populationProperties(40, 0.0);
    CEA cea(populationProperties, POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD);
    cea.initIndividual(-0.1, 0.1, 10);
    for (int loop = 0; loop < 1000; loop++){
        // for each node in the population
        for (int i = 0; i < POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD; i++){
            vector<future<CEA>> futures{};
            for (int thread = 0; thread < NUMBEROFTHREAD; thread++){
                futures.push_back(async(launch::async,[&, i, thread]()->CEA{
                    auto ceatemp = cea;
                    for (int j = thread*POPULATIONNUMBERLENGTHPERTHREAD; j < (thread+1) * POPULATIONNUMBERLENGTHPERTHREAD; j++){
                        singlegenerationprocess(i, j, ceatemp);
                    }
                    return ceatemp;
                }));
            }
            for (int thread = 0; thread < NUMBEROFTHREAD; thread++){
                auto ceatemp = futures[thread].get();
                for (int x = 0; x < POPULATIONNUMBERLENGTHPERTHREAD*NUMBEROFTHREAD; x++){
                    for(int y = thread*POPULATIONNUMBERLENGTHPERTHREAD; y < (thread+1) * POPULATIONNUMBERLENGTHPERTHREAD; y++){
                        cea.population[x][y] = ceatemp.population[x][y];
                    }
                }
            }
        }

        writeToFiile(loop, cea.population);
        printBestValue(cea.population);
    }
}

int main() {
    calculateCEAmultithreading();
    return 0;
}
