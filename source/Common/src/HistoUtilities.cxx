/***************************************************************************
 Class  : HistoUtilities
 File   : HistoUtilities.h
 Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>
 Date   : 30-03-2015    
 ***************************************************************************/

#include "HistoUtilities.h"

/***************************************************************************/
/**
 * This method gets all histograms from a file and put them to
 * a vector \a histolist
*/
void HistoUtilities::getHistosFromFile(std::vector<TH1*> &histolist, TFile* file)
{
	TObject* obj;
	TIter next(file->GetListOfKeys());
	TKey* key;
	while ((key=(TKey*)next())) {
		obj = key->ReadObj();
		if (obj->InheritsFrom("TH1"))
			histolist.push_back( (TH1*)obj );
	}
}

/***************************************************************************/
/**
 * This method gets specific histograms (with a list \a histoname) from 
 * a file and put them to a vector \a histolist
*/
void HistoUtilities::getHistosFromFile(std::vector<TH1*> &histolist, std::vector<TString> histoname, TFile* file)
{
	TObject* obj;
	TIter next(file->GetListOfKeys());
	TKey* key;
	while ((key=(TKey*)next())) {
		obj = key->ReadObj();
		if (obj->InheritsFrom("TH1")) {
			for(std::vector<TString>::iterator histoname_it = histoname.begin(); histoname_it != histoname.end(); ++histoname_it) {
				TString name; name.Form("%s",obj->GetName());
				if( name == (*histoname_it) ) {
					histolist.push_back( (TH1*)obj );
					break;
				}
			}
		}
	}
}

/***************************************************************************/
/**
 * This method merges all histotgrams in a file
 */
void HistoUtilities::mergeHistos(TFile* file)
{
	TH1* merged = 0;
	TObject* obj;
	TIter next(file->GetListOfKeys());
	TKey* key;
	int i = 0;
	while ((key=(TKey*)next())) {
		obj = key->ReadObj();
		if (obj->InheritsFrom("TH1")) {
			if (i == 0)
				merged = (TH1*)obj->Clone();
			else
				merged->Add( (TH1*)obj );
		}
		++i;
	}
	merged->SetName("mergedHisto");
	writeHisto(merged,file);
}

/***************************************************************************/
/**
 * This method writes the histogam \a hist to a file 
 */
void HistoUtilities::writeHisto(TH1* hist, TFile* file)
{
	INFO( TString::Format( "Write histogram '%s' to file '%s'", hist->GetName(), file->GetName() ) );
	hist->Write( hist->GetName() );
}

/***************************************************************************/
/**
 * This method writes all histogram in a list \a hist to a file
 */
void HistoUtilities::writeHistos(std::vector<TH1*> hist, TFile* file)
{
	INFO( TString::Format( "Write histogram to file '%s'", file->GetName() ) );
	for (std::vector<TH1*>::iterator hist_it = hist.begin(); hist_it != hist.end(); ++hist_it)
		(*hist_it)->Write( (*hist_it)->GetName() );
}

/***************************************************************************/
/**
 * This method coverts a 1-dimension data vector to a TH1F histogram
 */ 
TH1F* HistoUtilities::convert(TString name, TString title, std::vector<double> data, double min, double max)
{
	TH1F* hist = 0;
	if(data.size() < 2) {
		ERROR("Not have enough data to create TH1 histogram");
		return hist;
	}
	if(max <= min) {
		ERROR("Incorrect (min,max) information");
		return hist;
	}
	int nbin = (int) data.size();
	hist = new TH1F(name, title, nbin, min, max);
	for (int i = 0; i < nbin; ++i) {
		hist->SetBinContent(i+1, data[i]);
	}
	return hist;
}

/***************************************************************************/
/**
 * This method converts a 2-dimension data vector to a TH2F histogram
 */
TH2F* HistoUtilities::convert(TString name, TString title, std::vector< std::vector<double> > data, double xmin, double xmax, double ymin, double ymax)
{
	TH2F* hist = 0;
	if(data.size() < 2) {
		ERROR("Not have enough data to create TH2 histogram");
		return hist;
	}
	if(xmax <= xmin || ymax <= ymin) {
		ERROR("Incorrect (min,max) information");
		return hist;
	}
	int nbinx = (int) data.size();
	int nbiny = (int) data[0].size();
	hist = new TH2F(name, title, nbinx, xmin, xmax, nbiny, ymin, ymax);
	for (int i = 0; i < nbinx; ++i) {
		for (int j = 0; j < nbiny; ++j) {
			hist->SetBinContent(i+1, j+1, data[i][j]);
		}
	}
	return hist;
}

