/**
 * \defgroup Common Common Tools
 *
 * \brief    Common analysis tools
 *
 * This module contains all classes which are considered for general 
 * usage, not for any specific analysis purpose. These tools can be
 * used independently outside the scope of this package. 
 * 
 * \class    ErrHandler
 * \ingroup  Common
 *
 * \brief    Error and message handling
 *
 * This class prints out information, warning and error messages
 * during program running time. There are 5 levels of information 
 * print out, depends on the value of debug level variable:
 *
 *  - default - only print out information messages
 *
 *  -   0     - print out error messages
 *
 *  -   1     - print out error and warning messages
 *
 *  -   2     - print out error, warning and info messages
 *
 *  -   3     - print out error, warning, info and debug messages
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     ErrHandler.h
 * 
 */

#include <iostream>
#include <iomanip>
#include <TString.h>

/// Check operation system (true if Windows and false if Linux)
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__CYGWIN__)
	#define WINSYS false
#else
	#define WINSYS true
#endif

/// Macro to call message method
#define MESSAGE(msg) message.message(msg)
/// Macro to call info method
#define INFO(msg)    message.info(msg)
/// Macro to call warn method
#define WARN(msg)    message.warn(msg)
/// Macro to call error method
#define ERROR(msg)   message.error(msg)
/// Macro to call debug method
#define DEBUG(msg)   message.debug(msg,__LINE__)

/// level of message print out
extern int debug_level;


#ifndef __ErrHandler__
#define __ErrHandler__

class ErrHandler {

public:
	/// \brief Class constructor,
	/// initialize label of class to print out with messages
	ErrHandler(TString label) : m_label(label) {};
	
	/// \brief Class destructor
	~ErrHandler() {};
	
	/// \brief Method for printing out error message
	/// \param message to print out
	void error(TString message);
	
	/// \brief Method for printing out warning message
	/// \param message to print out
	void warn(TString message);
	
	/// \brief Method for printing out info message
	/// \param message to print out
	void info(TString message);
	
	/// \brief Method for printing out formal message
	/// \param message to print out
	void message(TString message);
	
	/// \brief Method for printing out debug message
	/// \param message to print out
	/// \param line line number in source file where calling method
	void debug(TString message, int line);

private:
	TString m_label; ///< Label of class to print out with message
};

#endif
