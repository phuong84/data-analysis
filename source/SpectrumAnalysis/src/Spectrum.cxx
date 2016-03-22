/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     Spectrum.cxx
 * 
 */

#include "Spectrum.h"

enum types{ETFE, TKA, MCA, RRUFF, UNKNOWN};

types hash_types (TString type) 
{
	if (type == "ETFE")  return ETFE;
	if (type == "TKA")   return TKA;
	if (type == "MCA")   return MCA;
	if (type == "RRUFF") return RRUFF;
	return UNKNOWN;
}

double gaussianF(double* x, double* par);
double lorentzianF(double* x, double* par);
double voigtF(double* x, double* par);
double gaussianPeak(double* x, double* par);
double lorentzianPeak(double* x, double* par);
double voigtPeak(double* x, double* par);
double exponential(double* x, double* par);
double linear(double* x, double* par);

/***************************************************************************/

void Spectrum::read(TString filename, TString type)
{
	/******** Open file ********/
	INFO("Opening file '"+filename+"'");
	std::ifstream file(filename);
	if (file.fail()) {
		ERROR("Cannot open file '"+filename+"'");
		return;
	}

	/******** Read file ********/
	switch (hash_types(type)) {
		case ETFE:
			m_spectrum = readETFE(file);
			break;
		case TKA:
			m_spectrum = readTKA(file);
			break;
		case MCA:
			m_spectrum = readMCA(file);
			break;
		case RRUFF:
			m_spectrum = readRRUFF(file);
			break;
		default:
			ERROR("Undefined file type!");
			return;
	}
	m_Nchannels = m_spectrum->GetNbinsX();
}

/***************************************************************************/

void Spectrum::setCalibCoeff(std::vector<double> coeff)
{
	m_calibCoeff = coeff;
}

/***************************************************************************/

void Spectrum::setEffCoeff(std::vector<double> coeff)
{
	m_effCoeff = coeff;
}

/***************************************************************************/

void Spectrum::scale(double x)
{
	m_spectrum->Scale(x);
}

/***************************************************************************/

double* Spectrum::getData()
{
	double* spectrum = new double[m_Nchannels];
	for(int i = 1; i <= m_Nchannels; ++i)
		spectrum[i-1] = m_spectrum->GetBinContent(i);
	return spectrum;
}

/***************************************************************************/

TH1* Spectrum::getHistogram()
{
	return m_spectrum;
}

/***************************************************************************/

int Spectrum::getNchannels()
{
	return m_Nchannels;
}

/***************************************************************************/

std::vector<double> Spectrum::getCalibCoeff()
{
	return m_calibCoeff;
}

/***************************************************************************/

std::vector<double> Spectrum::getCalibCoeff(std::vector<int> channel, std::vector<double> energy)
{
	std::vector<double> channel_d(channel.begin(), channel.end());
	std::vector<double> result = fitFunc(m_calibFormula, channel_d, energy);
	return result;
}

/***************************************************************************/

std::vector<double> Spectrum::getEffCoeff()
{
	return m_effCoeff;
}

/***************************************************************************/

std::vector<double> Spectrum::getEffCoeff(std::vector<double> energy, std::vector<double> efficiency)
{
	std::vector<double> result = fitFunc(m_effFormula, energy, efficiency);
	return result;
}

/***************************************************************************/

double Spectrum::getValue(int channel)
{
	TF1* f = new TF1("f", m_calibFormula.Data(), 1, m_Nchannels);
	f->SetParameters(&m_calibCoeff[0]);
	return f->Eval(channel);
}

/***************************************************************************/

double Spectrum::getEfficiency(double energy)
{
	TF1* f = new TF1("f", m_effFormula.Data(), getValue(1), getValue(m_Nchannels));
	f->SetParameters(&m_effCoeff[0]);
	return f->Eval(energy);
}

/***************************************************************************/

double Spectrum::getEfficiency(int channel)
{
	return getEfficiency(getValue(channel));
}

/***************************************************************************/

TF1* Spectrum::fitPeak(double min, double max, TString formula)
{
	TH1* hist = (TH1*)m_spectrum->Clone(); 
	TF1* f = new TF1 ("f", formula, min, max);
	f->SetParameter(3,(min+max)/2);
	f->SetParameter(4,(max-min)/2);
	hist->Fit("f", "r");
	double pars[10];
	f->GetParameters(pars);
	return f;
}

/***************************************************************************/

