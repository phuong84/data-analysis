/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     Plotter.cxx
 * 
 */

#include "Plotter.h"


/// Initialize color map
Plotter::ColorMap Plotter::colormap_ = init_map();

/***************************************************************************/
/**
 * This method looks for all necessary options for plot making from
 * the config file, and then calls #setPlotStyle() method.
 */
void Plotter::setStyle(Config* config)
{
	m_titleX      = config->get("X Title"      , "");
	m_titleY      = config->get("Y Title"      , "");
	m_titleZ      = config->get("Z Title"      , "");
	m_unit        = config->get("Unit"         , "Arbitrary unit");
	m_minBin      = config->get("Min Bin"      , 1);
	m_maxBin      = config->get("Max Bin"      , 0);
	m_plotStyle   = config->get("Plot Style"   , "L");
	m_markerSize  = config->get("Marker Size"  , 0.);
	m_markerStyle = config->get("Marker Style" , 20);
	m_lineWidth   = config->get("Line Width"   , 1);
	m_lineStyle   = config->get("Line Style"   , 1);
	m_logscale    = config->get("Log Scale"    , false);
	m_grid	      = config->get("Grid"         , false);
	m_smooth      = config->get("Smooth"       , false);
	m_ratio       = config->get("Show Ratio"   , false);
	m_ratioStyle  = config->get("Ratio Style"  , "ep");
	m_nContour    = config->get("Number of Contours" , 10);
	m_plotDir     = config->get("Plot Folder"  , "plots");
	m_plotFormat  = config->get("Plot Format"  , "pdf");
	
	TString color = config->get("Plot Color"   , "Blue");
	m_plotColor   = colormap_[color];
	setPlotStyle();
}

/***************************************************************************/
/**
 * This method loops on all elements in histogram vector and creates single plots
 * by calling #makePlot() 
 */
void Plotter::makeSinglePlot(std::vector<TH1*> hist) 
{
	for (std::vector<TH1*>::iterator hist_it = hist.begin(); hist_it != hist.end(); ++hist_it) {
		TCanvas* canvas = new TCanvas("", "", 0, 0, 700, 500);
		canvas->cd();
		makePlot(canvas, (*hist_it), m_plotColor, true);
		TString filename = m_plotDir+"/"+(*hist_it)->GetName()+"."+m_plotFormat;
		MESSAGE("Creating "+filename);
		canvas->SaveAs(filename);
		delete canvas;
	}
}

/***************************************************************************/
/**
 * This method loops on all elements in histogram vector and creates 1 comparison plot
 * by calling #makePlot()
 */
