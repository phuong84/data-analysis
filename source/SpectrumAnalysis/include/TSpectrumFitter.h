/**
 * \class    TSpectrumFitter
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Fitting spectrum using TSpectrum class
 *
 * This class provides fitting method for full spectrum analysis
 * by using TSpectrum class. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     TSpectrumFitter.h
 * 
 */

#include <TSpectrum.h>
#include "BaseFitter.h"

#ifndef __TSpectrumFitter__
#define __TSpectrumFitter__

class TSpectrumFitter : public BaseFitter {

public:

	/// \brief Class constructor, initialize member variables
	TSpectrumFitter() : BaseFitter() {};
	
	/// \brief Class destructor
	~TSpectrumFitter() {};
	
	/// \brief Set parameters
	/// \param pars vector of parameters
	void setParameters(double* pars);
	
	/// \brief Fit measured spectrum
	/// \param spectrum measured spectrum
	/// \param useWeight do use weight
	void fit(Spectrum spectrum, bool useWeight);

private:
	int m_nIter, m_nRep;
	double m_boost;
};

#endif
