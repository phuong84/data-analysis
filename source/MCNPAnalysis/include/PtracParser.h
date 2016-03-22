/**
 * \class    PtracParser
 * \ingroup  MCNPAnalysis
 *
 * \brief    Parse event from PTRAC file
 *
 * This class can be used to parse MCNP simulation events from
 * PTRAC file. It is remodified from ParsNIP (Parser for PTRAC files 
 * produced by MCNP) project http://ptracparser.sourceforge.net/ .
 * The PTRAC event will be store in ROOT Tree format with name 
 * \a PTRAC_Tree.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     PtracParser.h
 * 
 */

//////////////////////////////////////////////////////////
// This class is a modification of ParsNIP project
// Copyright (c) 2014, Tanya Hutton
// http://ptracparser.sourceforge.net/
//////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2F.h>
#include <TH3F.h>
#include "ErrHandler.h"
#include "StringParser.h"
#include "PtracEvent.h"

#ifndef __PtracParser__
#define __PtracParser__

#define TITLE 0
#define INPHD 1
#define NVARS 2
#define VARID 3
#define NPSLN 4
#define EINFO 5
#define EVENT 6

#define SRC  1000
#define BNK  2000
#define SUR  3000
#define COL  4000
#define TER  5000
#define END  9000

class PtracParser {

public:

	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	PtracParser() : message("PtracParser") {};
	
	/// \brief Class destructor
	~PtracParser() {};
	
	/// \brief Extract PTRAC events to root file
	/// \param filename name of root file
	/// \param n_iplines starting line
	void extract(TString filename, int n_iplines);
	
	/// \brief Filter PTRAC events
	/// \param infilename input root file
	/// \param outfilename output root file
	/// \param cuts filter cuts
	void filter(TString infilename, TString outfilename, TString cuts);

private:

	/// \brief Initialize tree
	/// \param tree TTree pointer
	void initTree(TTree* tree);
	
	/// \brief Parse PTRAC file
	/// \param type type of information
	/// \param line line string
	void process(int type, std::string line);
	
	/// \brief Parse info line
	/// \param str input string
	void parseINPHD(std::string str);
	
	/// \brief Parse number of variables
	/// \param str input string
	void parseNVARS(std::string str);
	
	/// \brief Parse variable IDs
	/// \param str input string
	void parseVARID(std::string str);
	
	/// \brief Parse NPS line
	/// \param str input string
	void parseNPSLN(std::string str);
	
	/// \brief Parse event info line
	/// \param str input string
	void parseEINFO(std::string str);
	
	/// \brief Parse event info
	/// \param str input string
	void parseEVENT(std::string str);
	//std::string lookup(int type, int val, int ipt);
	
