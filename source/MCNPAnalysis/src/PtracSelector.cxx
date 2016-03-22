/***************************************************************************
 Class  : PtracSelector  
 File   : PtracSelector.cxx
 Author : Dang Nguyen Phuong <dnphuong1984@gmail.com>
 Date   : 30-03-2015    
 ***************************************************************************/

#define PtracSelector_cxx
#include "PtracSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

/***************************************************************************/

int PtracSelector::event_selection()
{
	return 1;
}

/***************************************************************************/

void PtracSelector::Loop(TFile* outfile)
{
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;	

	initialHistos();	
	outfile->cd();

	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		if(jentry%100==0) 
			std::cout << "Entry " << jentry << std::endl;
		
		if(event_selection()) {
			fillHistos();
		}
   	}
	writeHistos();
}

/***************************************************************************/

void PtracSelector::initialHistos()
{
	TFile* file = TFile::Open(m_filename);
	TTree* tree = (TTree*)file->Get("PTRAC_Tree");
	double min, max;
	
	TH1F* h_nps = (TH1F*)file->Get("NumberOfHistory");
	std::cout << h_nps->GetBinContent(1) << std::endl; 
	h_NPS = new TH1F("NPS", "NPS", (int)h_nps->GetBinContent(1), 0, h_nps->GetBinContent(1)+1);

 	h_InitialEvent = new TH1F("InitialEvent", "InitialEvent", 10, 0, 10000);
	h_NextEvent = new TH1F("NextEvent", "NextEvent", 10, 0, 10000);
	h_Node = new TH1F("Node", "Node", 10, 0, 10);
	h_SourceType = new TH1F("SourceType", "SourceType", 100, 0, 100);
	h_ZZAAA = new TH1F("ZZAAA", "ZZAAA", 100, 0, 100000);
	h_ReactionType = new TH1F("ReactionType", "ReactionType", 900, 0, 900);
	h_ClosestSurface = new TH1F("ClosestSurface", "ClosestSurface", 1000, 0, 1000);
	h_AngleToSurface = new TH1F("AngleToSurface", "AngleToSurface", 190, 0, 190);
	h_TerminationType = new TH1F("TerminationType", "TerminationType", 13, 0, 13);
	h_BranchNumber = new TH1F("BranchNumber", "BranchNumber", 13, 0, 13);
	h_ParticleType = new TH1F("ParticleType", "ParticleType", 5, 0, 5);
	h_CellNumber = new TH1F("CellNumber", "CellNumber", 1000, 0, 1000);
	h_MaterialNumber = new TH1F("MaterialNumber", "MaterialNumber", 100, 0, 100);
	h_Type = new TH1F("Type", "Type", 5, 1000, 6000);

	min = tree->GetMinimum("NumberOfCollision");  max = tree->GetMaximum("NumberOfCollision");
	h_NumberOfCollision = new TH1F("NumberOfCollision", "NumberOfCollision", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("X");  max = tree->GetMaximum("X");
	h_X = new TH1F("X", "X", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("Y");  max = tree->GetMaximum("Y");
 	h_Y = new TH1F("Y", "Y", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("Z");  max = tree->GetMaximum("Z");
	h_Z = new TH1F("Z", "Z", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("U");  max = tree->GetMaximum("U");
	h_U = new TH1F("U", "U", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("V");  max = tree->GetMaximum("V");
	h_V = new TH1F("V", "V", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("W");  max = tree->GetMaximum("W");
	h_W = new TH1F("W", "W", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("Energy");  max = tree->GetMaximum("Energy");
	h_Energy = new TH1F("Energy", "Energy", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("Weight");  max = tree->GetMaximum("Weight");
	h_Weight = new TH1F("Weight", "Weight", 100, min, max+(max-min)/100.);
	min = tree->GetMinimum("Time");  max = tree->GetMaximum("Time");
	h_Time = new TH1F("Time", "Time", 100, min, max+(max-min)/100.);

}

/***************************************************************************/

void PtracSelector::fillHistos()
{
	h_NPS->Fill(NPS);
 	h_InitialEvent->Fill(InitialEvent);
	h_NextEvent->Fill(NextEvent);
	h_Node->Fill(Node);
	h_SourceType->Fill(SourceType);
	h_ZZAAA->Fill(ZZAAA);
	h_ReactionType->Fill(ReactionType);
	h_ClosestSurface->Fill(ClosestSurface);
	h_AngleToSurface->Fill(AngleToSurface);
	h_TerminationType->Fill(TerminationType);
	h_BranchNumber->Fill(BranchNumber);
	h_ParticleType->Fill(ParticleType);
	h_CellNumber->Fill(CellNumber);
	h_MaterialNumber->Fill(MaterialNumber);
	h_Type->Fill(Type);
	h_NumberOfCollision->Fill(NumberOfCollision);
	h_X->Fill(X);
 	h_Y->Fill(Y);
	h_Z->Fill(Z);
	h_U->Fill(U);
	h_V->Fill(V);
	h_W->Fill(W);
	h_Energy->Fill(Energy);
	h_Weight->Fill(Weight);
	h_Time->Fill(Time);
}

/***************************************************************************/

void PtracSelector::writeHistos()
{
	h_NPS->Write();
 	h_InitialEvent->Write();
	h_NextEvent->Write();
	h_Node->Write();
	h_SourceType->Write();
	h_ZZAAA->Write();
	h_ReactionType->Write();
	h_ClosestSurface->Write();
	h_AngleToSurface->Write();
	h_TerminationType->Write();
	h_BranchNumber->Write();
	h_ParticleType->Write();
	h_CellNumber->Write();
	h_MaterialNumber->Write();
	h_Type->Write();
	h_NumberOfCollision->Write();
	h_X->Write();
 	h_Y->Write();
	h_Z->Write();
	h_U->Write();
	h_V->Write();
	h_W->Write();
	h_Energy->Write();
	h_Weight->Write();
	h_Time->Write();
}
