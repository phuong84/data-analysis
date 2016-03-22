/**
 * \class    Config
 * \ingroup  Common
 *
 * \brief    Read and parse configuration file
 *
 * This class can be used to open and retrieve information from 
 * configuration file. The information in config file is usually string 
 * type, this class provides methods to convert these strings into 
 * corresponding inputs for program.
 *
 * The structure of configuration file should be:
 *
 * Keyword : Value(s)
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     Config.h
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <TString.h>
#include "ErrHandler.h"

#ifndef __Config__
#define __Config__


class Config {

public:
	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	Config() : message("Config") {};
	
	/// \brief Class destructor
	~Config() {};
	
	/// \brief Method for openning file
	/// \param filename name of input file
	/// \return status of openning file
	int open(TString filename);
	
	/// \brief Method for retrieve string type value
	/// \param key information keyword
	/// \param def default value
	/// \return string value
	TString get(TString key, const std::string& def);

	/// \brief Method for retrieve char* type value
	/// \param key information keyword
	/// \param def default value
	/// \return string value
	TString get(TString key, const char *def);
	
	/// \brief Method for retrieve integer type value
	/// \param key information keyword
	/// \param def default value
	/// \return integer value
	int get(TString key, int def);
	
	/// \brief Method for retrieve double type value
	/// \param key information keyword
	/// \param def default value
	/// \return double value
	double get(TString key, double def);
	
	/// \brief Method for retrieve boolean type value
	/// \param key information keyword
	/// \param def default value
	/// \return boolean value
	bool get(TString key, bool def);
	
	/// \brief Method for retrieve string type vector
	/// \param key information keyword
	/// \param delim deliminator
	/// \return string vector
	std::vector<TString> getString(TString key, char delim = ',');
	
	/// \brief Method for retrieve integer type vector
	/// \param key information keyword
	/// \param delim deliminator
	/// \return integer vector
	std::vector<int> getInt(TString key, char delim = ',');
	
	/// \brief Method for retrieve double type vector
	/// \param key information keyword
	/// \param delim deliminator
	/// \return double vector
	std::vector<double> getDouble(TString key, char delim = ',');

private:
	std::ifstream file;                  ///< input filename
	std::map<TString,TString> m_config;  ///< pair of (keyword, value)
	ErrHandler message;                  ///< label of class to print out with message
};

#endif 
