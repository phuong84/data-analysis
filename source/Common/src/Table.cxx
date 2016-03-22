/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     25-12-2015
 *
 * \file     Table.cxx
 * 
 */

#include "Table.h"

/***************************************************************************/
/**
 * This method converts a histogram into a table
 */
Table::Table(TH1* hist) : message("Table")
{
	int nrows = hist->GetNbinsX();
	int ncols = hist->GetNbinsY();
	for(int j = 1; j <= ncols; ++j) {
		std::vector<GenericData> array;
		for(int i = 1; i <= nrows; ++i) {
			array.push_back(hist->GetBinContent(i,j));
		}
		m_table.push_back(array);
	}
}

/***************************************************************************/
/**
 * This method returns number of rows
 */
int Table::getNrows()
{
	return (int)m_table[0].size();
}

/***************************************************************************/
/**
 * This method returns number of columns
 */
int Table::getNcols()
{
	return (int)m_table.size();
}

/***************************************************************************/
/**
 * This method prepends a new column to table
 */
void Table::prependCol(std::vector<GenericData> input)
{
	if(m_table[0].size() != input.size()) {
		ERROR("Size difference, cannot prepend new column to table.");
		return;
	}
	m_table.insert(m_table.begin(),input);
}

/***************************************************************************/
/**
 * This method prepends a new row to table
 */
void Table::prependRow(std::vector<GenericData> input)
{
	if(m_table.size() != input.size()) {
		ERROR("Size difference, cannot prepend new row to table.");
		return;
	}
	for(int i = 0; i < (int)m_table.size(); ++i)
		m_table[i].insert(m_table[i].begin(),input[i]);
}

/***************************************************************************/
/**
 * This method appends a new column to table
 */
void Table::appendCol(std::vector<GenericData> input)
{
	if(m_table[0].size() != input.size()) {
		ERROR("Size difference, cannot append new column to table.");
		return;
	}
	m_table.push_back(input);
}

/***************************************************************************/
/**
 * This method appends a new row to table
 */
void Table::appendRow(std::vector<GenericData> input)
{
	if(m_table.size() != input.size()) {
		ERROR("Size difference, cannot append new row to table.");
		return;
	}
	for(int i = 0; i < (int)m_table.size(); ++i)
		m_table[i].push_back(input[i]);
}

/***************************************************************************/
/**
 * This method inserts a new column to table at specific position
 */
void Table::insertCol(int col, std::vector<GenericData> input)
{
	if(m_table[0].size() != input.size()) {
		ERROR("Size difference, cannot insert new column to table.");
		return;
	}
	m_table.insert(m_table.begin(), col, input);
}

/***************************************************************************/
/**
 * This method inserts a new row to table at specific position
 */
void Table::insertRow(int row, std::vector<GenericData> input)
{
	if(m_table.size() != input.size()) {
		ERROR("Size difference, cannot insert new row to table.");
		return;
	}
	for(int i = 0; i < (int)m_table.size(); ++i)
		m_table[i].insert(m_table[i].begin(), row, input[i]);
}

/***************************************************************************/
/**
 * This method prints table to monitor
 */
void Table::print(int width)
{
	int ncols = (int)m_table.size();
	int nrows = (int)m_table[0].size();
	for(int i = 0; i < nrows; ++i) {
		std::cout << " | ";
		for(int j = 0; j < ncols; ++j) {
			if(i == 0)
				std::cout << printCenter(m_table[j][i],width) << " | ";
			else
				std::cout << printRight(m_table[j][i],width) << " | ";
		}
		std::cout << std::endl;
		if(i == 0) std::cout << std::string(width*ncols + 2*ncols, '-') << std::endl;
	}
}

/***************************************************************************/
/**
 * This method prints table to file
 */
void Table::print(std::ofstream& file, TString format)
{
/*  TString title, Xtitle, Ytitle, Ztitle;
  double x, y, z;
  int N = 1;
  if(type == "latex") {
    file << "\\begin{tabular} {c|c|c";
    for (int i = 0; i < N; i++) 
      file << "|c";
    file << "}" << std::endl << "\\hline \\hline" << std::endl;
  }
  if(type == "latex")
    file << hist->GetXaxis()->GetTitle() << " & " << hist->GetYaxis()->GetTitle() << " & " << hist->GetZaxis()->GetTitle();
  else
    file << hist->GetXaxis()->GetTitle() << " \t" << hist->GetYaxis()->GetTitle() << " \t" << hist->GetZaxis()->GetTitle();
  for (int i = 0; i < N; i++) {
    //infile >> title;
    //if(type == "latex") 
    //  file << " & " << title;
    //else
    //  file << " \t" << title;
  }
  if(type == "latex") 
    file << " \\\\" << std::endl << "\\hline";
  //while(infile >> x >> y >> z) {
    if(type == "latex") 
      file << std::endl << std::ios::fixed << x << " & " << y << " & " << z;
    else
      file << std::endl << std::ios::fixed << x << " \t" << y << " \t" << z;
    for (int i = 0; i < N; i++) {
      //binx = hist[i]->GetXaxis()->FindBin(x);
      //biny = hist[i]->GetYaxis()->FindBin(y);
      //binz = hist[i]->GetZaxis()->FindBin(z);
      //if(type == "latex")
	//file << " & " << std::scientific << hist[i]->GetBinContent(binx,biny,binz);
      //else
	//file << " \t" << std::scientific << hist[i]->GetBinContent(binx,biny,binz);
    }
    if(type == "latex") 
      file << " \\\\";
    //}
  if(type == "latex") 
    file << std::endl << "\\hline \\hline" << std::endl << "\\end{tabular}";
*/
}

/***************************************************************************/
/**
 * This method aligns printing data to right handside
 */
std::string Table::printRight(std::string input, int width)
{
	std::stringstream ss;
	ss << std::fixed << std::right;
	ss.fill(' ');
	ss.width(width);
	ss << input;
	return ss.str();
}

/***************************************************************************/
/**
 * This method aligns printing data to center
 */
std::string Table::printCenter(std::string input, int width)
{
	std::stringstream ss, spaces;
	int padding = width - input.size();
	for(int i = 0; i < padding/2; ++i)
		spaces << " ";
	ss << spaces.str() << input << spaces.str();
	if(padding > 0 && padding%2 != 0)
		ss << " ";
	return ss.str();
}


