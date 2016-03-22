/**
 * \class    GAFitter
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Fitting spectrum using genetic algorithm method
 *
 * This class provides fitting method for full spectrum analysis
 * by using genetic algorithm. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     GAFitter.h
 * 
 */

#include <TRandom3.h>
#include "BaseFitter.h"

#ifndef __GAFitter__
#define __GAFitter__

/**
 * \class GeneticType
 * \ingroup  SpectrumAnalysis
 * \brief Structure of genetic individual 
 */
struct GeneticType {
		std::vector<double> gene;
		double fitness;
		bool operator<(const GeneticType& a) const {
			return fitness < a.fitness;
		}
	};

class GAFitter : public BaseFitter {

public:

	/// \brief Class constructor
	GAFitter() : BaseFitter() {};
	
	/// \brief Class destructor
	~GAFitter() {};
	
	/// \brief Set parameters
	/// \param pars vector of parameters
	void setParameters(double* pars);
	
	/// \brief Fit measured spectrum
	/// \param spectrum measured spectrum
	/// \param useWeight do use weight
	void fit(Spectrum spectrum, bool useWeight);

private:

	/// \brief Initialize population
	void initialize();
	
	/// \brief Process selection 
	void process();
	
	/// \brief Finalize results
	void finalize();
	
	/// \brief Create new population
	void create_new();
	
	/// \brief Mating between individuals
	void crossover();
	
	/// \brief Mutate individuals
	void mutate();
	
	/// \brief Swap two genetic parts from two individuals 
	/// \param i first individual number
	/// \param j second individual number
	/// \param pos position for swapping
	void swap(int i, int j, int pos);
	
	/// \brief Print report
	/// \param generation the generation to be printed out
	void report(int generation);
	
	/// \brief Generate individuals
	GeneticType generate();

	/// \brief Object function
	double obj_func(std::vector<double> val);
	
	std::vector<double> m_bound;          ///< boundaries for individual
	int m_popsize;                        ///< size of population
	int m_ngenerations;                   ///< number of generations
	int m_nvals;                          ///< number of genes
	int m_nchannels;                      ///< number of spectrum channels
	double m_pmutation;                   ///< mutation probability
	double m_pcrossover;                  ///< crossover probability
	double m_psurvive;                    ///< survive probability
	std::vector<GeneticType> population;  ///< vector of populations
};

#endif