std::vector<TF1*> Spectrum::fitMultiPeak(const int N, double min, double max)
{
	TH1* hist = (TH1*)m_spectrum->Clone();
	TF1* f = new TF1("f", voigtF, min, max, 4*(N+1));
	f->SetTitle("Total");
	TF1* bkg = new TF1("bkg", linear, min, max, 2);
	bkg->SetTitle("Background");

	hist->GetXaxis()->SetRangeUser(min,max);

	f->FixParameter(0,N);
	f->SetParLimits(1,50,200);
	f->SetParLimits(2,-0.5,0.5);
	for (int i=1; i<=N; ++i) {
		f->SetParameter(i*4,hist->GetMaximum()/2);
		//f->SetParameter(i*3+1,(max+min)/2);
		f->SetParameter(i*3+2,(max-min)/6);
	}
	f->SetParameter(4,9);
	//f->SetParLimits(5,1,10);
	f->SetParameter(8,19);
	//f->SetParLimits(8,1,10);
	f->SetParameter(12,21);
	//f->SetParLimits(11,1,10);
	
	hist->Fit("f", "r0");
	double pars[4*(N+1)];
	std::vector<TF1*> func;
	f->GetParameters(pars);
	func.push_back(f);
	bkg->SetParameters(&pars[1]);
	func.push_back(bkg);
	for (int i=0; i<N; ++i) {
		TString str;
		str.Form("Peak%d",i+1);
		TF1* peak = new TF1("peak", voigtPeak, min, max, 4);
		peak->SetTitle(str);
		peak->SetParameters(&pars[(i+1)*4]);
		func.push_back(peak);
	}
	return func;
}

/***************************************************************************/

std::vector<double> Spectrum::findPeaks(double sigma, double threshold)
{
	TSpectrum* s = new TSpectrum();
	TH1* hist = (TH1*)m_spectrum->Clone();
	const int Npeaks = s->Search(hist, sigma, "", threshold);
	double* pos = s->GetPositionX();
	
	MESSAGE( Form("Number of peaks: %d",Npeaks) );
	std::cout << "... Peak positions: ";
	for (int i = 0; i < Npeaks; ++i) {
		int bin = hist->FindBin(pos[i]);
		std::cout << hist->GetBinLowEdge(bin);
		if (i != Npeaks-1)  std::cout << ", ";
		else  std::cout << std::endl;
	}
	std::vector<double> v_pos(pos,pos+Npeaks);
	return v_pos;
}

/***************************************************************************/

TH1* Spectrum::estimateBkg(int Niter, TString option)
{
	TSpectrum* s = new TSpectrum();
	TH1* hist = (TH1*)m_spectrum->Clone();
	TH1* bkg = s->Background(hist, Niter, option);
	bkg->SetName("bkg");
	bkg->SetTitle("Background");
	INFO( Form("Store background in histogram '%s'",bkg->GetName()) );
	return bkg;
}

/***************************************************************************/

TH1* Spectrum::subtractBkg(TH1* bkg)
{
	INFO( Form("Subtract background from '%s'",bkg->GetName()) );
	TString name = m_spectrum->GetName();
	TH1* hist = (TH1*)m_spectrum->Clone(name+"_subbkg");
	hist->Add(bkg,-1.);
	return hist;
}

/***************************************************************************/

TH1* Spectrum::readETFE(std::ifstream& file)
{
	TH1* hist = 0;
	
	/******** Read data ********/
	double theta[2000], d[2000], counts[2000];
	int Nbins = 0;
	double theta_min = 0., theta_max = 0., theta_width = 0., d_max = 0., d_min = 0., d_width = 0.;
	TString str;
	INFO("Information: ");
	int foundInfo = 0;
	int foundData = 0;
	while (file >> str) {  // bat dau doc thong tin
		if (str[0] == '*') {  // bat dau kiem tra dau '*'
			foundInfo = 1;
			TString substr = str(1, str.Length()-1);
			if (substr != "Data") {  // kiem tra khong phai '*Data'
				str.ReadLine(file);
				INFO("... "+substr+": "+str);
			} 
			else if (substr == "Data") {  // kiem tra '*Data'
				foundData = 1;
				int i=0;
				while (!file.eof()) {  // doc du lieu
					++i;
					file >> theta[i] >> d[i] >> counts[i];
				}
				INFO("... Stored in histogram 'h_theta'");
				Nbins = i-1;
				theta_min = theta[1];
				theta_width = (theta[Nbins]-theta[1])/(Nbins-1);
				theta_max = theta[Nbins] + theta_width;
				d_max = d[1];
				d_width = (d[1]-d[Nbins])/(Nbins-1);
				d_min = d[Nbins] + d_width;
				INFO( Form("... Number of bins: %d",Nbins) );
				INFO( Form("... Min(2theta): %lf",theta_min) );
				INFO( Form("... Max(2theta): %lf",theta_max) );
				INFO( Form("... Width(2theta): %lf",theta_width) );
			}  
		}  // ket thuc kiem tra dau '*'
	}  // ket thuc doc thong tin
	if (!foundInfo) {
		ERROR("Cannot find any information. Please specify '*'.");
	}
	if (!foundData) {
		ERROR("Cannot find data. Please specify '*Data'.");
		return hist;
	}
	file.close();
	
	/**** Create histogram ****/
	TH1F* h_theta = new TH1F("h_theta", "2#theta(degrees)", Nbins, theta_min, theta_max);
	TH1F* h_d = new TH1F("h_d", "d(A)", Nbins, d_min, d_max);
	for (int i = 1; i <= Nbins; ++i) {
		h_theta->SetBinContent(i,counts[i]);
		h_d->SetBinContent(i,counts[i]);
	}
	hist = (TH1*)h_theta->Clone();
	
	return hist;
}

