/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     ErrHandler.cxx
 * 
 */

#include "ErrHandler.h"

/***************************************************************************/

void ErrHandler::error(TString message)
{
	if(debug_level >= 0) {
		if(WINSYS)	std::cout << ">>> ERROR ";
		else		std::cout << "\033[31mERROR \033[0m";
		std::cout << std::resetiosflags(std::ios::adjustfield);
		std::cout << std::setiosflags(std::ios::left);
		std::cout << std::setw(12) << m_label << " : " << message << std::endl;
	}
}

/***************************************************************************/

void ErrHandler::warn(TString message)
{
	if(debug_level >= 1) {
		if(WINSYS)	std::cout << ">>> WARN  ";
		else		std::cout << "\033[1;33mWARN  \033[0m";
		std::cout << std::resetiosflags(std::ios::adjustfield);
		std::cout << std::setiosflags(std::ios::left);
		std::cout << std::setw(12) << m_label << " : " << message << std::endl;
	}
}

/***************************************************************************/

void ErrHandler::info(TString message)
{
	if(debug_level >= 2) 
		this->message(message);
}

/***************************************************************************/

void ErrHandler::message(TString message)
{
	if(WINSYS)	std::cout << "    INFO  ";
	else		std::cout << "\033[32mINFO  \033[0m";
	std::cout << std::resetiosflags(std::ios::adjustfield);
	std::cout << std::setiosflags(std::ios::left);
	std::cout << std::setw(12) << m_label << " : " << message << std::endl;
}

/***************************************************************************/

void ErrHandler::debug(TString message, int line)
{
	if(debug_level >= 3) {
		if(WINSYS)	std::cout << "... DEBUG  ";
		else		std::cout << "\033[1;37mDEBUG \033[0m";
		std::cout << std::resetiosflags(std::ios::adjustfield);
		std::cout << std::setiosflags(std::ios::left);
		std::cout << std::setw(12) << m_label << " : " << message << "   @line " << line << std::endl;
	}
}

/***************************************************************************/
