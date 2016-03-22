/**
 * \class    BaseFitter
 * \ingroup  SpectrumAnalysis
 *
 * \brief    Fitting spectrum using matrix method
 *
 * This class provides fitting method for full spectrum analysis
 * by using matrix technique. 
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     BaseFitter.h
 * 
 */

#include <fstream>
#include <vector>
#include <TH1.h>
#include <TString.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include "ErrHandler.h"
#include "Spectrum.h"

#ifndef __BaseFitter__
#define __BaseFitter__

class BaseFitter {

public:

	/// \brief Class constructor, initialize member variables
	BaseFitter() :	m_vals(0), m_errs(0), m_vec(0), m_mat(0), message("Fitter") {};
	
	/// \brief Class destructor
	virtual ~BaseFitter() {};
	
	/// \brief Set calibration spectra
	/// \param calib_spec vector of calibration spectra
	void setCalibSpectra(std::vector<Spectrum> calib_spec);
	
	/// \brief Set parameters
	/// \param pars vector of parameters
	virtual void setParameters(double* pars) {};
	
	/// \brief Fit measured spectrum
	/// \param spectrum measured spectrum
	/// \param useWeight do use weight
	virtual void fit(Spectrum spectrum, bool useWeight);
	
	/// \brief Get fitting results
	/// \return vector of fitting results
	std::vector<double> getVals() { return m_vals; }
	
	/// \brief Get fitting result errors
	/// \return vector of fitting result errors
	std::vector<double> getErrs() { return m_errs; }
	
	/// \brief Get size of fitting results
	/// \return size of fitting results
	int getNvals() { return (int)m_vals.size(); }

protected:
	/** Vector of results */
	//@{
	std::vector<double> m_vals, m_errs, m_vec; 
	//@}
	std::vector< std::vector<double> > m_mat;  ///< Matrix of calibration spectra 
	ErrHandler message;                        ///< Label of class to print out with message 
};

#endif