	PtracEvent event;    ///< PTRAC event
	ErrHandler message;  ///< Label of class to print out with message
};

	static const std::string event_name[5]={ "SRC", "BNK", "SUR", "COL", "TER" };
	static const std::string idx_keyword[14] = { "Buffer", "Cell", "Event", "File", "Filter", "Max", "MEPH", "NPS", "Surface", "Tally", "Type", "Value", "Write" };
	static const std::string bank_desc[26] = {"DXTRAN Track ", "Energy Split", "Weight Window Surface Split", "Weight Window Collision Split", "Forced Collision-Uncollided Part", "Importance Split", "Neutron from Neutron (n,xn) (n,f)", "Photon from Neutron", "Photon from Double Fluorescence", "Photon from Annihilation", "Electron from Photoelectric", "Electron from Compton", "Electron from Pair Production", "Auger Electron from Photon/X-ray", "Positron from Pair Production", "Bremsstrahlung from Electron", "Knock-on Electron", "X-rays from Electron", "Photon from Neutron - Multigroup", "Neutron (n,f) - Multigroup", "Neutron (n,xn) k- Multigroup", "Photon from Photon - Multigroup", "Adjoint Weight Split - Multigroup", "Weight window time split", "Neutron from photonuclear", "DXTRAN annnihilation photon from pulse height tally variance reduction" };
	static const std::string ter_type[20] = { "Escape", "Energy cut-off", "Time cut-off", "Weight window", "Cell importance", "Weight cut-off", "Energy importance", "DXTRAN", "Forced collision", "Exponential transform", "Downscattering", "Capture", "Loss to (n,xs)", "Loss to fission", "Compton scatter", "Capture", "Pair production", "Scattering ", "Bremsstrahlung" };
	static const std::string varid_mcnp[28] = { "NPS", "S_EVENT", "NCL", "NSF", "JPTAL", "TAL", "NXT_EVENT", "NODE", "NSR", "NXS", "NYTN", "NSF", "ANG", "NTER", "BRANCH", "IPT", "NCL", "MAT", "NCP", "XXX", "YYY", "ZZZ", "UUU", "VVV", "WWW", "ERG", "WGT", "TME" };
	static const std::string varid_name[28] = { "History number", "Type of first history event", "Cell number", "Nearest surface headed towards", "Tally specifier", "TFC specifier", "Next event type", "Number of nodes in track from source to this point", "Source number", "ZZAAA for interaction", "Reaction type (MT)", "Surface number", "Angle with surface normal (degrees)", "Termination type", "Branch number", "Particle type", "Cell number", "Material number", "Number of collisions in history", "x-coordinate of event (cm)", "y-coordinate of event (cm)", "z-coordinate of event (cm)", "x-component of exit direction vector", "y-component of exit direction vector", "z-component of exit direction vector", "Energy of particle after event", "Weight of particle after event", "Time of event" };
	static const int mt_lookup[118]={ 1, 2, 3, 4, 5, 10, 11, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 38, 41, 42, 44, 45, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 101, 102, 103, 104, 105, 106, 107, 108, 109, 111, 112, 113, 114, 115, 116, 117, 151, 451, 452, 455, 456, 458, 460, 600, 601, 602, 648, 649, 650, 651, 698, 699, 700, 701, 748, 749, 750, 751, 798, 799, 800, 801, 848, 849 };
	static const std::string mt_name[118]={ "n_total", "z_elastic", "z_nonelastic", "z_n", "z_anything", "z_continuum", "z_2nd", "z_2n", "z_2n", "z_fission", "z_f", "z_nf", "z_2nf", "z_na", "z_n3a", "z_2na", "z_3na", "z_abs", "z_np", "z_n2a", "z_2n2a", "z_nd", "z_nt", "z_n3He", "z_nd2a", "z_nt2a", "z_4n", "z_3nf", "z_2np", "z_3np", "z_n2p", "z_npa", "z_n0", "z_n1", "z_n2", "z_n3", "z_n4", "z_n5", "z_n6", "z_n7", "z_n8", "z_n9", "z_n10", "z_n11", "z_n12", "z_n13", "z_n14", "z_n15", "z_n16", "z_n17", "z_n18", "z_n19", "z_n20", "z_n21", "z_n22", "z_n23", "z_n24", "z_n25", "z_n26", "z_n27", "z_n28", "z_n29", "z_n30", "z_n31", "z_n32", "z_n33", "z_n34", "z_n35", "z_n36", "z_n37", "z_n38", "z_n39", "z_n40", "z_nc", "z_disap", "z_gamma", "z_p", "z_d", "z_t", "z_3He", "z_a", "z_2a", "z_3a", "z_2p", "z_pa", "z_t2a", "z_d2a", "z_pd", "z_pt", "z_da", "resonance_params", "description", "fission_n", "fission_n_delayed", "fission_n_prompt", "fission_n_energy", "g_delayed", "z_p0", "z_p1", "z_p2", "z_p48", "z_pc", "z_d0", "z_d1", "z_d48", "z_dc", "z_t0", "z_t1", "z_t48", "z_tc", "z_3He0", "z_3He1", "z_3He48", "z_3Hec", "z_a0", "z_a1", "z_a48", "z_ac" };
	static const std::string photon_ntyn[5] = { "Incoherent scatter", "Coherent scatter", "Fluorescence", "Double fluorescence", "Pair production" };

#endif
