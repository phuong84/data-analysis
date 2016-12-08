/**
 * \class    Plotter
 * \ingroup  Common
 *
 * \brief    Make plots
 *
 * This class can be used to create output plots and ROOT histograms. 
 * The histograms can be either 1-, 2- or 3-dimension. The class also
 * provides many options for adjusting the output plots (e.g. log scale,
 * smooth, axis labels,...).
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     Plotter.h
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <TError.h>
#include <TKey.h>
#include <TObject.h>
#include <TTree.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLine.h>
#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "ErrHandler.h"
#include "Config.h"

#ifndef __Plotter__
#define __Plotter__

class Plotter {

public:
	/// \brief Class constructor,
	/// initialize member variables and label of class to print out with messages
	Plotter() : m_logscale(0), m_smooth(0), m_grid(0), m_ratio(0), m_plotStyle(""), m_ratioStyle(""), m_plotFormat(""), m_plotDir(""),
			m_titleX(""), m_titleY(""), m_titleZ(""), m_unit(""), m_plotColor(Blue), m_markerSize(0.),
			m_minBin(0), m_maxBin(0), m_markerStyle(0), m_lineStyle(0), m_lineWidth(0), m_nContour(0),
			message("Plotter") {};
	/// \brief Class destructor.
	~Plotter() {};

	/// \brief Set configuration
	/// \param config pointer of Config object
	void setStyle(Config* config);

	/// \brief Create single histogam plot
	/// \param hist vector of histograms
	void makeSinglePlot(std::vector<TH1*> hist);

	/// \brief Create comparison histogam plot
	/// \param hist vector of histograms
	/// \param title vector of legends
	void makeComparisonPlot(std::vector<TH1*> hist, std::vector<TString> title = std::vector<TString>());

	/// \brief Create histogam plot
	/// \param hist histogram object 
	/// \param prefix prefix of output file
	void makeHistPlot(TH1* hist, TString prefix = "");

	/// \brief Create multiple histogam plot
	/// \param hist vector of histograms 
	void makeHistPlots(std::vector<TH1*> hist);

	/// \brief Create all histogams from a file
	/// \param file pointer to \a TFile object
	void makeFilePlot(TFile* file);
	
	/// \brief Export histogram to text file
	/// \param hist histogram to be exported
	/// \param prefix prefix of filename
	void exportHist2Text(TH1* hist, TString prefix);

	/// \brief List of color used in Plotter class
	enum Color {
		Blue       = 4,
		Red        = 2,
		Pink       = 6,
		Cyan       = 7,
		Green      = 8,
		Purple     = 9,
		Yellow     = 5,
		DarkGreen  = 32,
		Grey       = 40,
		LightGreen = 3,
		DarkRed    = 46,
		Brown      = 49,
		Black      = 1
	};
	typedef std::map<TString, Color> ColorMap;
	static ColorMap colormap_;
	
private:
	/// \brief Set plotting style
	void setPlotStyle();

	/// \brief Create plot
	/// \param canvas pointer to \a TCanvas object
	/// \param hist pointer to histogram object
	/// \param color histogram color
	/// \param isFirst check if the histogram is the first one
	void makePlot(TCanvas* canvas, TH1* hist, Color color = Blue, bool isFirst= true);

	/** plotting option */
	//@{
	bool m_logscale, m_smooth, m_grid, m_ratio;
	TString m_plotStyle, m_ratioStyle, m_plotFormat, m_plotDir, m_titleX, m_titleY, m_titleZ, m_unit;
	Color m_plotColor;
	float m_markerSize;
	int m_minBin, m_maxBin, m_markerStyle, m_lineStyle, m_lineWidth, m_nContour;
	//@}
	
	ErrHandler message;  ///< label of class to print out with message

	static ColorMap init_map() {
		ColorMap map;
		map["Blue"]       = Blue;
		map["Red"]        = Red;
		map["Pink"]       = Pink;
		map["Cyan"]       = Cyan;
		map["Green"]      = Green;
		map["Purple"]     = Purple;
		map["Yellow"]     = Yellow;
		map["DarkGreen"]  = DarkGreen;
		map["LightGreen"] = LightGreen;
		map["DarkRed"]    = DarkRed;
		map["Brown"]      = Brown;
		map["Black"]      = Black;
		return map;
	}
};

#endif
