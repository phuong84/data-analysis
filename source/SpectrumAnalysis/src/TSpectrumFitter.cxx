/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     TSpectrumFitter.cxx
 * 
 */

#include "TSpectrumFitter.h"

/***************************************************************************/
/**
 * This method sets parameters \ref m_nIter, \ref m_nRep, \ref m_boost
 */
void TSpectrumFitter::setParameters(double* pars)
{
	m_nIter = (int)pars[0];
	m_nRep  = (int)pars[1];
	m_boost = pars[2];
}

/***************************************************************************/
/**
 * This method fits the measured spectrum and return the vectors
 * of results (\ref m_vals). The fitting method here is TSpectrum::Unfolding()
 * method.
 */
void TSpectrumFitter::fit(Spectrum spectrum, bool useWeight)
{
	double* spec = spectrum.getData();
	int nvals = (int)m_mat.size();
	int nchannels = (int)spectrum.getNchannels();
	m_vec.insert(m_vec.end(),&spec[0],&spec[nchannels]);

	TSpectrum *s = new TSpectrum();
	double** mat = new double*[nvals];
	for(int i = 0; i < nvals; ++i)
		mat[i] = &m_mat[i][0];
	const double** const_mat = const_cast<const double **>(mat);
	s->Unfolding(spec,const_mat,nchannels,nvals,m_nIter,m_nRep,m_boost);
	m_vals.insert(m_vals.end(),&spec[0],&spec[nvals]);
}

