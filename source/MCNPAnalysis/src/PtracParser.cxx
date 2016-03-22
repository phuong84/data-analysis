/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     PtracParser.cxx
 * 
 */

#include "PtracParser.h"

/***************************************************************************/
/**
 * This method loops all PTRAC file line, decide which type of information can
 * be read and call the corresponding method. After reading information, event 
 * object information will be written out to the output ROOT tree.
 */
void PtracParser::extract(TString filename, int n_iplines)
{
	// Open ptrac file for input
	std::ifstream infile;
	infile.open(filename.Data(), std::ifstream::in);
	if (infile.fail()) {
		ERROR("Cannot open file '"+filename+"'");
	}

	// Open root file for output
	TFile outfile(filename+".root","RECREATE");

	// Create histogram for number of histories
	TH1F *h_hist = new TH1F("NumberOfHistory", "NumberOfHistory", 1, 0, 1);
	
	// Start to extract information line by line
	event.initialize();
	TTree* tree = new TTree("PTRAC_Tree", "PTRAC_Tree");
	initTree(tree);

	int id_line    = n_iplines + 3;
	int nps_line   = id_line + 4;
	int einfo_line = nps_line + 1;
	int event_line = einfo_line + 1;

	int lctr     = 0;
	int nstep    = 3;
	int eistep   = 3;
	int nxt_type = 0;

	event.event_ctr = 0;
	event.hist_ctr  = 0;
	event.nps_ctr   = 0;

	std::string line;
	while (std::getline(infile, line)) {
		// first 3 lines title/other info -> ignore for now
		if (lctr < 3);
		// for lines 3 -> id_line, process filter info
		else if (lctr >= 3 && lctr < id_line) 
			process(INPHD, line);
		// for id_line, process number of variables for each event line
		else if (lctr == id_line) 
			process(NVARS, line);
		// for lines id_line+1 -> nps_line process variable ids for events
		else if (lctr > id_line && lctr < nps_line) 
			process(VARID, line);
		// for line nps_line process, increase nps_line to next expected nps_line
		if (lctr == nps_line) {                                                   
			if (!event.event_ctr)
				INFO("Parsing PTRAC events...");
			process(NPSLN, line);  
			event.nps_ctr = 0;
			nps_line += nstep;
		}
		// for line = einfo_line, process accordingly and set next nps_line... 
		// If returned value from process() is 9000 add to history counter and reset step values
		else if (lctr == einfo_line) {
			if (event.nps_ctr)
				event.type = nxt_type;     
			else 
				event.type = event.s_event;
			process(EINFO, line);
			if (event.nxt_event == END) {
				eistep = 3;
				++event.hist_ctr;  
				h_hist->Fill(0);                   
			} else {
				eistep = 2;
				nps_line += 2;
				nxt_type = event.nxt_event;
			}
			einfo_line += eistep;              
		} 
		else if (lctr == event_line) {  
			process(EVENT, line);
			++event.nps_ctr;
			++event.event_ctr;
			event_line = einfo_line + 1;     
			tree->Fill();        // write out event
			event.initialize();  // initialize new event
		}   
		++lctr;
	}
	tree->Print();

	// End of extraction
	INFO("Writing out events to '"+filename+".root'");
	outfile.Write();
	outfile.Close();
	infile.close();	
}

/***************************************************************************/
/**
 * This method reads PTRAC line info based on the specific type.
 */
void PtracParser::process(int type, std::string line)
{
	//int nps, s_event=0;
	switch(type){
		case INPHD:
			parseINPHD(line);
			break;
		case NVARS:
			parseNVARS(line);
			break;
		case VARID:
			parseVARID(line);
			break;
		case NPSLN:
			parseNPSLN(line);
			break;
		case EINFO:
			parseEINFO(line);
			break;
		case EVENT:
			parseEVENT(line);
			break;
		default:
			break;
	}
}

/***************************************************************************/

void PtracParser::parseINPHD(std::string str)
{

}

/***************************************************************************/

void PtracParser::parseNVARS(std::string str)
{

}

/***************************************************************************/

void PtracParser::parseVARID(std::string str)
{

}

/***************************************************************************/
/**
 * This method reads NPS and event type number.
 */
void PtracParser::parseNPSLN(std::string str)
{
	if (str.size() < 3) {
		ERROR("Missing NPSLN information.");
		return;
	}
	StringParser parser;
	std::vector<int> var = parser.getInt(str);
	event.nps     = var[0];
	event.s_event = var[1];
}

/***************************************************************************/
/**
 * This method reads PTRAC event information.
 */
