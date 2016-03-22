/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     Config.cxx
 * 
 */

#include "../include/Config.h"

/***************************************************************************/
/**
 * This method opens the configuration file; return 1 if there is any error 
 * during openning, 0 if everything ok.
 * 
 * It loops all lines in the file, finds the position of charater ':' to 
 * determine (keyword, value) pair, both in string type, and store in \ref m_config
 * member.
 *
 * To comment a line in configuration file, use '#' at the beginning. 
 */
int Config::open(TString filename) 
{
	INFO("Reading configuration file '" + filename + "'");
	file.open (filename.Data(), std::ifstream::in);
	if (file.fail()) {
		ERROR("Cannot open file '"+filename+"'");
		return 1;
	}
	
	// Read config file
	TString str;
	while (!file.eof()) {
		str.ReadLine(file);
		if (str[0] == '#') continue;
		else {
			int i = str.First(':');
			TString substr1 = str(0, i);
			str.Remove(0,i+1);
			TString substr2 = str(0, str.Length());
			substr1.Remove(TString::kBoth,' ');
			substr2.Remove(TString::kBoth,' ');
			m_config.insert (std::pair<TString,TString>(substr1,substr2));
		}
	}
	file.close();
	return 0;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the string
 * value corresponding to the keyword defined via \a key argument. If there 
 * is no consistent keyword found in #m_config, the default value (\a def) 
 * will be used. 
 */
TString Config::get(TString key, const std::string& def)
{
	TString value = TString(def);
	std::map<TString,TString>::iterator it;
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			value = it->second;
	}
	value.Remove(TString::kBoth,' ');
	DEBUG("String type value: " + value);
	return value;
}

/***************************************************************************/
/**
 * This method calls #get(TString,const std::string&)
 */
TString Config::get(TString key, const char* def)
{
	return get(key,std::string(def));
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the integer
 * value corresponding to the keyword defined via \a key argument. If there 
 * is no consistent keyword found in #m_config, the default value (\a def) 
 * will be used. 
 */
int Config::get(TString key, int def)
{
	TString value = "";
	int number = def;
	std::map<TString,TString>::iterator it = m_config.begin();
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			value = it->second;
	}
	if (value != "")
		number = value.Atoi();
	DEBUG( TString::Format( "Integer type value:  %d", number ) );
	return number;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the double
 * value corresponding to the keyword defined via \a key argument. If there 
 * is no consistent keyword found in #m_config, the default value (\a def) 
 * will be used. 
 */
double Config::get(TString key, double def)
{
	TString value = "";
	double number = def;
	std::map<TString,TString>::iterator it = m_config.begin();
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			value = it->second;
	}
	if (value != "")
		number = value.Atof();
	DEBUG( TString::Format( "Double type value:  %lf", number ) );
	return number;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the boolean
 * value corresponding to the keyword defined via \a key argument. If there 
 * is no consistent keyword found in #m_config, the default value (\a def) 
 * will be used. 
 */
bool Config::get(TString key, bool def)
{
	TString value = "";
	bool number = def;
	std::map<TString,TString>::iterator it;
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			value = it->second;
	}
	DEBUG("Boolean type value: " + value);
	if (value == "true")
		number = true;
	else
		number = false;
	return number;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the string
 * vector corresponding to the keyword defined via \a key argument. The
 * deliminator \a delim is used to separate string into vector elements. 
 */
std::vector<TString> Config::getString(TString key, char delim)
{
	std::vector<TString> value;
	TString str;
	std::map<TString,TString>::iterator it;

	DEBUG( TString::Format( "Initial size of string values = %d", (int)value.size() ) );

	int i = 0;
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key) {
			str = it->second;
			i = 1;
		}
	}
	while(i > 0) {
		i = str.First(delim);
		if(i < 0) {
			str.Remove(TString::kBoth,' ');
			value.push_back(str);
			break;
		}
		TString substr = str(0, i);
		str.Remove(0,i+1);
		substr.Remove(TString::kBoth,' ');
		value.push_back(substr);
	}

	DEBUG( TString::Format( "Final size of string values = %d", (int)value.size() ) );
	return value;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the integer
 * vector corresponding to the keyword defined via \a key argument. The
 * deliminator \a delim is used to separate string into vector elements. 
 */
std::vector<int> Config::getInt(TString key, char delim)
{
	std::vector<int> value;
	TString str;
	std::map<TString,TString>::iterator it;
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			str = it->second;
	}
	//str.Remove(str.Length()-1,str.Length());
	int i = 1;
	while(i > 0) {
		i = str.First(delim);
		if(i < 0) {
			value.push_back(str.Atoi());
			break;
		}
		TString substr = str(0, i);
		str.Remove(0,i+1);
		value.push_back(substr.Atoi());
	}

	return value;
}

/***************************************************************************/
/**
 * This method loops on all elements of #m_config and return the double
 * vector corresponding to the keyword defined via \a key argument. The
 * deliminator \a delim is used to separate string into vector elements. 
 */
std::vector<double> Config::getDouble(TString key, char delim)
{
	std::vector<double> value;
	TString str;
	std::map<TString,TString>::iterator it;
	for (it=m_config.begin(); it!=m_config.end(); ++it) {
		if(it->first == key)
			str = it->second;
	}
	//str.Remove(str.Length()-1,str.Length());
	int i = 1;
	while(i > 0) {
		i = str.First(delim);
		if(i < 0) {
			value.push_back(str.Atof());
			break;
		}
		TString substr = str(0, i);
		str.Remove(0,i+1);
		value.push_back(substr.Atof());
	}

	return value;
}

