/**
 * \defgroup SpectrumAnalysis Spectrum Analysis
 *
 * \brief    Analysis tools for spectrum
 *
 * This module contains classes for analysing spectrum files 
 * (e.g. X-ray, gamma, sound,... spectra). 
 * 
 * \file     SpectrumAnalysis.cxx
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Contain main function for Spectrum Analysis module
 *
 * This file has the main() function for Spectrum Analysis module.
 * Different procedures for analysing many types of spectra are 
 * implemented in this file.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
#include "ErrHandler.h"
#include "Config.h"
#include "Plotter.h"
#include "Table.h"
#include "Spectrum.h"
#include "GAFitter.h"

void info();
void processSpectrum(Config *config);
void processFSA(Config *config);

ErrHandler message("SpectrumAnalysis");
int debug_level;

/***************************************************************************/
/*                              Main function                              */
/***************************************************************************/
/**
 * This is the main function, calls other functions based on the
 * \a ANALYSIS \a MODE defined in configuration file.  
 */
int main(int argc, char **argv) 
{
	info();

	/******* Read config file *******/
	std::vector<TString> params;
	for (int i = 0; i < argc; i++)
		params.push_back(TString(argv[i]));
	TString configFile = params[1];

	Config* config = new Config();
	config->open(configFile);

	/***** Choose analysis mode ****/
	TString type = config->get("ANALYSIS MODE", "");
	debug_level  = config->get("DEBUG LEVEL"  , 0);

	if      (type == "SPECTRUM") {
		MESSAGE("Analysis mode SPECTRUM");
		processSpectrum(config);
	}
	else if (type == "FSA") {
		MESSAGE("Analysis mode FSA");
		processFSA(config);
	}
	else
		MESSAGE("Undefined analysis mode!");

	/************* END *************/	
	std::cout << std::endl;
	delete config;
	return 0;
}

/***************************************************************************/
/**
 * This is the function for reading and analysing spectra, with configuration 
 * options:
 * * \a File \a Name : name of spectrum files
 * * \a File \a Type : type of spectrum files
 * * \a Peak \a Finding: find peaks 
 * * \a Peak \a Fitting: fit peak 
 * * \a Background \a Subtraction: subtract background
 * * \a Efficiency \a Fitting: fit efficiency curve 
 * * \a Make \a Plots: make plots 
 *
 * Options for finding peaks:
 * * \a Sigma : sigma of peak finding method
 * * \a Threshold : threshold of peak finding method
 * 
 * Options for fitting peaks:
 * * \a Minimum \a Range : minimum value of range
 * * \a Maximum \a Range : maximum value of range
 * * \a Fitting \a Formula : fitting formula 
 * * \a Number \a of \a Peaks : number of fitting peaks
 *
 * Options of background estimation:
 * * \a Number \a of \a Iteration : number of iterations
 * * \a Background \a Estimation : background estimation method
 *
 * Options for efficiency fitting:
 * * \a Energy : energy values
 * * \a Efficiency : efficiency values corresponding to energies
 *
 */
