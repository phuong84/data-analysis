/**
 * \class    Table
 * \ingroup  Common
 *
 * \brief    Create table
 *
 * This class can be used to create output tables.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     Table.h
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include "ErrHandler.h"
#include "Config.h"
#include "Helper.h"

#ifndef __Table__
#define __Table__

class Table {

public:
	/// \brief Class constructor,
	/// initialize member variables and label of class to print out with messages
	Table() : m_table(0), message("Table") {};

	/// \brief Class constructor, initalize generic data
	Table(std::vector< std::vector<GenericData> > table) : m_table(table), message("Table") {};

	/// \brief Class constructor, initalize histogram
	Table(TH1* hist);

	/// \brief Class destructor
	~Table() {};

	/// \brief Get number of rows
	int getNrows();

	/// \brief Get number of columns
	int getNcols();

	/// \brief Prepend new column 
	/// \param input input vector
	void prependCol(std::vector<GenericData> input);

	/// \brief Prepend new row
	/// \param input input vector
	void prependRow(std::vector<GenericData> input);

	/// \brief Append new column
	/// \param input input vector
	void appendCol(std::vector<GenericData> input);

	/// \brief Append new row
	/// \param input input vector
	void appendRow(std::vector<GenericData> input);

	/// \brief Insert new column
	/// \param col new column position
	/// \param input input vector
	void insertCol(int col, std::vector<GenericData> input);

	/// \brief Insert new row
	/// \param row new row position
	/// \param input input vector
	void insertRow(int row, std::vector<GenericData> input);

	/// \brief Print table
	/// \param width column width
	void print(int width);

	/// \brief Insert table to file
	/// \param file output stream file
	/// \param format output file format
	void print(std::ofstream& file, TString format = "text");
	
private:
	/// \brief Print right align format
	/// \param input input string
	/// \param width column width
	std::string printRight(std::string input, int width);

	/// \brief Print center align format
	/// \param input input string
	/// \param width column width
	std::string printCenter(std::string input, int width);

	std::vector< std::vector<GenericData> > m_table; ///< table of generic data
	ErrHandler message;  ///< label of class to print out with message
};

#endif
