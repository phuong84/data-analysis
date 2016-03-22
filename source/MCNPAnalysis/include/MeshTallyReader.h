/**
 * \class    MeshTallyReader
 * \ingroup  MCNPAnalysis
 *
 * \brief    Read MCNP mesh tally output
 *
 * This class can be used to get MCNP mesh tally results.
 * The results will be written out to a output root file with
 * histograms format, and then we can read these histograms and 
 * analysis results. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     MeshTallyReader.h
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include "ErrHandler.h"
#include "StringParser.h"
#include "HistoUtilities.h"

#ifndef __MeshTallyReader__
#define __MeshTallyReader__

class MeshTallyReader {

public:

	/// \brief Class constructor
	MeshTallyReader();
	
	/// \brief Class destructor
	~MeshTallyReader() {};
	
	/// \brief Read MCNP mesh file
	/// \param filename name of MCNP mesh file
	void read(TString filename);
	
	/// \brief Extract mesh histograms to output file
	/// \param filename name of output file
	/// \param isUpdate add histograms to existing file
	void extractHisto(TString filename, bool isUpdate = false);
	
	enum Tag{INFO, BOUND, VALUE, ERROR}; ///< Types of information
	enum Plane{NONE, XY, YZ, XZ};        ///< Types of mesh plane

private:

	/// \brief Process line information
	/// \param tag type of information to process
	/// \param line information string line 
	void process(Tag tag, std::string line);
	
	/// \brief Read information
	/// \param line information string line
	void readInfo(std::string line);
	
	/// \brief Read axis binning values
	/// \param line information string line
	void readBound(std::string line);
	
	/// \brief Read mesh value
	/// \param line information string line
	void readValue(std::string line);
	
	/// \brief Read mesh value error
	/// \param line information string line
	void readError(std::string line);
	
	std::vector< std::vector< std::vector<double> > > m_meshVal; ///< Mesh value vector
	std::vector< std::vector< std::vector<double> > > m_meshErr; ///< Mesh value error vector
	std::vector< std::vector<double> > m_val;                    ///< Temporary value vector
	std::vector< std::vector<double> > m_err;                    ///< Temporary value error vector
	TString m_histoname;                                         ///< Name of writeout histograms
	int nps;                                                     ///< Number of histories
	int tally;                                                   ///< Tally number
	Plane plane;                                                 ///< Mesh plane type
	
	/** Axis boundary values */
	//@{
	double xlow, xhigh, ylow, yhigh, zlow, zhigh;   
	//@}
	/** Axis bins */
	//@{
	int xbin, ybin, zbin; 
	//@}
	ErrHandler message;                                          ///< Label of class to print out with message
};

#endif