void processSpectrum(Config *config)
{
	/********* Read spectra *********/
	std::vector<TString> filename = config->getString("File Name", ',');
	TString filetype              = config->get      ("File Type", "TKA");
	std::vector<Spectrum> spec;
	for(int i = 0; i < (int)filename.size(); ++i) {
		Spectrum s;
		s.read(filename[i],filetype);
		spec.push_back(s);
	}

	/******** Analyze spectra *******/
	bool doFindPeak      = config->get("Peak Finding"          , false);
	bool doFitPeak       = config->get("Peak Fitting"          , false);
	bool doBkgSubtration = config->get("Background Subtraction", false);
	bool doFitEfficiency = config->get("Efficiency Fitting"    , false);
	bool doMakePlot      = config->get("Make Plots"            , false);

	for(int i = 0; i < (int)spec.size(); ++i) {
		if(doFindPeak) {
			MESSAGE("Finding peaks in spectrum'"+filename[i]+"'");
			double sigma     = config->get("Sigma"    , 2.);
			double threshold = config->get("Threshold", 0.2);
			std::vector<double> peak_pos = spec[i].findPeaks(sigma,threshold);
			std::vector<GenericData> g_peak_pos(peak_pos.begin(),peak_pos.end());
			Table table;
			table.appendRow(g_peak_pos);
			std::vector<GenericData> s;
			s.push_back("Peak position");
			table.prependRow(s);
			table.print(15);
		}
		if(doFitPeak) {
			MESSAGE("Fitting peaks in spectrum'"+filename[i]+"'");
			double min_range = config->get("Minimum Range"  , 0.);
			double max_range = config->get("Maximum Range"  , 0.);
			TString formula  = config->get("Fitting Formula", "gaus+expo");
			int Npeaks       = config->get("Number of Peaks", 1);
			if(min_range == 0 && max_range == 0) {
				ERROR("No fitting range specified.");
				continue;
			}
			if(Npeaks < 1) {
				ERROR("Invalid number of peaks.");
				continue;
			}
			if(Npeaks == 1) {
				TF1* f = spec[i].fitPeak(min_range,max_range,formula);
			} else {
				std::vector<TF1*> f = spec[i].fitMultiPeak(Npeaks,min_range,max_range);
			}
		}
		if(doBkgSubtration) {
			MESSAGE("Background subtraction in spectrum'"+filename[i]+"'");
			int Niter      = config->get("Number of Iteration"  , 50);
			TString option = config->get("Background Estimation", "BackIncreasingWindow");
			TH1* bkg = spec[i].estimateBkg(Niter,option);
			spec[i].subtractBkg(bkg);
		}
		
		if(doMakePlot) {
			MESSAGE("Make plots with spectrum'"+filename[i]+"'");
		}
	}
	if(doFitEfficiency) {
		MESSAGE("Fitting efficiency curve");
		std::vector<double> energy     = config->getDouble("Energy"    , ',');
		std::vector<double> efficiency = config->getDouble("Efficiency", ',');
		std::vector<double> effCoef = spec[0].getEffCoeff(energy,efficiency);
	}
}

/***************************************************************************/
/**
 * This is the function for analyse gamma spectrum with FSA (\a Full \a Spectrum 
 * \a Analysis) method, with configuration options:
 * * \a Measurement \a Spectra : name of measured spectra
 * * \a Calibration \a Spectra : name of calibration spectra
 * * \a File \a Type           : spectrum type name 
 * * \a Fitting \a Method      : fitting method 
 * * \a Use \a Weight          : use weights in object functions 
 *
 */
void processFSA(Config *config)
{
	/********* Read spectra *********/
	std::vector<TString> meas_file  = config->getString("Measurement Spectra", ',');
	std::vector<TString> calib_file = config->getString("Calibration Spectra", ',');
	TString filetype                = config->get      ("File Type"          , "TKA");
	TString method                  = config->get      ("Fitting Method"    , "");
	bool useWeight                  = config->get      ("Use Weight"         , true);

	std::vector<Spectrum> meas_spec;
	for(int i = 0; i < (int)meas_file.size(); ++i) {
		Spectrum s;
		s.read(meas_file[i],filetype);
		meas_spec.push_back(s);
	}
	std::vector<Spectrum> calib_spec;
	for(int i = 0; i < (int)calib_file.size(); ++i) {
		Spectrum s;
		s.read(calib_file[i],filetype);
		calib_spec.push_back(s);
	}
	BaseFitter *fsa = new GAFitter();
	fsa->setCalibSpectra(calib_spec);
	for(int i = 0; i < (int)meas_spec.size(); ++i) {
		fsa->fit(meas_spec[i],useWeight);
	}

	/********** Make plots **********/

	Plotter plotter;
	plotter.setStyle(config);
}

/***************************************************************************/
/**
 * This is the function for printing SPECTRUM ANALYSIS module information.
 */
void info()
{
	std::cout << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << "   * Welcome to SPECTRUM ANALYSIS package v0.1                  *" << std::endl;
	std::cout << "   * Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>       *" << std::endl;
	std::cout << "   * Date   : 25-12-2015                                        *" << std::endl;
	std::cout << "   * Url    : https://sites.google.com/site/nmtpgroup/home/demo *" << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << std::endl;
}

