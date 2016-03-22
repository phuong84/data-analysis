/**
 * \class    StringParser
 * \ingroup  Common
 *
 * \brief    Parse string
 *
 * This class parses input string into other types of data 
 * (word, integer, double).
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     StringParser.h
 * 
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "ErrHandler.h"

#ifndef __StringParser__
#define __StringParser__

class StringParser {

public:
	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	StringParser() : message("StringParser") {};

	/// \brief Class destructor.
	~StringParser() {};

	/// \brief Parse string to words
	/// \param str input string
	/// \return vector of strings
	std::vector<std::string> getWord(std::string str);

	/// \brief Parse string to integers
	/// \param str input string
	/// \return vector of integers
	std::vector<int> getInt(std::string str);

	/// \brief Parse string to doubles
	/// \param str input string
	/// \return vector of doubles
	std::vector<double> getDouble(std::string str);
private:
	ErrHandler message;  ///< label of class to print out with message
};

#endif
