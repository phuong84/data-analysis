/**
 * \defgroup MCNPAnalysis MCNP Analysis
 *
 * \brief    Analysis tools for MCNP output
 *
 * This module contains classes for analysing MCNP output files.
 * MCNP is a general-purpose Monte Carlo N-Particle code for 
 * simulating particle (photon, electron, neutron,...) transport. 
 * More details can be seen at https://mcnp.lanl.gov/
 * 
 * \file     MCNPAnalysis.cxx
 * \ingroup  MCNPAnalysis
 *
 * \brief    Contain main function for MCNP analysis module
 *
 * This file has the main() function for MCNP analysis module.
 * Different procedures for analysing MCNP outputs are implemented
 * in this file.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
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
#include "TallyReader.h"
#include "MeshTallyReader.h"
#include "PtracParser.h"
#include "PtracSelector.h"
#include "HistoUtilities.h"

void info();
void processTally(Config *config);
void processMesh (Config *config);
void processPtrac(Config *config);
void processHisto(Config *config);
void processTallyComparison(Config *config);

ErrHandler message("MCNPAnalysis");
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

	if      (type == "TALLY") {
		MESSAGE("Analysis mode TALLY");
		processTally(config);
	}
	else if (type == "TALLY COMPARISON") {
		MESSAGE("Analysis mode TALLY COMPARISON");
		processTallyComparison(config);
	}
	else if (type == "MESHTALLY") {
		MESSAGE("Analysis mode MESHTALLY");
		processMesh(config);
	}
	else if (type == "PTRAC") {
		MESSAGE("Analysis mode PTRAC");
		processPtrac(config);
	}
	else if (type == "HISTO") {
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
 * This is the function for reading MCNP tally output, with configuration 
 * options:
 * * \a File \a Name : name of MCNP output file
 * * \a Make \a Plot : create plots for tally output (true or false)
 * * \a Tally \a Number: output tally numbers for reading 
 *
 * After reading output, a histogram root file will be created with the same 
 * name with the output file.
 */
void processTally(Config* config)
{
	TString filename               = config->get("File Name"          , "");
	bool makeplot                  = config->get("Make Plot"          , false);
	std::vector<TString> tallylist = config->getString("Tally Number" , ',');

	DEBUG( TString::Format( "Number of tallies = %d", (int)tallylist.size() ) );

	TallyReader tally;
	MESSAGE("Read tally files...");
	tally.read(filename);
	tally.extractHisto(filename+".root");

	if(makeplot) {
		MESSAGE("Make tally plots...");
		TFile* file = new TFile(filename+".root");
		Plotter plotter;
		plotter.setStyle(config);
		if(tallylist.size() > 0) {
			std::vector<TH1*> histlist;
			for (size_t i = 0; i < tallylist.size(); ++i) {
				TH1* hist = (TH1*)file->Get("Tally"+tallylist[i]);
				histlist.push_back(hist);
			}
			plotter.makeHistPlots(histlist);
		} else {
			WARN("No tally number specified. Plotting all!");
			plotter.makeFilePlot(file);
		} 
		file->Close();
	}
}

/***************************************************************************/
/**
 * This is the function for reading different tally results in different MCNP
 * output files and compare them with each other. Configuration options:
 * * \a File \a Name : name of MCNP output files (separate by ',')
 * * \a Tally \a Number: output tally numbers for reading (separate by ',')
 *
 * After reading outputs, histogram root files will be created with the same 
 * names with the output files.
 */
void processTallyComparison(Config* config)
{
	std::vector<TString> filelist  = config->getString("File Name"    , ',');
	std::vector<TString> tallylist = config->getString("Tally Number" , ',');
	std::vector<TString> legendlist = config->getString("Legend Title" , ',');

	DEBUG( TString::Format( "Number of files = %d", (int)filelist.size() ) );
	DEBUG( TString::Format( "Number of tallies = %d", (int)tallylist.size() ) );

	size_t size = ( filelist.size() < tallylist.size() ? filelist.size() : tallylist.size() );
	if(size <= 1) {
		ERROR("Not have enough number of tallies to make comparison!");
		return;
	}
	if( size != filelist.size() || size != tallylist.size() ) 
		WARN("Numbers of files and tallies are different!");
	if( legendlist.size() == 0 ) 
		WARN("No legend title was set.");

	MESSAGE("Read tally files...");
	for (int i = 0; i < (int)size; ++i) {
		TallyReader tally;
		tally.read(filelist[i]);
		tally.extractHisto(filelist[i]+".root");
	}

	std::vector<TH1*> histlist;
	for (int i = 0; i < (int)size; ++i) {
		TFile *file = new TFile(filelist[i]+".root");
		TH1* hist = (TH1*)file->Get("Tally"+tallylist[i]);
		hist->SetDirectory(0);
		if( i < (int)legendlist.size() )
			hist->SetTitle(legendlist[i]);
		else
			WARN( TString::Format( "Not setting legend title for %s", hist->GetTitle() ) );
		histlist.push_back(hist);
		file->Close();
	}
	Plotter plotter;
	plotter.setStyle(config);
	MESSAGE("Make comparison plots...");
	plotter.makeComparisonPlot(histlist);	
}

/***************************************************************************/
/**
 * This is the function for reading different tally mesh results, merging 
 * and plotting them. Configuration options:
 * * \a File \a Name : name of MCNP mesh tally outputs (separate by ',')
 * * \a Outputfile \a Name : name of histogram output file
 * * \a Merging: do merge mesh tallies (true or false)
 * * \a Make \a Ratio : create projection ratio plots between meshes
 * * \a Make \a Plot : create plots for tally outputs (true or false)
 * * \a Projection \a Plane : name of plane to make 2D projection plots on (e.g. "XY")
 * * \a Projection \a Axis : name of axis to make 1D projection plots on (e.g. "X")
 * * \a First \a Bin: the first bin to be included in projection
 * * \a Last \a Bin: the last bin to be included in projection
 */