void Plotter::makeComparisonPlot(std::vector<TH1*> hist, std::vector<TString> title) 
{
	Color color[] = {Blue, Red, Pink, Cyan, Green, Purple, Yellow, DarkGreen, Grey, LightGreen,
					DarkRed, Brown, Black};

	double ratioPadRatio       = 0.;
	double padScaling          = 1.;
	double ratioPadScaling     = 2.;
	double additionalTopMargin = 0.;

	TCanvas* canvas = new TCanvas("", "", 0, 0, 700, 500);
	if(m_ratio) {
		canvas->SetWindowSize(700,700);
		ratioPadRatio	= 0.25;
		padScaling 	= 1. / (1. - ratioPadRatio) / 8. * 6.;
		ratioPadScaling	= (1. / ratioPadRatio) / 8. * 6.;
	}
	TPad* pad      = new TPad("pad", "pad", 0., ratioPadRatio, 1., 1.);
	TPad* ratioPad = new TPad("ratioPad", "ratioPad", 0., 0., 1., ratioPadRatio);
	additionalTopMargin = pad->GetTopMargin() * (padScaling - 1);
	pad->SetTopMargin(padScaling * pad->GetTopMargin());
	if(m_ratio)  pad->SetBottomMargin(0.);	
	pad->Draw();
	pad->cd();

	double x1 = 0.75;
	double y1 = 0.90 - 0.04*hist.size() - additionalTopMargin; //0.75 - additionalTopMargin;
	double x2 = 0.90;
	double y2 = 0.90 - additionalTopMargin;
	y1 = y2 - (y2 - y1) * padScaling;
	//TLegend *legend = new TLegend(0.75, 0.9-0.04*hist.size(), 0.9, 0.9);
	TLegend * legend = new TLegend(x1, y1, x2, y2);
	legend->SetBorderSize(0);
	legend->SetTextSize(0.035);
	legend->SetFillColor(0);
	
	int icol = 0;
	bool isFirst = true;
	TString filename;
	DEBUG( TString::Format( "Size of histogram vector = %d", (int)hist.size() ) );
	hist[0]->Draw();
	for (size_t i = 0; i < hist.size(); ++i) {
		DEBUG( TString::Format( "Histogram name : %s", hist[i]->GetName() ) );
		if(isFirst)  
			filename = hist[i]->GetName();
		makePlot(canvas, hist[i], color[icol], isFirst);
		if(title.size() == hist.size())
			legend->AddEntry( hist[i], title[i] );
		else
			legend->AddEntry( hist[i], hist[i]->GetTitle() );
		isFirst = false;
		if(color[icol] == Black)
			icol = 0;
		else
			++icol;
	}
	legend->Draw();

	gPad->RedrawAxis();
	
	if(m_ratio) {
		canvas->cd();
		ratioPad->SetTopMargin(.015);
		//ratioPad->SetBottomMargin(ratioPadScaling * ratioPad->GetBottomMargin());
		ratioPad->SetBottomMargin(1./ratioPadRatio * ratioPad->GetBottomMargin());
		ratioPad->Draw();
		ratioPad->cd();

		int nBins = hist[1]->GetNbinsX();
		double xLowerLimit = hist[1]->GetBinLowEdge(1);
		double xUpperLimit = hist[1]->GetBinLowEdge(nBins) + hist[1]->GetBinWidth(nBins);
		hist[1]->GetXaxis()->SetTitleSize(hist[1]->GetTitleSize("x") * 1./ratioPadRatio * 0.9);
		hist[1]->GetXaxis()->SetLabelSize(hist[1]->GetLabelSize("x") * 1./ratioPadRatio * 0.8);
		hist[1]->GetYaxis()->SetTitleSize(hist[1]->GetTitleSize("y") * 1./ratioPadRatio * 0.7);
		hist[1]->GetYaxis()->SetLabelSize(hist[1]->GetLabelSize("y") * 1./ratioPadRatio * 0.6);
		hist[1]->GetXaxis()->SetTickLength(hist[1]->GetXaxis()->GetTickLength() * ratioPadScaling);
		hist[1]->GetYaxis()->SetNdivisions(305,kTRUE);

		hist[1]->GetYaxis()->SetTitle("Ratio");
		hist[1]->GetYaxis()->CenterTitle(1);
		hist[1]->GetYaxis()->SetTitleOffset(0.3);

		for(size_t i = 1; i < hist.size(); ++i) {
			hist[i]->Divide(hist[0]);
			if(i==1)
				hist[i]->Draw(m_ratioStyle);
			else
				hist[i]->Draw("same"+m_ratioStyle);
		}
		//hist[0]->GetYaxis()->SetRangeUser(0.5,1.5);
	
		TLine * line = new TLine(xLowerLimit, 1., xUpperLimit, 1.);
		line->SetLineColor(kBlue);
		line->Draw();
	
		ratioPad->SetGridy();
		gPad->RedrawAxis();
	}

	filename = m_plotDir+"/"+filename+"_incl."+m_plotFormat;
	MESSAGE("Creating "+filename);
	canvas->SaveAs(filename);
	delete canvas;
}

/***************************************************************************/
/**
 * This method creates a single histogram plot by calling #makePlot()
 */
void Plotter::makeHistPlot(TH1* hist, TString prefix)
{
	TCanvas* canvas = new TCanvas("", "", 0, 0, 700, 500);
	makePlot(canvas, hist, m_plotColor, true);
	TString filename = m_plotDir+"/";
	if(prefix == "")
		filename += TString::Format("%s",hist->GetName())+"."+m_plotFormat;
	else
		filename += prefix+"_"+TString::Format("%s",hist->GetName())+"."+m_plotFormat;
	MESSAGE("Creating "+filename);
	canvas->SaveAs(filename);
	delete canvas;
}

/***************************************************************************/
/**
 * This method creates multiple histogram plots by calling #makePlot()
 */
void Plotter::makeHistPlots(std::vector<TH1*> hist)
{
  for (size_t i = 0; i < hist.size(); ++i) { 
	 //std::vector<TH1*>::iterator hist_it = hist.begin(); hist_it != hist.end(); ++hist_it) {
		TCanvas* canvas = new TCanvas("", "", 0, 0, 700, 500);
		makePlot(canvas, hist[i], m_plotColor, true);
		TH3F* h = (TH3F*)(hist[0])->Clone();
		h->Draw();
		TString filename = m_plotDir+"/"+TString::Format("%s",hist[i]->GetName())+"."+m_plotFormat;
		MESSAGE("Creating "+filename);
		canvas->SaveAs(filename);
		delete canvas;
	}
}


/***************************************************************************/
/**
 * This method creates all histogram plots from a file 
 * by calling #makePlot()
 */
void Plotter::makeFilePlot(TFile* file)
{
	//gROOT->SetBatch(true);
	TObject* obj; 
	TIter next(file->GetListOfKeys());
	TKey* key;
	while ((key=(TKey*)next())) {
		obj = key->ReadObj();
		if (obj->InheritsFrom("TH1")) {
			TCanvas* canvas = new TCanvas("", "", 0, 0, 700, 500);
			canvas->cd();
			makePlot(canvas, (TH1*)obj, m_plotColor, true);
			TString filename = m_plotDir+"/"+TString(obj->GetName())+"."+m_plotFormat;
			MESSAGE("Creating "+filename);
			canvas->SaveAs(filename);
			delete canvas;
		} else
			WARN("No histogram found!");
	}
}


