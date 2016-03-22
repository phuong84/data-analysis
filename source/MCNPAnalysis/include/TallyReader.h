/**
 * \class    TallyReader
 * \ingroup  MCNPAnalysis
 *
 * \brief    Read MCNP tally output
 *
 * This class can be used to get MCNP tally results.
 * The results will be written out to a output root file with
 * histograms format, and then we can read these histograms and 
 * analysis results. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     TallyReader.h
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include "ErrHandler.h"
#include "StringParser.h"
#include "HistoUtilities.h"

#ifndef __TallyReader__
#define __TallyReader__

class TallyReader {

public:

	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	TallyReader() : message("TallyReader") {};
	
	/// \brief Class destructor
	~TallyReader() {};
	
	/// \brief Read MCNP output file
	/// \param filename name of MCNP output file
	void read(TString filename);
	
	/// \brief Extract mesh histograms to root file
	/// \param filename name of root file
	/// \param isUpdate add histograms to existing file
	void extractHisto(TString filename, bool isUpdate = false);
	
	enum Tag{INFO, TALLYINFO, VALUE}; ///< Types of information
	
private:

	/// \brief Process line information
	/// \param tag type of information to process
	/// \param line information string line 
	void process(Tag tag, std::string line);
	
	/// \brief Read information
	/// \param line information string line
	void readInfo(std::string line);
	
	/// \brief Read tally information
	/// \param line information string line
	void readTallyInfo(std::string line);
	
	/// \brief Read MCNP results
	/// \param line information string line
	void readValue(std::string line);
	
	std::vector<int> tally;                      ///< Tally number vector
	std::vector<int> nps;                        ///< Number of histories vector
	std::vector< std::vector<double> > energies; ///< Energy vector
	std::vector< std::vector<double> > values;   ///< MCNP result vector
	std::vector< std::vector<double> > errors;   ///< MCNP result error vector
	std::vector<double> energy;                  ///< Temporary energy vector
	std::vector<double> value;                   ///< Temporary MCNP result vector
	std::vector<double> error;                   ///< Temporary MCNP result error vector
	int raw_nps;                                 ///< Original number of histories
	ErrHandler message;	                         ///< Label of class to print out with message
};

#endif