void PtracParser::parseEINFO(std::string str)
{
	if (str.size() < 7) {
		ERROR("Missing EINFO information.");
		return;
	}
	StringParser parser;
	std::vector<int> var = parser.getInt(str);
	int type = event.type;
	if(fabs(type) >= BNK && fabs(type) < SUR){
		type = BNK;
	}

	event.nxt_event = var[0];
	event.node      = var[1];
	if(type == SRC) {
		event.nsr = var[2];
		event.ncl = var[3];
		event.ncl = var[4];
		event.mat = var[5];
	} else if (type == COL || type == BNK) {
		event.nxs  = var[2];
		event.ntyn = var[3];
		event.ncl  = var[4];
		event.mat  = var[5];
	} else if(type == SUR) {
		event.nsf = var[2];
		event.ang = var[3];
		event.mat = var[4];
		event.ncp = var[5];
	} else if(type == TER) {
		event.nter   = var[2];
		event.branch = var[3];
		event.ncl    = var[4];
		event.mat    = var[5];
	}
	event.ncp = var[6];
}

/***************************************************************************/
/**
 * This method reads PTRAC event information (position and momentum vectors)
 */
void PtracParser::parseEVENT(std::string str)
{
	if (str.size() < 9) {
		ERROR("Missing EVENT information.");
		return;
	}
	StringParser parser;
	std::vector<double> var = parser.getDouble(str);
	event.xxx = var[0];
	event.yyy = var[1];
	event.zzz = var[2];
	event.uuu = var[3];
	event.vvv = var[4];
	event.www = var[5];
	event.erg = var[6];
	event.wgt = var[7];
	event.tme = var[8];
}

/***************************************************************************/
/**
 * This method filters PTRAC events by applying cuts to the PTRAC tree. The 
 * new PTRAC tree will be written out to another ROOT file. 
 */
void PtracParser::filter(TString infilename, TString outfilename, TString cuts)
{
	TFile* infile = TFile::Open(infilename);
	if (!infile) {
		ERROR("Cannot open file '"+infilename+"'");
	}
	TTree* inTree = (TTree*)infile->Get("PTRAC_Tree");
	TFile* outfile = TFile::Open(outfilename,"RECREATE");
	TTree* outTree = inTree->CopyTree(cuts);
	outfile->cd();
	INFO("Writing out filtered events to '"+outfilename+".root'");
	outTree->Write();
	outfile->Close();
	infile->Close();
}

/***************************************************************************/
/**
 * This method initializes PTRAC tree, and sets event branchs for the tree. 
 */
void PtracParser::initTree(TTree* tree)
{
	//tree->Branch("EventCounter", &(event.event_ctr), "EventCounter/I");
	tree->Branch("NPS"              , &(event.nps)      , "NPS/I");
	tree->Branch("InitialEvent"     , &(event.s_event)  , "InitialEvent/I");
	tree->Branch("NextEvent"        , &(event.nxt_event), "NextEvent/I");
	tree->Branch("Node"             , &(event.node)     , "Node/I");
	tree->Branch("SourceType"       , &(event.nsr)      , "SourceType/I");
	tree->Branch("ZZAAA"            , &(event.nxs)      , "ZZAAA/I");
	tree->Branch("ReactionType"     , &(event.ntyn)     , "ReactionType/I");
	tree->Branch("ClosestSurface"   , &(event.nsf)      , "ClosestSurface/I");
	tree->Branch("AngleToSurface"   , &(event.ang)      , "AngleToSurface/I");
	tree->Branch("TerminationType"  , &(event.nter)     , "TerminationType/I");
	tree->Branch("BranchNumber"     , &(event.branch)   , "BranchNumber/I");
	tree->Branch("ParticleType"     , &(event.ipt)      , "ParticleType/I");
	tree->Branch("CellNumber"       , &(event.ncl)      , "CellNumber/I");
	tree->Branch("MaterialNumber"   , &(event.mat)      , "MaterialNumber/I");
	tree->Branch("Type"             , &(event.type)     , "Type/I");
	tree->Branch("NumberOfCollision", &(event.ncp)      , "NumberOfCollision/I");
	tree->Branch("X"                , &(event.xxx)      , "X/D");
	tree->Branch("Y"                , &(event.yyy)      , "Y/D");
	tree->Branch("Z"                , &(event.zzz)      , "Z/D");
	tree->Branch("U"                , &(event.uuu)      , "U/D");
	tree->Branch("V"                , &(event.vvv)      , "V/D");
	tree->Branch("W"                , &(event.www)      , "W/D");
	tree->Branch("Energy"           , &(event.erg)      , "Energy/D");
	tree->Branch("Weight"           , &(event.wgt)      , "Weight/D");
	tree->Branch("Time"             , &(event.tme)      , "Time/D");
}