/***************************************************************************/
/**
 * This method export a histogram to a text file
 */
void Plotter::exportHist2Text(TH1* hist, TString prefix)
{
	TString filename = m_plotDir+"/";
	if(prefix == "")
		filename += TString::Format("%s",hist->GetName())+".txt";
	else
		filename += prefix+"_"+TString::Format("%s",hist->GetName())+".txt";
	std::ofstream outfile;
	outfile.open(filename);
	
	if(outfile.is_open()) {
		for(int k = 0; k < hist->GetNbinsZ(); ++k)
			for(int j = 0; j < hist->GetNbinsY(); ++j)
				for(int i = 0; i < hist->GetNbinsX(); ++i)
					outfile << hist->GetBinContent(i+1,j+1,k+1) << std::endl;
		outfile.close();
	} else
		ERROR("Unable to open file '"+filename+"'!"); 
	
	return;
}

/***************************************************************************/
/**
 * This method creates a plot from an input histograms, checking if the histogram
 * is the first one or not, if not add option "same"
 */
void Plotter::makePlot(TCanvas* canvas, TH1* hist, Color color, bool isFirst) 
{
  	hist->SetLineColor(color);   
	hist->SetLineWidth(m_lineWidth);
	hist->SetLineStyle(m_lineStyle);
	hist->SetMarkerColor(color);   
	hist->SetMarkerStyle(m_markerStyle);
	hist->SetMarkerSize(m_markerSize);
	hist->GetXaxis()->SetTitle(m_titleX);
	hist->GetYaxis()->SetTitle(m_titleY);
	hist->GetYaxis()->SetNdivisions(505);

	if(dynamic_cast<TH2*>(hist)) {
		hist->SetEntries(1);
		hist->GetZaxis()->SetTitle(m_titleZ);
		hist->GetZaxis()->SetTitleOffset(1.1);
		hist->GetZaxis()->CenterTitle(1);
	}

	if (m_smooth)	hist->Smooth();
	if(isFirst)		hist->DrawCopy(m_plotStyle);
	else			hist->DrawCopy("same" + m_plotStyle);
	if(m_grid)		canvas->SetGrid();
	if(m_logscale) {
		gPad->SetLogy();
		gPad->RedrawAxis();
	}

	return;
}

/***************************************************************************/
/**
 * This method sets necessary options for plotting
 */
void Plotter::setPlotStyle() 
{
	//gErrorIgnoreLevel = kPrint, kInfo, kWarning, kError, kBreak, kSysError, kFatal;
	gErrorIgnoreLevel = kWarning;

	// use plain black on white colors
	Int_t icol = 0; // WHITE
	gStyle->SetFrameBorderMode(icol);
	gStyle->SetFrameFillColor(icol);
	gStyle->SetCanvasBorderMode(icol);
	gStyle->SetCanvasColor(icol);
	gStyle->SetPadBorderMode(icol);
	gStyle->SetPadColor(icol);
	gStyle->SetStatColor(icol);

	// set the paper & margin sizes
	gStyle->SetPaperSize(20,26);

	// set margin sizes
	gStyle->SetPadTopMargin(0.05);
	if(m_plotStyle.Contains("col"))
		gStyle->SetPadRightMargin(0.12);
	else
		gStyle->SetPadRightMargin(0.05);
	gStyle->SetPadBottomMargin(0.08);
	gStyle->SetPadLeftMargin(0.08);

	// set title offsets (for axis label)
	gStyle->SetTitleXOffset(1.);
	gStyle->SetTitleYOffset(1.);

	// use large fonts
	//Int_t font=72; // Helvetica italics
	Int_t font=42; // Helvetica
	Double_t tsize=0.035;
	gStyle->SetTextFont(font);

	//gStyle->SetTextSize(tsize);
	gStyle->SetLabelFont(font,"xyz");
	gStyle->SetTitleFont(font,"xyz");
	gStyle->SetLabelSize(tsize,"xyz");
	//gStyle->SetTitleSize(tsize*1.2,"xyz");

	// use bold lines and markers
	gStyle->SetMarkerStyle(20);
	gStyle->SetMarkerSize(0.4);
	gStyle->SetMarkerColor(kBlue);
	//gStyle->SetHistLineWidth(2.);
	gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

	// do not display any of the standard histogram decorations
	gStyle->SetOptTitle(0);
	//gStyle->SetOptStat(1111);
	gStyle->SetOptStat(0);
	//gStyle->SetOptFit(1111); 
	gStyle->SetOptFit(0);

	// put tick marks on top and RHS of plots
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

}


