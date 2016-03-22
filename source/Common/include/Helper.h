#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifndef __GenericData__
#define __GenericData__

/**
 * \class    GenericData
 * \ingroup  Common
 *
 * \brief    Generic data structure
 *
 * This class is obtained from http://ubuntuforums.org/showthread.php?t=1273582
 * This is a data structures which can store a mixture of any data type.
 *
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     Helper.h
 * 
 */

class GenericData {

public:
	/// \brief Class constructor,
	/// initialize char* type data
	GenericData(const char* data) : m_data(data) {}

	/// \brief Class constructor,
	/// initialize string type data
	GenericData(const std::string& data) : m_data(data) {}
	
	/// \brief Class constructor,
	/// initialize char type data
	GenericData(char ch) : m_data(1, ch) {}
	
	/// \brief Class constructor,
	/// initialize integer type data
	GenericData(int val) { std::stringstream ss; ss << val; m_data = ss.str(); }
	
	/// \brief Class constructor,
	/// initialize double type data
	GenericData(double val) { std::stringstream ss; ss << val; m_data = ss.str(); }

	/// \brief Class destructor
        ~GenericData() {};

	/// \brief Method of converting to string
	operator std::string () const { return m_data; }
	/// \brief Method of converting to char
	operator char () const { return m_data[0]; }
	/// \brief Method of converting to int
	operator int () const { int val = 0; std::stringstream ss(m_data); ss >> val; return val; }
	/// \brief Method of converting to double
	operator double () const { double val = 0; std::stringstream ss(m_data); ss >> val; return val; }

private:
	std::string m_data;  ///< string type data
};

#endif
