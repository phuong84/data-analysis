/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     BaseFitter.cxx
 * 
 */

#include "BaseFitter.h"

/***************************************************************************/
/**
 * This method gets a vector of calibration spectra and stores to \ref m_mat
 */
void BaseFitter::setCalibSpectra(std::vector<Spectrum> calib_spec)
{
	int nchannel = calib_spec[0].getNchannels();
	for(int i = 0; i < (int)calib_spec.size(); ++i) {
		std::vector<double> spec(calib_spec[i].getData(),calib_spec[i].getData()+nchannel);
		m_mat.push_back(spec);
	}
}

/***************************************************************************/
/**
 * This method fits the measured spectrum with \ref m_mat and return the vectors
 * of results (\ref m_vals, \ref m_errs). The fitting method here is inverted
 * matrix method.
 */
void BaseFitter::fit(Spectrum spectrum, bool useWeight)
{
	double* spec = spectrum.getData();
	const int size = (int)m_mat.size();
	const int N = (int)spectrum.getNchannels();
	m_vec.insert(m_vec.end(),&spec[0],&spec[N]);

	TVectorD beta(size);
	TMatrixD alpha(size,size);
	double temp, weight;
	for(int i = 0; i < size; ++i) {
		beta(i) = 0.;
		for(int k = 0; k < N; ++k) {
			temp = m_vec[k]*m_mat[i][k];
			if(useWeight) {
				weight = 1./m_vec[k];
				temp *= weight;
			}
			beta(i) += temp;
		}
		for(int j = 0; j < size; ++j) {
			alpha(i,j) = 0.;
			for(int k = 0; k < N; ++k) {
				temp = m_mat[i][k]*m_mat[j][k];
				if(useWeight) {
					weight = 1./m_vec[k];
					temp *= weight;
				}
				alpha(i,j) += temp;
			}
		}
	}
	
	TMatrixD epsilon = alpha.Invert();
	TVectorD a = epsilon * beta;
	double* vals = a.GetMatrixArray();
	m_vals.insert(m_vals.end(),&vals[0],&vals[size]);
	for(int i = 0; i < size; ++i)
		m_errs[i] = sqrt(epsilon(i,i));
}


