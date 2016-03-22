/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     TallyReader.cxx
 * 
 */

#include "TallyReader.h"

/***************************************************************************/
/**
 * This method reads each line in MCNP output file and decides which
 * information type can be extracted from that line, then it calls \ref 
 * process() method to read information.
 */
void TallyReader::read(TString filename)
{
	// Open MCNP output file for reading
	std::ifstream infile;
	infile.open(filename.Data(), std::ifstream::in);
	if (infile.fail()) {
		ERROR("Cannot open file '"+filename+"'");
	}
	INFO("Opening file '"+filename+"'");

	// Start to extract information line by line
	Tag tag = INFO;
	int n_line = 0;
	std::string line;
	while (std::getline(infile, line)) {
		++n_line;
		if(line.find("1tally ") != std::string::npos && line.find("nps =") != std::string::npos) {
			tag = TALLYINFO;
		}
		if(tag == TALLYINFO && line.find("energy") != std::string::npos) {
			tag = VALUE;
			continue;
		}
		if(tag == VALUE && line.find("total") != std::string::npos) {
			energies.push_back(energy); energy.clear();
			values.push_back(value);    value.clear();
			errors.push_back(error);    error.clear();
			tag = INFO;
			continue;
		}
		process(tag, line);
	}
	INFO( Form( "Found %d tallies in total",(int)tally.size() ) );
}

/***************************************************************************/
/**
 * This method calls other methods to read information corresponding to the
 * type described in \a tag argument.
 */
void TallyReader::process(Tag tag, std::string line)
{
	switch(tag){
		case INFO:
			readInfo(line);
			break;
		case TALLYINFO:
			readTallyInfo(line);
			break;
		case VALUE:
			readValue(line);
			break;
		default:
			break;
	}
}

/***************************************************************************/
/**
 * This method reads information lines, e.g. \ref raw_nps,...
 */
void TallyReader::readInfo(std::string line)
{
	if(line.find(" the original number of histories was") != std::string::npos) {
		line.erase(0,38);
		std::istringstream(line) >> raw_nps;
	}
}

/***************************************************************************/
/**
 * This method reads tally information lines, e.g. tally number, nps,...
 */
void TallyReader::readTallyInfo(std::string line)
{
	size_t pos = line.find("nps =");
	if(pos != std::string::npos) {
		std::string line1 = line.substr(0,pos-1);
		line1.erase(0,7);
		int n = atoi(line1.c_str());
		tally.push_back(n);
		std::string line2 = line.substr(pos,line.size()-1);
		line2.erase(0,6);
		n = atoi(line2.c_str());
		nps.push_back(n);
	}
}

/***************************************************************************/
/**
 * This method reads MCNP output results (\ref energy, \ref value, \ref error).
 */
void TallyReader::readValue(std::string line)
{
	StringParser parser;
	std::vector<double> numbers = parser.getDouble(line);
	if (numbers.size() == 3) {
		energy.push_back(numbers[0]);
		value.push_back(numbers[1]);
		error.push_back(numbers[2]);
	}
}

/***************************************************************************/
/**
 * This method extracts MCNP output results to histograms and write to a root file.
 */
void TallyReader::extractHisto(TString filename, bool isUpdate)
{
	TFile* file;

	if(isUpdate)
		file = TFile::Open(filename,"UPDATE");
	else
		file = TFile::Open(filename,"RECREATE");
	file->cd();
	INFO("Write histograms to file '"+filename+"'");
	for (int i = 0; i < (int)tally.size(); ++i) {
		double min = energies[0][1] - 2*energies[0][0];
		double max = energies[0].at(energies[0].size()-1);
		HistoUtilities hutil;
		TH1F* hist = hutil.convert( TString( Form("Tally%d",tally[i]) ), TString( Form("Tally%d",tally[i]) ), values[i], min, max );	
		hist->Write();
	}
	file->Close();
}
