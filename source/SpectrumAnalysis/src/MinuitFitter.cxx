/***************************************************************************
 Class  : MinuitFitter 
 File   : MinuitFitter.cxx
 Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>
 Date   : 25-12-2015    
 ***************************************************************************/

#include "MinuitFitter.h"

/***************************************************************************/

static void A (int& , double* , double &f, double* fitPars, int iflag);
MinuitFitter* MinuitFitter::m_this = 0;

/***************************************************************************/

MinuitFitter::MinuitFitter ()
	: m_minuit	(0)
{
	m_this = this;
}

/***************************************************************************/

void MinuitFitter::setData (double *spectra) 
{
	m_spec.Use(m_nbins, spectra);
	m_mat.Use(m_nbins,m_npars,&spectra[m_nbins]);
}

/***************************************************************************/

void MinuitFitter::FCN (int& , double* , double &f, double* fitPars, int iflag)
{
	m_this->BLH(f, fitPars);
}

/***************************************************************************/

void MinuitFitter::fit ()
{
	const int Nunknowns = m_mat.GetNcols();
	m_minuit = new TMinuit(2*Nunknowns);

	int ierr = 0;
	double args[10];
	double Ntot = 0.0;
	for (int i = 0; i < m_spec.GetNrows(); i++)
		Ntot += m_spec(i);
	
	int ipar = 0;
	for (int i = 0; i < m_mat.GetNcols(); i++) {
		m_minuit->mnparm(ipar, TString::Format("p%d", i), 0.1*Ntot, 0.01*Ntot, 0.0, Ntot, ierr); 
		ipar++;
	}

	m_npars = ipar; // true number of unknowns

	// define the FCN
	m_minuit->SetFCN(&FCN);
  
	// error level: 1 
	args[0] = 0.5;
	m_minuit->mnexcm("SET ERR", args, 1, ierr);
  
	// print warnings ?
	m_minuit->mnexcm("SET NOWARNINGS", args, 0, ierr);
  
	// define fit strategy
	args[0] = 2;
	m_minuit->mnexcm("SET STRATEGY", args, 1, ierr);

	// call SIMPLEX
	args[0] = 10000;   // maximum function calls
	args[1] = 0.1;     // tolerance at minimum
	//m_tminuit->mnexcm("SIMPLEX", args, 2, ierr);

	// call MIGRAD
	args[0] = 50000;   // maximum function calls
	args[1] = 0.1;     // tolerance at minimum
	m_minuit->mnexcm("MIGrad", args, 2, ierr);

	args[0] = 50000;   // maximum function calls
	//m_minuit->mnexcm("IMProve", args, 1, ierr);

	// add MINOS analysis 
	args[0] = 50000;
	//m_minuit->mnexcm("MINOs", args, 1, ierr);
	

	// Getting outputs
	ipar = 0;
	m_Nsol.ResizeTo(m_mat.GetNcols());
	m_NsolErr.ResizeTo(m_mat.GetNcols());

	for (int i = 0; i < m_mat.GetNcols(); i++) {
		double Nx = 0.0, NxErr = 0.0;
		getParameter(ipar, Nx, NxErr); ipar++;
		m_Nsol(i) = Nx;
		m_NsolErr(i) = NxErr;
	}

}

/***************************************************************************/

void MinuitFitter::Chi2 (double &f, double* fitPars)
{
	int ipar = 0;
	TVectorD Nsol(m_mat.GetNcols());
	for (int i = 0; i < m_mat.GetNcols(); i++) {
		Nsol(i) = fitPars[ipar]; 
		ipar++;
	}
	
	// Tinh gia tri Chi-square
	double chi2 = 0.0;
	const TVectorD m_exp = m_mat*Nsol;
	for (int i = 0; i < m_mat.GetNrows(); i++) {
		const double Nobs = m_spec(i);
		const double Nexp = m_exp(i);
		if (Nobs < 1.0) continue;
		const float x = (Nexp - Nobs)/sqrt(Nobs);
		chi2 += x*x;
	}

	const int ndf = m_mat.GetNrows()*m_mat.GetNcols() - m_npars;
	f = chi2/ndf;
}

/***************************************************************************/

void MinuitFitter::BLH (double &f, double* fitPars)
{
	int ipar = 0;
	TVectorD Nsol(m_mat.GetNcols());
	for (int i = 0; i < m_mat.GetNcols(); i++) {
		Nsol(i) = fitPars[ipar]; 
		ipar++;
	}
	
	// Tinh gia tri -LogL
	double logL = 0.0;
	const TVectorD m_exp = m_mat*Nsol;
	for (int i = 0; i < m_mat.GetNrows(); i++) {
		const double Nobs = m_spec(i);
		const double Nexp = m_exp(i);

		// Gaussian
		// if (Nobs < 1.0) continue;
		// const float x = (Nexp - Nobs)/sqrt(Nobs);
		// logL -= 0.5*x*x;
		
		// Poisson
		logL += (Nexp > 0.0) ? Nobs*log(Nexp) - Nexp : -1.e10;
	}

	f = -logL;
}

/***************************************************************************/

int MinuitFitter::getParameter (int parNo, double &par, double &err)
{
	return m_minuit->GetParameter(parNo, par, err);
}

/***************************************************************************/