void processMesh(Config* config)
{
	std::vector<TString> filelist = config->getString("File Name"       , ',');
	TString outfilename           = config->get      ("Outputfile Name" , "mesh_tally");
	bool doMerging                = config->get      ("Merging"         , false);
	bool doRatio                  = config->get      ("Make Ratio"      , false);
	bool makeplot                 = config->get      ("Make Plot"       , false);
	TString plane                 = config->get      ("Projection Plane", "");
	TString axis                  = config->get      ("Projection Axis" , "");
	std::vector<int> firstbinList = config->getInt   ("First Bin"       , ',');
	std::vector<int> lastbinList  = config->getInt   ("Last Bin"        , ',');

	// read meshtally files and write out histograms to output file
	MESSAGE("Read meshtally files...");
	int size = (int) filelist.size();
	for(int i = 0; i < size; ++i) {
		MeshTallyReader mesh;
		mesh.read(filelist[i]);
		if(i==0)
			mesh.extractHisto(outfilename+".root", false);
		else
			mesh.extractHisto(outfilename+".root", true);
	}

	// merge 3D meshtally histograms
	if(doMerging) {
		MESSAGE("Merge meshtally histograms...");
		HistoUtilities hutil;
		TFile *file = new TFile(outfilename+".root","update");
		hutil.mergeHistos(file);
		file->Close();
	}
	
	// make ratio 3D meshtally histograms
	if(doRatio) {
		MESSAGE("Make ratio meshtally histograms...");
		HistoUtilities hutil;
		TFile *infile = new TFile(outfilename+".root","read");
		std::vector<TH1*> histlist;
		hutil.getHistosFromFile(histlist,filelist,infile);
		for(int i = 1; i < (int)histlist.size(); ++i) {
			histlist[i]->Divide(histlist[i],histlist[0]);
			histlist[i]->SetName( TString::Format( "ratio_%s_%s", histlist[i]->GetName(), histlist[0]->GetName() ) );
		}
		TFile* outfile = new TFile("ratio_"+outfilename+".root","recreate");
		hutil.writeHistos(histlist,outfile);
		infile->Close();
		outfile->Close();
	}

	// make meshtally plots
	if(makeplot) {
		if(lastbinList.size() < firstbinList.size()) {
			ERROR("Missing last bin numbers!");
			return;
		}

		TFile* file = 0;
		if (doRatio)
			file = new TFile("ratio_"+outfilename+".root","draw");
		else
			file = new TFile(outfilename+".root","draw");
		
		HistoUtilities hutil;
		std::vector<TH1*> histolist;
		hutil.getHistosFromFile(histolist,file);
		
		if(histolist.size() == 0) {
			ERROR("Couldn't get any histogam from file '"+outfilename+".root'!");
			return;
		}
		Plotter plotter;
		plotter.setStyle(config);

		// make 2D projection plots
		MESSAGE("Make 2D projection plots...");
		if(plane != "") {
			for(size_t i = 0; i < firstbinList.size(); ++i) {
				double weight = 1.;
				if(doRatio) weight = 1./(lastbinList[i]-firstbinList[i]+1);
				if(doMerging)
					plotter.makeProj2DPlot( (TH3F*)histolist[histolist.size()-1], plane, firstbinList[i], lastbinList[i], weight );
				else
					for(size_t j = 0; j < histolist.size(); ++j)
						plotter.makeProj2DPlot( (TH3F*)histolist[j], plane, firstbinList[i], lastbinList[i], weight );
			}
		}
		
		/*
		// make 1D projection plots
		if(axis != "") {

		}
		*/
		file->Close();
	}
}

/***************************************************************************/
/**
 * This is the function for processing PTRAC file.
 */
void processPtrac(Config* config)
{
	TString filename    = config->get("File Name" , "");

	PtracParser ptrac;	
	ptrac.extract(filename,4);
	//ptrac.filter(filename+".root", "fil_"+filename+".root", "NPS == 1");

	// Lua chon cac event theo dieu kien dat ra
/*	PtracSelector selector(filename+".root");
	TFile *outfile = new TFile("sel_"+filename+".root","RECREATE");
	TH1::AddDirectory(0);
	selector.Loop(outfile);
	TH1::AddDirectory(1);
	outfile->Close();
*/
}

/***************************************************************************/
/**
 * This is the function for processing hitograms.
 */
void processHisto(Config* config)
{
	TString filename               = config->get      ("File Name" , "");
	std::vector<TString> histoname = config->getString("Histogram" , ',');

	TFile* file = new TFile(filename,"read");
	std::vector<TH1*> histolist;
	HistoUtilities hutil;
	hutil.getHistosFromFile(histolist,histoname,file);
	if(histolist.size() == 0) {
		ERROR("Couldn't get any histogam from file '"+filename+".root'!");
		return;
	}
	file->Close();
	Plotter plotter;
	plotter.setStyle(config);
	plotter.makeHistPlots(histolist);
	return;
}


/***************************************************************************/
/**
 * This is the function for printing MCNP ANALYSIS module information.
 */
void info()
{
	std::cout << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << "   * Welcome to MCNP ANALYSIS package v0.1                      *" << std::endl;
	std::cout << "   * Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>       *" << std::endl;
	std::cout << "   * Date   : 30-03-2015                                        *" << std::endl;
	std::cout << "   * Url    : https://sites.google.com/site/nmtpgroup/home/demo *" << std::endl;
	std::cout << "   **************************************************************" << std::endl;
	std::cout << std::endl;
}