/***************************************************************************/

TH1* Spectrum::readTKA(std::ifstream& file)
{
	TH1* hist = 0;
	
	/******** Read data ********/
	int counts[10000];
	int n = 0;
	INFO("Information: ");
	file >> n;
	if (n == 0) 
		WARN("Zero livetime");
	else
		INFO( Form("Livetime: %d",n) );
	file >> n;
	if (n == 0) 
		WARN("Zero realtime");
	else
		INFO( Form("Realtime: %d",n) );
	
	counts[0] = 0; counts[1] = 0;  // dat so dem 2 kenh dau la 0
	int Nbins = 2;
	while (file >> n) {  // bat dau doc du lieu
		counts[Nbins] = n;
		++Nbins;
	}
	if (Nbins == 0) {
		ERROR("Cannot find data.");
		return hist;
	}
	else {
		INFO( Form("Number of bins: %d",Nbins) );
	}
	file.close();

	/**** Create histogram ****/
	hist = new TH1F("h", "Channel", Nbins, 0, Nbins);
	for (int i = 1; i <= Nbins; ++i) {
		hist->SetBinContent(i,counts[i-1]);
	}
	
	return hist;
}

/***************************************************************************/

TH1* Spectrum::readMCA(std::ifstream& file)
{
	TH1* hist = 0;
	
	/******** Read data ********/
	int counts[10000];
	int Nbins=0;
	float minCh=0., maxCh=0., minE=0., maxE=0.;
	TString str, titleX = "Channel";
	INFO("Information: ");
	int foundInfo = 0;
	int foundData = 0;
	while (1) {  // bat dau doc thong tin
		str.ReadLine(file);
		if (str == "<<PMCA SPECTRUM>>") {  // kiem tra 'PMCA SPECTRUM'
			foundInfo = 1;
			while (1) {
				str.ReadLine(file);
				if (str.BeginsWith("<<"))
					break;
				int i = str.First('-');
				TString substr1 = str(0, i-1);
				TString substr2 = str(i+1, str.Length()-1);
				substr1.Remove(TString::kBoth,' ');
				substr2.Remove(TString::kBoth,' ');
				INFO(substr1+": "+substr2);
			}			
		} 
		if (str == "<<CALIBRATION>>") {  // kiem tra 'CALIBRATION'
			foundInfo = 1;
			str.ReadLine(file);
			int i = str.First('-');
			TString substr1 = str(0, i-1);
			TString substr2 = str(i+1, str.Length()-1);
			substr1.Remove(TString::kBoth,' ');
			substr2.Remove(TString::kBoth,' ');
			INFO(substr1+": "+substr2);
			titleX = substr2;
			file >> minCh >> minE;
			file >> maxCh >> maxE;
		} 
		if (str == "<<DATA>>") {  // kiem tra 'DATA'
			foundInfo = 1;
			foundData = 1;
			while (!file.eof()) {
				str.ReadLine(file);
				if (str == "<<END>>")
					break;
				counts[Nbins] = str.Atoi();
				++Nbins;
			}  
			INFO( Form("Number of bins: %d",Nbins) );
		}  
		if (str == "<<END>>") {  // kiem tra 'END'
			break;
		}  
	}  // ket thuc doc thong tin
	if (foundInfo == 0) {
		ERROR("Cannot find any information. Please specify '<< >>'");
	}
	if (!foundData) {
		ERROR("Cannot find data. Please specify '<<DATA>>'");
		return hist;
	}
	file.close();
	
	/**** Create histogram ****/
	float binwidth = 0.;
	if (maxE == 0 || maxCh == 0)
		binwidth = 1;
	else 
		binwidth = (maxE-minE)/(maxCh-minCh);
	hist = new TH1F("h", titleX, Nbins, minE, Nbins*binwidth);
	for (int i = 1; i <= Nbins; ++i) {
		hist->SetBinContent(i,counts[i-1]);
	}	
	return hist;
}

