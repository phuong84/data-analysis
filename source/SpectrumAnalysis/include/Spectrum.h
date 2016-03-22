/**
 * \class    Spectrum
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Spectrum class
 *
 * This class provides methods for spectrum analysis. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     Spectrum.h
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TString.h>
#include <TMath.h>
#include <TSpectrum.h>
#include "ErrHandler.h"

#ifndef __Spectrum__
#define __Spectrum__

class Spectrum {

public:

	/// \brief Class constructor, initialize member variables
	Spectrum() : m_spectrum(0), m_Nchannels(0), m_calibFormula(""), m_effFormula(""), m_calibCoeff(0), m_effCoeff(0), message("Spectrum") {};
	
	/// \brief Class destructor
	~Spectrum() {};
	
	/// \brief Read spectrum
	/// \param filename name of spectrum file
	/// \param type file type
	void read(TString filename = "", TString type = "TKA");	
	
	/// \brief Set calibration coefficients
	/// \param coeff vector of coefficients
	void setCalibCoeff(std::vector<double> coeff);
	
	/// \brief Set efficiency coefficients
	/// \param coeff vector of coefficients
	void setEffCoeff(std::vector<double> coeff);
	
	/// \brief Scale spectrum
	/// \param x scale factor
	void scale(double x);
	
	/// \brief Get spectrum data
	/// \return spectrum data vector
	double* getData();
	
	/// \brief Get spectrum histogram
	/// \return pointer of spectrum histogram
	TH1* getHistogram();
	
	/// \brief Get number of spectrum channels
	/// \return number of channels
	int getNchannels();
	
	/// \brief Get calibration coefficients
	/// \return vector of coefficients
	std::vector<double> getCalibCoeff();
	
	/// \brief Get calibration coefficients
	/// \param channel channel number
	/// \param energy energy value corresponding to channel number
	/// \return vector of coefficients
	std::vector<double> getCalibCoeff(std::vector<int> channel, std::vector<double> energy);
	
	/// \brief Get efficiency coefficients
	/// \return vector of coefficients
	std::vector<double> getEffCoeff();
	
	/// \brief Get efficiency coefficients
	/// \param energy energy value
	/// \param efficiency efficiency value corresponding to energy
	/// \return vector of coefficients
	std::vector<double> getEffCoeff(std::vector<double> energy, std::vector<double> efficiency);
	
	/// \brief Get energy value
	/// \param channel channel number
	/// \return energy
	double getValue(int channel);
	
	/// \brief Get efficiency value
	/// \param energy energy value
	/// \return efficiency value
	double getEfficiency(double energy);
	
	/// \brief Get efficiency value
	/// \param channel channel number
	/// \return efficiency value
	double getEfficiency(int channel);
	
	/// \brief Fit peak
	/// \param min left boundary of peak
	/// \param max right boundary of peak
	/// \param formula peak fitting formula
	/// \return pointer of TF1 object
	TF1* fitPeak(double min, double max, TString formula = "gaus+expo");
	
	/// \brief Fit multiple peaks
	/// \param N number of peaks
	/// \param min left boundary of peak
	/// \param max right boundary of peak
	/// \return vector of TF1 object pointers
	std::vector<TF1*> fitMultiPeak(const int N, double min, double max);
	
	/// \brief Find peaks
	/// \return vector of peak positions
	std::vector<double> findPeaks(double sigma = 2., double threshold = 0.2);
	
	/// \brief Estimate background
	/// \param Niter number of iterations
	/// \param option estimation method
	/// \return background histogram
	TH1* estimateBkg(int Niter = 50, TString option="BackIncreasingWindow");
	
	/// \brief Subtract background
	/// \param bkg background histogram
	/// \return spectrum histogram after backgrounf subtraction
	TH1* subtractBkg(TH1* bkg);

private:

	/// \brief Read EFTE spectrum
	/// \param file spectrum file stream
	/// \return spectrum histogram
	TH1* readETFE(std::ifstream& file);
	
	/// \brief Read TKA spectrum
	/// \param file spectrum file stream
	/// \return spectrum histogram
	TH1* readTKA(std::ifstream& file);
	
	/// \brief Read MCA spectrum
	/// \param file spectrum file stream
	/// \return spectrum histogram
	TH1* readMCA(std::ifstream& file);
	
	/// \brief Read RRUFF spectrum
	/// \param file spectrum file stream
	/// \return spectrum histogram
	TH1* readRRUFF(std::ifstream& file);
	
	/// \brief Fit data
	/// \param formula fitting formula
	/// \param x data vector
	/// \param y data vector
	/// \return vector of fitted coefficients
	std::vector<double> fitFunc(TString formula, std::vector<double> x, std::vector<double> y);
	
	TH1* m_spectrum;                  ///< spectrum histogram
	int m_Nchannels;                  ///< number of channels
	TString m_calibFormula;           ///< calibration formula
	TString m_effFormula;             ///< efficiency formula
	std::vector<double> m_calibCoeff; ///< calibrated coefficients
	std::vector<double> m_effCoeff;   ///< efficiency coefficients
	ErrHandler message;               ///< Label of class to print out with message
};

#endif
