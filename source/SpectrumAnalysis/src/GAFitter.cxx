/***************************************************************************
 Class  : GAFitter
 File   : GAFitter.cxx
 Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>
 Date   : 25-12-2015    
 ***************************************************************************/

#include "GAFitter.h"

/***************************************************************************/
/**
 * This method sets parameters for GA fitting: \ref m_popsize, \ref m_ngenerations,
 * \ref m_pmutation, \ref m_pcrossover, \ref m_psurvive.
 */
void GAFitter::setParameters(double* pars)
{
	m_popsize      = (int)pars[0];
	m_ngenerations = (int)pars[1];
	m_pmutation    = pars[2];
	m_pcrossover   = pars[3];
	m_psurvive     = pars[4];
}

/***************************************************************************/
/**
 * This method performs fitting measured spectrum using GA method via \ref
 * initialize(), \ref process() and \ref finalize().
 */
void GAFitter::fit(Spectrum spectrum, bool useWeight)
{
	double* spec = spectrum.getData();
	m_nvals = (int)m_mat.size();
	m_nchannels = (int)spectrum.getNchannels();
	m_vec.insert(m_vec.end(),&spec[0],&spec[m_nchannels]);

	initialize();
	process();
	finalize();
}

/***************************************************************************/
/**
 * This method calculates boundaries for generating inidividuals, initializes
 * the first population. 
 */
void GAFitter::initialize()
{
	delete gRandom;
	gRandom = new TRandom3(0);

	// calculate boundaries for random generation
	double sum[m_nvals+1];
	sum[m_nvals] = 0.;
	for(int i = 0; i < m_nchannels; ++i)
		sum[m_nvals] += m_vec[i];
	for(int i = 0; i < m_nvals; ++i) {
		sum[i] = 0.;
		for(int j = 0; j < m_nchannels; ++j)
			sum[i] += m_mat[i][j];
		m_bound.push_back(sum[i]/sum[m_nvals]);
	}
	
	// create first population
	for(int i = 0; i < m_popsize; ++i) {
		GeneticType x = generate();
		population.push_back(x);
	}
}

/***************************************************************************/
/**
 * This method performs GA via \ref crossover(), \ref mutate() and \ref create_new().
 */
void GAFitter::process()
{
	for(int i = 0; i < m_ngenerations; ++i) {
		crossover();
		mutate();
		create_new();
		if(debug_level >= 2) report(i);
	}
}

/***************************************************************************/
/**
 * This method finalizes results, copies the best individual genes to results
 * vector.
 */
void GAFitter::finalize()
{
	m_vals = population[0].gene;
}

/***************************************************************************/
/**
 * This method sorts individuals, and creates new population.
 */
void GAFitter::create_new()
{
	std::sort(population.begin(),population.end());
	for(int i = (int)(m_psurvive*m_popsize); i < m_popsize; ++i) 
			population[i] = generate();
}

/***************************************************************************/
/**
 * This method performs mating between individuals by randomly choosing two
 * individuals and swapping two random parts of them.
 */
void GAFitter::crossover()
{
	for(int i = 0; i < m_pcrossover*m_popsize; ++i) {
		int individual1 = (int)(gRandom->Rndm()*m_popsize);
		int individual2 = (int)(gRandom->Rndm()*m_popsize);
		int pos = (int)(gRandom->Rndm()*m_nvals);
		swap(individual1,individual2,pos);
	}
}

/***************************************************************************/
/**
 * This method performs individual mutation by changing a random gene.
 */
void GAFitter::mutate()
{
	for(int i = 0; i < m_pmutation*m_popsize; ++i) {
		int individual = (int)(gRandom->Rndm()*m_popsize);
		int pos = (int)(gRandom->Rndm()*m_nvals);
		population[individual].gene[pos] = gRandom->Rndm()*m_bound[pos];
		population[individual].fitness = obj_func(population[individual].gene);
	}
}

/***************************************************************************/
/**
 * This method swaps two parts from two individuals.
 */
void GAFitter::swap(int i, int j, int pos)
{
	for(int k = 0; k < pos; ++k) {
		double temp = population[i].gene[k];
		population[i].gene[k] = population[j].gene[k];
		population[j].gene[k] = temp;
	}
}

/***************************************************************************/
/**
 * This method prints out information of a specific generation.
 */
void GAFitter::report(int generation)
{
	if(generation == 0) {
		INFO("Report Genetic algorithm results:");
		std::cout << std::endl;
		std::cout << "  Generation       Best            Average       Standard" << std::endl;
		std::cout << "  number           value           fitness       deviation" << std::endl;
		std::cout << std::endl;
	}
	double sum = 0.0;
	double sum_square = 0.0;

	for(int i = 0; i < m_popsize; ++i) {
		sum += population[i].fitness;
		sum_square += population[i].fitness * population[i].fitness;
	}
	double avg = sum / m_popsize;
	double square_sum = avg * avg * m_popsize;
	double stddev = sqrt( (sum_square-square_sum) / (m_popsize-1) );
	double best_val = population[0].fitness;
	
	std::cout << "  " << std::setw(8)  << generation+1
	          << "  " << std::setw(14) << best_val 
	          << "  " << std::setw(14) << avg 
	          << "  " << std::setw(14) << stddev << std::endl;
}

/***************************************************************************/
/**
 * This method generates random gene values for individual and calculates its
 * fitness (object function value).
 */
GeneticType GAFitter::generate()
{
	GeneticType x;
	std::vector<double> gene;
	for(int i = 0; i < m_nvals; ++i)
		gene.push_back(gRandom->Rndm()*m_bound[i]);
	x.gene = gene;
	x.fitness = obj_func(gene);
	return x;
}

/***************************************************************************/
/**
 * This method returns value of object function (chi^2).
 */
double GAFitter::obj_func(std::vector<double> val)
{
	double chi2 = 0.0;
	for(int i = 0; i < m_nchannels; ++i) {
		double x = m_vec[i];
		for(int j = 0; j < m_nvals; ++j) 
			x -= m_mat[j][i]*val[j];
		chi2 += x*x/m_vec[i];
	}
	return chi2;
}

/***************************************************************************/



