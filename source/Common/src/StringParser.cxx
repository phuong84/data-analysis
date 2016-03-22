/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     StringParser.cxx
 * 
 */

#include "StringParser.h"

/***************************************************************************/
/**
 * This method converts a string into a set of words
 */
std::vector<std::string> StringParser::getWord(std::string str)
{
	std::stringstream stream(str);
	std::vector<std::string> words;
	std::string word;
	while(stream >> word){
    		words.push_back(word);
	}
	return words;
}

/***************************************************************************/
/**
 * This method converts a string into a set of integer numbers
 */
std::vector<int> StringParser::getInt(std::string str)
{
	std::stringstream stream(str);
	std::vector<int> values;
	int n;
	while(stream >> n){
    		values.push_back(n);
	}
	return values;
}

/***************************************************************************/
/**
 * This method converts a string into a set of double numbers
 */
std::vector<double> StringParser::getDouble(std::string str)
{
	std::stringstream stream(str);
	std::vector<double> values;
	double n;
	while(stream >> n){
    		values.push_back(n);
	}
	return values;
}

