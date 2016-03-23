/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     MeshTallyReader.cxx
 * 
 */

#include "MeshTallyReader.h"

/***************************************************************************/
/**
 * This is construtor of MeshTallyReader class, it initializes axis boundary 
 * values and \ref m_histoname, \ref message members.
 */
MeshTallyReader::MeshTallyReader() : m_histoname("meshtal"),message("MeshTallyReader") 
{
	xbin = 1; xlow = 0.; xhigh = 1.;
	ybin = 1; ylow = 0.; yhigh = 1.;
	zbin = 1; zlow = 0.; zhigh = 1.;
	plane = NONE;
}

/***************************************************************************/
/**
 * This method reads each line in MCNP mesh output file and decides which
 * information type can be extracted from that line, then it calls \ref 
 * process() method to read information.
 */
void MeshTallyReader::read(TString filename)
{
	m_histoname = filename;

	// Open meshtal file for reading
	std::ifstream infile;
	infile.open(filename.Data(), std::ifstream::in);
	if(infile.fail()) {
		ERROR("Cannot open file '"+filename+"'");
	} else
		INFO("Open file '"+filename+"'");

	// Start to extract information line by line
	Tag tag = INFO;
	int n_line = 0;
	std::string line;
	while( std::getline(infile, line) ) {
		++n_line;
		if(line.find(" Tally bin boundaries:") != std::string::npos) {
			tag = BOUND;
			continue;
		}
		if(line.find("Tally Results:") != std::string::npos) {
			if(plane == NONE && line.find("X (across) by Y (down)") != std::string::npos)
				plane = XY;
			if(plane == NONE && line.find("Y (across) by Z (down)") != std::string::npos)
				plane = YZ;
			if(plane == NONE && line.find("X (across) by Z (down)") != std::string::npos)
				plane = XZ;
			tag = VALUE;
			std::getline(infile, line);
			continue;
		}
		if(line.find("Relative Errors") != std::string::npos) {
			tag = ERROR; 
			std::getline(infile, line);
			continue;
		}
		if(line.find("X bin:") != std::string::npos || line.find("Y bin:") != std::string::npos || line.find("Z bin:") != std::string::npos) {
			if(tag == ERROR) {
				m_meshVal.push_back(m_val);  m_val.clear();
				m_meshErr.push_back(m_err);  m_err.clear();
				tag = INFO;
			}
			continue;
		}
		process(tag, line);
	}
	m_meshVal.push_back(m_val);  m_val.clear();
	m_meshErr.push_back(m_err);  m_err.clear();
}

/***************************************************************************/
/**
 * This method calls other methods to read information corresponding to the
 * type described in \a tag argument.
 */
void MeshTallyReader::process(Tag tag, std::string line)
{	
	switch(tag) {
		case INFO:
			readInfo(line);
			break;
		case BOUND:
			readBound(line);
			break;
		case VALUE:
			readValue(line);
			break;
		case ERROR:
			readError(line);
			break;
		default:
			break;
	}
}

/***************************************************************************/
/**
 * This method reads information lines, e.g. nps, tally number,...
 */
void MeshTallyReader::readInfo(std::string line)
{
	if(line.find(" Number of histories used for normalizing tallies =") != std::string::npos) {
		line.erase(0,51);
		std::istringstream(line) >> nps;
		INFO( Form("Number of histories used for normalizing tallies = %d",nps) );
	}
	if(line.find(" Mesh Tally Number") != std::string::npos) {
		line.erase(0,19);
		std::istringstream(line) >> tally;
		INFO( Form("Mesh Tally Number = %d",tally) );
	}
}

/***************************************************************************/
/**
 * This method reads axis boundary values of MCNP mesh output.
 */
void MeshTallyReader::readBound(std::string line)
{
	StringParser parser;
	std::vector<double> values;
	if(line.find("    X direction:") != std::string::npos) {
		line.erase(0,16);
		values = parser.getDouble(line);
		xbin = values.size()-1; xlow = values[0]; xhigh = values[xbin];
		INFO( Form("Number of X bins = %d , from %lf to %lf",xbin,xlow,xhigh) );
	}
	if(line.find("    Y direction:") != std::string::npos) {
		line.erase(0,16);
		values = parser.getDouble(line);
		ybin = values.size()-1; ylow = values[0]; yhigh = values[ybin];
		INFO( Form("Number of Y bins = %d , from %lf to %lf",ybin,ylow,yhigh) );
	}
	if(line.find("    Z direction:") != std::string::npos) {
		line.erase(0,16);
		values = parser.getDouble(line);
		zbin = values.size()-1; zlow = values[0]; zhigh = values[zbin];
		INFO( Form("Number of Z bins = %d , from %lf to %lf",zbin,zlow,zhigh) );
	}
}

/***************************************************************************/
/**
 * This method reads MCNP mesh values.
 */
void MeshTallyReader::readValue(std::string line)
{
	StringParser parser;
	std::vector<double> data = parser.getDouble(line);
	if(data.size() == 0)
		return;
	data.erase(data.begin());
	m_val.push_back(data);
}

/***************************************************************************/
/**
 * This method reads MCNP mesh value errors.
 */
void MeshTallyReader::readError(std::string line)
{
	StringParser parser;
	std::vector<double> data = parser.getDouble(line);
	if(data.size() == 0)
		return;
	data.erase(data.begin());
	m_err.push_back(data);
}

/***************************************************************************/
/**
 * This method extracts MCNP mesh values to histograms and write to a root file.
 */
void MeshTallyReader::extractHisto(TString filename, bool isUpdate)
{
	HistoUtilities hutil;
	TH3F* hist = 0;
	if(plane == XY) {
		TH3F *htmp = hutil.convert("", "Mesh Tally", m_meshVal, zlow, zhigh, ylow, yhigh, xlow, xhigh);
		hist = hutil.changeAxis(htmp,"ZYX",m_histoname);
	}
	if(plane == YZ) {
		TH3F *htmp = hutil.convert("", "Mesh Tally", m_meshVal, xlow, xhigh, zlow, zhigh, ylow, yhigh);
		hist = hutil.changeAxis(htmp,"XZY",m_histoname);
	}
	if(plane == XZ) {
		TH3F *htmp = hutil.convert("", "Mesh Tally", m_meshVal, ylow, yhigh, zlow, zhigh, xlow, xhigh);
		hist = hutil.changeAxis(htmp,"YZX",m_histoname);
	}	
	INFO("Writing out histograms to '"+filename+"'");
	TFile* file;
	if(isUpdate)
	  file = TFile::Open(filename,"UPDATE");
	else
	  file = TFile::Open(filename,"RECREATE");
	hist->Write();
	file->Close();
}