/***************************************************************************/
/**
 * This method converts a 3-dimension data vector to a TH3F histogram
 */
TH3F* HistoUtilities::convert(TString name, TString title, std::vector< std::vector< std::vector<double> > > data, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
	TH3F* hist = 0;
	if(data.size() < 2) {
		ERROR("Not have enough data to create TH2 histogram");
		return hist;
	}
	if(xmax <= xmin || ymax <= ymin || zmax <= zmin) {
		ERROR("Incorrect (min,max) information");
		return hist;
	}
	int nbinx = (int) data.size();
	int nbiny = (int) data[0].size();
	int nbinz = (int) data[0][0].size();
	hist = new TH3F(name, title, nbinx, xmin, xmax, nbiny, ymin, ymax, nbinz, zmin, zmax);
	for (int i = 0; i < nbinx; ++i) {
		for (int j = 0; j < nbiny; ++j) {
			for (int k = 0; k < nbinz; ++k) {
				hist->SetBinContent(i+1, j+1, k+1, data[i][j][k]);
			}
		}
	}
	return hist;
}

/***************************************************************************/
/**
 * This method rotates a TH3F histogram axis
 */
TH3F* HistoUtilities::changeAxis(TH3F* hist, TString option, TString name)
{
	//TString name  = hist->GetName();
	TString title = hist->GetTitle();
	int nbinx = hist->GetNbinsX();
	int nbiny = hist->GetNbinsY();
	int nbinz = hist->GetNbinsZ();
	double xmin = hist->GetXaxis()->GetBinLowEdge(0);
	double xmax = hist->GetXaxis()->GetBinLowEdge(hist->GetNbinsX()) + hist->GetXaxis()->GetBinWidth(0);
	double ymin = hist->GetYaxis()->GetBinLowEdge(0);
	double ymax = hist->GetYaxis()->GetBinLowEdge(hist->GetNbinsY()) + hist->GetYaxis()->GetBinWidth(0);
	double zmin = hist->GetZaxis()->GetBinLowEdge(0);
	double zmax = hist->GetZaxis()->GetBinLowEdge(hist->GetNbinsZ()) + hist->GetZaxis()->GetBinWidth(0);

	TH3F* newhist = 0;
	if (option == "XYZ") {
		newhist = new TH3F(name, title, nbinx, xmin, xmax, nbiny, ymin, ymax, nbinz, zmin, zmax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(i+1, j+1, k+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else if (option == "ZXY") {
		newhist = new TH3F(name, title, nbinz, zmin, zmax, nbinx, xmin, xmax, nbiny, ymin, ymax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(k+1, i+1, j+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else if (option == "XZY") {
		newhist = new TH3F(name, title, nbinx, xmin, xmax, nbinz, zmin, zmax, nbiny, ymin, ymax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(i+1, k+1, j+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else if (option == "YXZ") {
		newhist = new TH3F(name, title, nbiny, ymin, ymax, nbinx, xmin, xmax, nbinz, zmin, zmax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(j+1, i+1, k+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else if (option == "ZYX") {
		newhist = new TH3F(name, title, nbinz, zmin, zmax, nbiny, ymin, ymax, nbinx, xmin, xmax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(k+1, j+1, i+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else if (option == "YZX") {
		newhist = new TH3F(name, title, nbiny, ymin, ymax, nbinz, zmin, zmax, nbinx, xmin, xmax);
		for (int i = 0; i < nbinx; ++i)
			for (int j = 0; j < nbiny; ++j)
				for (int k = 0; k < nbinz; ++k)
					newhist->SetBinContent(j+1, k+1, i+1, hist->GetBinContent(i+1, j+1, k+1) );
	}
	else
		ERROR("Incorrect type of axis transformation. Should use 'XYZ' 'ZXY' 'XZY' 'YXZ' 'ZYX' 'YZX'");

	return newhist;
}

/***************************************************************************/