/***************************************************************************/

TH1* Spectrum::readRRUFF(std::ifstream& file)
{
	TH1* hist = 0;
	
	/******** Read data ********/
	double x[10000], counts[10000];
	int Nbins=0;
	float min=0., max=0.;
	TString str, titleX = "2#theta(degrees)";
	INFO("Information: ");
	int foundInfo = 0;
	int foundData = 0;
	while (1) {  // bat dau doc thong tin
		str.ReadLine(file);
		if (str == "##END=")
			break;
		if (str.BeginsWith("##")) {  // kiem tra '##'
			foundInfo = 1;
			TString substr = str(2, str.Length()-1);
			int i = substr.First('=');
			TString substr1 = substr(0, i);
			TString substr2 = substr(i+1, str.Length()-1);
			substr1.Remove(TString::kBoth,' ');
			substr2.Remove(TString::kBoth,' ');
			INFO(substr1+": "+substr2);	
		}		
		else {
			foundData = 1;
			int i = str.First(',');
			TString substr1 = str(0, i);
			TString substr2 = str(i+1, str.Length()-1);
			x[Nbins] = substr1.Atof();
			counts[Nbins] = substr2.Atof();
			++Nbins;
			if(Nbins%500 == 0) INFO( Form("Data entry: %d",Nbins) );
		}    
	}  // ket thuc doc thong tin
	INFO( Form("... Number of bins: %d",Nbins) );
	if (!foundInfo) {
		ERROR("Cannot find any information. Please specify '##'");
	}
	if (!foundData) {
		ERROR("Cannot find data. Please specify data.");
		return hist;
	}
	file.close();
	
	/**** Create histogram ****/
	min = x[0]; max = x[Nbins-1];
	if(min > 100.) titleX = "Wave numbers (cm-1)";
	hist = new TH1F("h", titleX, Nbins, min, max);
	for (int i = 1; i <= Nbins; ++i) {
		hist->SetBinContent(i,counts[i-1]);
	}	
	return hist;
}

/***************************************************************************/

std::vector<double> Spectrum::fitFunc(TString formula, std::vector<double> x, std::vector<double> y)
{
	if( x.size() == 0 || x.size() != y.size() ) {
		WARN("Not enough data to calculate coefficients.");
		return getEffCoeff();
	}
	if(formula == "") {
		WARN("No formula to be set.");
		return getEffCoeff();
	}
	int size = (int)x.size();
	TF1* f = new TF1("f", formula.Data(), *std::min_element(x.begin(), x.end()), *std::max_element(x.begin(), x.end()));
	TGraph *g = new TGraph(size, &x[0], &y[0]);
	g->Fit("f","r");
	const int nPars = f->GetNpar();
	double pars[nPars];
	f->GetParameters(pars);
	std::vector<double> fitpars(pars, pars + nPars);
	return fitpars;
}

/***************************************************************************/

// Gaussian Peaks + Background
double gaussianF(double *x, double *par) 
{
	double f = 0.;
	f = linear(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
		f += gaussianPeak(x,&par[i*3]);
	}
	return f;
}

// Lorenzian Peaks + Background
double lorentzianF(double *x, double *par) 
{
	double f = 0.;
	f += linear(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
		f += lorentzianPeak(x,&par[i*3]);
	}
	return f;
}

// Voigt Peaks + Background
double voigtF(double *x, double *par) 
{
	double f = 0.;
	f += linear(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
		f += voigtPeak(x,&par[i*3]);
	}
	return f;
}

/***************************************************************************/

// Gaussian peak function
double gaussianPeak(double *x, double *par) 
{
	return par[0]*TMath::Gaus(x[0], par[1], par[2]);
}

// Lorenzian peak function
double lorentzianPeak(double *x, double *par) 
{
	return par[0]/TMath::Max( 1.e-10,(x[0]-par[2])*(x[0]-par[2]) + .25*par[1]*par[1] );
}

// Voigt peak function
double voigtPeak(double *x, double *par) 
{
	return par[0]*TMath::Voigt(x[0]-par[1], par[2], par[3]);
}

/***************************************************************************/

// Exponential background function
double exponential(double *x, double *par) 
{
	return TMath::Exp(par[0] + par[1]*x[0]);
}

// Linear background function
double linear(double *x, double *par) 
{
	return par[0] + par[1]*x[0];
}

