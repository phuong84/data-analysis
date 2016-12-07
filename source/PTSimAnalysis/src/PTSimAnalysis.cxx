/**
 * \defgroup PTSimAnalysis PTSim Analysis
 *
 * \brief    Analysis tools for PTSim output
 *
 * This module contains classes for analysing PTSim output files.
 * PTSim is a Geant4-based simulation framework for radiotherapy
 * of cancer treatment with a special focus on proton and carbon 
 * therapy.
 * 
 * \file     PTSimAnalysis.cxx
 * \ingroup  PTSimAnalysis
 *
 * \brief    Contain main function for PTSim analysis module
 *
 * This file has the main() function for PTSim analysis module.
 * Different procedures for analysing PTSim outputs are implemented
 * in this file.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-11-2016
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
#include "HistoUtilities.h"

void info();
void processHisto(Config *config);

ErrHandler message("PTSimAnalysis");
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
	for (Int_t i = 0; i < argc; i++)
		params.push_back(TString(argv[i]));
	TString configFile = params[1];

	Config* config = new Config();
	config->open(configFile);

	/***** Choose analsyis mode ****/
	TString type = config->get("ANALYSIS MODE", "");
	debug_level  = config->get("DEBUG LEVEL"  , 0);

	if (type == "HISTO") {
		MESSAGE("Analysis mode HISTO");
		processHisto(config);
	}
	else {
		ERROR("Undefined analysis mode!");
		WARN("May due to the inconsistency between Windows and Linux/Cygwin text file formats.");
		WARN("If you're using Cygwin, please try to avoid using Notepad/Wordpad for editing config files,");
		WARN("and use Emacs, Vim, Notepad++,... instead.");
	}

	/************* END *************/	
	std::cout << std::endl;
	delete config;
	return 0;
}


/***************************************************************************/
/**
 * This is the function for processing hitograms.
 */
void processHisto(Config* config)
{
	std::vector<TString> filename  = config->getString("File Name"       , ',');
	std::vector<TString> histoname = config->getString("Histogram"       , ',');
	TString type                   = config->get      ("Projection Type" , "");
	std::vector<int> rangeX        = config->getInt   ("X Range"         , ',');
	std::vector<int> rangeY        = config->getInt   ("Y Range"         , ',');
	std::vector<int> rangeZ        = config->getInt   ("Z Range"         , ',');
	std::vector<double> valueX     = config->getDouble("X Value"         , ',');
	std::vector<double> valueY     = config->getDouble("Y Value"         , ',');
	std::vector<double> valueZ     = config->getDouble("Z Value"         , ',');
	bool doComparision             = config->get      ("Make Comparison" , false);
	bool doHist2Txt                = config->get      ("Export To Text"  , false);
	std::vector<TString> title     = config->getString("Title"           , ',');
	TString titleX                 = config->get      ("Title X"         , "");
	TString titleY                 = config->get      ("Title Y"         , "");

	std::vector<TH1*> comp_histo;
	HistoUtilities hutil;
	Plotter plotter;
	plotter.setStyle(config);
	
	for(size_t i = 0; i < filename.size(); ++i) {
		INFO("Reading file '"+filename[i]+"'");
		TFile* file = new TFile(filename[i],"read");
		std::vector<TH1*> histolist;
		hutil.getHistosFromFile(histolist,histoname,file);
		if(histolist.size() == 0) {
			ERROR("Couldn't get any histogam from file '"+filename[i]+"'!");
			continue;
		}
		if(type != "") {
			MESSAGE("Making projection plots...");
			for(size_t j = 0; j < histolist.size(); ++j) {
				TH1* h = hutil.makeProjection( (TH3F*)histolist[j], type, rangeX, rangeY, rangeZ, valueX, valueY, valueZ );
				TString prefix = filename[i];
				prefix.ReplaceAll("/","_");
				prefix.ReplaceAll(".root","");
				DEBUG("Prefix: "+prefix);
				plotter.makeHistPlot(h,prefix);
				h->SetDirectory(0);
				comp_histo.push_back(h);
				if(doHist2Txt)
					plotter.exportHist2Text(h,prefix);
			}
		}
		file->Close();
	}
	if(type != "" && doComparision) {
		MESSAGE("Making comparison plots...");
		plotter.makeComparisonPlot(comp_histo, title);
	}
	
	return;
}


/***************************************************************************/
/**
 * This is the function for printing PTSIM ANALYSIS module information.
 */
void info()
{
	std::cout << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << "   * Welcome to PTSIM ANALYSIS package v0.1                     *" << std::endl;
	std::cout << "   * Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>       *" << std::endl;
	std::cout << "   * Date   : 30-11-2016                                        *" << std::endl;
	std::cout << "   * Url    : https://sites.google.com/site/nmtpgroup/home/demo *" << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << std::endl;
}


