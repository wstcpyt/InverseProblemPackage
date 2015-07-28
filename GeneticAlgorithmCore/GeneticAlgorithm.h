//
// Created by yutong pang on 7/14/15.
//

#ifndef INVERSEPROBLEM_GENETICALGORITHM_H
#define INVERSEPROBLEM_GENETICALGORITHM_H

#include <vector>
using std::vector;


//! Main Class for Genetic Algorithm
/*!
  It has a population group which is easy to initialize. We also make it easy to
  do crossover and mutation in the population group.

  Genetic Algorithm is a class template. You can create the class accroding to
  different kind of properties type of the unit in the population.

  @tparam PopulationPropertiesType the type of populationProperties.
*/
template <class PopulationPropertiesType>
class GeneticAlgorithm {
    //!Struct for a unit in the population.
    struct GaStruct
    {
        /*!
          Properties (Parameter Space) for a unit in the population
        */
        PopulationPropertiesType populationProperties;
        /*!
          Fitness value. Store the fitness of a unit in the population
        */
        double fitness;
    };
    unsigned int populationProperitesSize;
    GaStruct gaStruct;
    typedef vector<GaStruct> GaVector;
    unsigned int populationNumber;
    GaVector population;
    GaVector buffer;
    double randomMin, randMax;
    double randBase;
public:
    //! Constructor for GeneticAlgorithm class
    /*!
    <b>Example:</b>

    The following is used to create a geneticAlgoritm object with a populationProperties of
    `vector<double>` type and the number of units in the whole population is 1000.
    @verbatim embed:rst
    .. code-block:: c

      #include <vector>
      using namespace std;
      vector<double> populationProperties (10, 0.0);
      int populationNumber = 1000;
      GeneticAlgoritm<vector<double>> geneticAlgoritm(populationProperties, populationNumber);
    @endverbatim

    @param population_properties Properties (Parameter Space) for a unit in the population.
    @param population_number Number of units in the whole population.
    */
    GeneticAlgorithm(PopulationPropertiesType population_properties, unsigned int population_number);

    const GaStruct &getGaStruct() const {
        return gaStruct;
    }

    unsigned int getPopulationNumber() const {
        return populationNumber;
    }

    unsigned int getPopulationproperitessize() const {
        return populationProperitesSize;
    }
    //! Population Pointer
    /*!
    A GaVector pointer point to the population member.

    GaVector is actually an alias that define by `typedef` to replace `vector<GaStruct>`.
    @verbatim embed:rst
    .. code-block:: c

      typedef vector<GaStruct> GaVector;
    @endverbatim
    */
    GaVector *populationP;
    //! Buffer Pointer
    /*!
      A GaVector pointer pouint to the buffer member. Buffer with GaVector type serve as
      a temporary buffer layer to store population.
    */
    GaVector *bufferP;
    //! Initialize the population group.
    /*!
    Every Properties for the unit in the population group will initialize
    with a random number. The random number will generate in the following way:
    @verbatim embed:rst
    .. code-block:: c

      double fraction = (double)(rand() % int(rand_base) / rand_base);
      double randomNumber = rand_min + fraction * (rand_max - rand_min);
    @endverbatim
    */
    /*!
    @param rand_min Random number lower bound.
    @param rand_max Random number upper bound.
    @param rand_base Random number generation base.
    */
    void initPopulation(double rand_min, double rand_max, double rand_base);
    //! Sort the population by the fitness of the unit from low to high.
    void sortByFitness();
    //! Generate next generation of population by mutation and crossover.
    /*!
    @param elitrate elitism rate of the Genetic Algorithm. Elitism unit is the
    unit directly transfer to the next generation without mutation and crossover.
    @param mutationrate Rate for the Mutation.
    */
    void mate(double elitrate, double mutationrate);

private:
    double doubleRand(double rand_min, double rand_max);
    void eltism(int e_size);
    void mutate(GaStruct &mutatemember);
    void swap();
};

#endif //INVERSEPROBLEM_GENETICALGORITHM_H
