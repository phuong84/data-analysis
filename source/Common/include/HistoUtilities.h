/**
 * \class    HistoUtilities
 * \ingroup  Common
 *
 * \brief    Tools for histogram
 *
 * This class provides utility tools for working with histograms;
 * e.g. read, write , merge and rotate histograms. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     HistoUtilities.h
 * 
 */

#include <vector>
#include <TKey.h>
#include <TObject.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>
#include <TString.h>
#include "ErrHandler.h"

#ifndef __HistoUtilities__
#define __HistoUtilities__

class HistoUtilities {

public:
	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	HistoUtilities() : message("HistoUtilities") {};
	
	/// \brief Class destructor
	~HistoUtilities() {};
	
	/// \brief Get all histograms from file
	/// \param histolist vector of histograms retrieved from file
	/// \param file pointer of TFile
	void getHistosFromFile(std::vector<TH1*> &histolist, TFile* file);
	
	/// \brief Get all histograms with specific names from file
	/// \param histolist vector of histograms retrieved from file
	/// \param histoname vector of specific histogram names
	/// \param file pointer of \a TFile object
	void getHistosFromFile(std::vector<TH1*> &histolist, std::vector<TString> histoname, TFile* file);
	
	/// \brief Merge all histograms in file
	/// \param file pointer of \a TFile object
	void mergeHistos(TFile* file);
	
	/// \brief Write a histogram to file
	/// \param hist pointer to histogram object
	/// \param file pointer of \a TFile object
	void writeHisto(TH1* hist, TFile* file);
	
	/// \brief Write more than one histogram to file
	/// \param hist vector of histogram object pointer
	/// \param file pointer of \a TFile object
	void writeHistos(std::vector<TH1*> hist, TFile* file);
	
	/// \brief Convert 1D vector to histogram
	/// \param name name of created histogram
	/// \param title title of created histogram
	/// \param data 1-dimension vector
	/// \param min minimum range of histogram
	/// \param max maximum range of histogram
	/// \return 1-dimension histogram
	TH1F* convert(TString name, TString title, std::vector<double> data, double min, double max);
	
	/// \brief Convert 2D vector to histogram
	/// \param name name of created histogram
	/// \param title title of created histogram
	/// \param data 2-dimension vector
	/// \param xmin minimum range of histogram x-axis
	/// \param xmax maximum range of histogram x-axis
	/// \param ymin minimum range of histogram y-axis
	/// \param ymax maximum range of histogram y-axis
	/// \return 2-dimension histogram
	TH2F* convert(TString name, TString title, std::vector< std::vector<double> > data, double xmin, double xmax, double ymin, double ymax);
	
	/// \brief Convert 3D vector to histogram
	/// \param name name of created histogram
	/// \param title title of created histogram
	/// \param data 3-dimension vector
	/// \param xmin minimum range of histogram x-axis
	/// \param xmax maximum range of histogram x-axis
	/// \param ymin minimum range of histogram y-axis
	/// \param ymax maximum range of histogram y-axis
	/// \param zmin minimum range of histogram z-axis
	/// \param zmax maximum range of histogram z-axis
	/// \return 3-dimension histogram
	TH3F* convert(TString name, TString title, std::vector< std::vector< std::vector<double> > > data, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
	
	/// \brief Rotate 3D histogram
	/// \param hist 3-dimension histogram
	/// \param option option for rotation
	/// \param name name of new histogram
	/// \return 3-dimension histogram
	TH3F* changeAxis(TH3F* hist, TString option, TString name);
private:
	ErrHandler message;  ///< label of class to print out with message
};

#endif
