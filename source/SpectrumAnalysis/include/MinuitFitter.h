/**
 * \class    MinuitFitter
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Fitting spectrum using TMinuit class
 *
 * This class provides fitting method for full spectrum analysis
 * by using TMinuit class. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     MinuitFitter.h
 * 
 */

#include <iomanip>
#include <TMath.h>
#include <Riostream.h>
#include <TH1.h>
#include <TF1.h>
#include <TMinuit.h>
#include <TMatrixD.h>
#include <TVectorD.h>

#ifndef __MinuitFitter__
#define __MinuitFitter__

class MinuitFitter {

public:
	MinuitFitter ();
	~MinuitFitter () { delete m_minuit; }
	void setData (double* spectra);
	void setNbins (int nbins) { m_nbins = nbins; }
	void setNpars (int npars) { m_npars = npars; }
	void fit ();
	int getParameter (int parNo, double &par, double &err);
	TVectorD getNsol() const { return m_Nsol; }

private:
	static void FCN (int& , double* , double &f, double* fitPars, int iflag);
	static MinuitFitter* m_this;
	void Chi2 (double &f, double* fitPars);
	void BLH (double &f, double* fitPars);
	
	TMinuit* m_minuit;
	TVectorD m_spec;
	TMatrixD m_mat;
	//double   m_Nobstot;
	int 	 m_npars;
	int 	 m_nbins;
	TVectorD m_Nsol;
	TVectorD m_NsolErr;
};

#endif


