/**
 * \class    PtracSelector
 * \ingroup  MCNPAnalysis
 *
 * \brief    Select event from PTRAC tree
 *
 * This class can be used to select MCNP event from a PTRAC tree. 
 * It is remodified from an auto generated ROOT file using \a 
 * TTree::MakeClass() method.
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     PtracSelector.h
 * 
 */

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 30 12:16:07 2015 by ROOT version 5.34/11
// from TTree PTRAC_Tree/PTRAC_Tree
// found on file: ptrac_test.root
//////////////////////////////////////////////////////////

#ifndef PtracSelector_h
#define PtracSelector_h

#include <iostream>
#include <fstream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include "ErrHandler.h"

class PtracSelector {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           NPS;
   Int_t           InitialEvent;
   Int_t           NextEvent;
   Int_t           Node;
   Int_t           SourceType;
   Int_t           ZZAAA;
   Int_t           ReactionType;
   Int_t           ClosestSurface;
   Int_t           AngleToSurface;
   Int_t           TerminationType;
   Int_t           BranchNumber;
   Int_t           ParticleType;
   Int_t           CellNumber;
   Int_t           MaterialNumber;
   Int_t           Type;
   Int_t           NumberOfCollision;
   Double_t        X;
   Double_t        Y;
   Double_t        Z;
   Double_t        U;
   Double_t        V;
   Double_t        W;
   Double_t        Energy;
   Double_t        Weight;
   Double_t        Time;

   // List of branches
   TBranch        *b_NPS;   //!
   TBranch        *b_InitialEvent;   //!
   TBranch        *b_NextEvent;   //!
   TBranch        *b_Node;   //!
   TBranch        *b_SourceType;   //!
   TBranch        *b_ZZAAA;   //!
   TBranch        *b_ReactionType;   //!
   TBranch        *b_ClosestSurface;   //!
   TBranch        *b_AngleToSurface;   //!
   TBranch        *b_TerminationType;   //!
   TBranch        *b_BranchNumber;   //!
   TBranch        *b_ParticleType;   //!
   TBranch        *b_CellNumber;   //!
   TBranch        *b_MaterialNumber;   //!
   TBranch        *b_Type;   //!
   TBranch        *b_NumberOfCollision;   //!
   TBranch        *b_X;   //!
   TBranch        *b_Y;   //!
   TBranch        *b_Z;   //!
   TBranch        *b_U;   //!
   TBranch        *b_V;   //!
   TBranch        *b_W;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_Time;   //!

   TH1F           *h_NPS;
   TH1F           *h_InitialEvent;
   TH1F           *h_NextEvent;
   TH1F           *h_Node;
   TH1F           *h_SourceType;
   TH1F           *h_ZZAAA;
   TH1F           *h_ReactionType;
   TH1F           *h_ClosestSurface;
   TH1F           *h_AngleToSurface;
   TH1F           *h_TerminationType;
   TH1F           *h_BranchNumber;
   TH1F           *h_ParticleType;
   TH1F           *h_CellNumber;
   TH1F           *h_MaterialNumber;
   TH1F           *h_Type;
   TH1F           *h_NumberOfCollision;
   TH1F           *h_X;
   TH1F           *h_Y;
   TH1F           *h_Z;
   TH1F           *h_U;
   TH1F           *h_V;
   TH1F           *h_W;
   TH1F           *h_Energy;
   TH1F           *h_Weight;
   TH1F           *h_Time;

   int event_selection(); //! Select event
   void initialHistos();  //! Initialize histograms
   void fillHistos();     //! Fill histograms
   void writeHistos();    //! Write out histograms

   /// \brief Class constructor, initialize filename
   PtracSelector(TString filename = "ptrac.root");
   /// \brief Class destructor
   virtual ~PtracSelector();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TFile *outfile);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

private:
   TString m_filename;  //! Name of output ROOT file
   ErrHandler message;  //! Label of class to print out with message

};

#endif

/***************************************************************************/

#ifdef PtracSelector_cxx

PtracSelector::PtracSelector(TString filename) : fChain(0), m_filename(filename), message("PtracSelector")
{
   TTree *tree = 0;
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
   if (!f || !f->IsOpen()) {
     f = new TFile(filename);
   }
   f->GetObject("PTRAC_Tree",tree);
   Init(tree);
}

/***************************************************************************/

PtracSelector::~PtracSelector()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PtracSelector::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

/***************************************************************************/

Long64_t PtracSelector::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

/***************************************************************************/

void PtracSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("NPS", &NPS, &b_NPS);
   fChain->SetBranchAddress("InitialEvent", &InitialEvent, &b_InitialEvent);
   fChain->SetBranchAddress("NextEvent", &NextEvent, &b_NextEvent);
   fChain->SetBranchAddress("Node", &Node, &b_Node);
   fChain->SetBranchAddress("SourceType", &SourceType, &b_SourceType);
   fChain->SetBranchAddress("ZZAAA", &ZZAAA, &b_ZZAAA);
   fChain->SetBranchAddress("ReactionType", &ReactionType, &b_ReactionType);
   fChain->SetBranchAddress("ClosestSurface", &ClosestSurface, &b_ClosestSurface);
   fChain->SetBranchAddress("AngleToSurface", &AngleToSurface, &b_AngleToSurface);
   fChain->SetBranchAddress("TerminationType", &TerminationType, &b_TerminationType);
   fChain->SetBranchAddress("BranchNumber", &BranchNumber, &b_BranchNumber);
   fChain->SetBranchAddress("ParticleType", &ParticleType, &b_ParticleType);
   fChain->SetBranchAddress("CellNumber", &CellNumber, &b_CellNumber);
   fChain->SetBranchAddress("MaterialNumber", &MaterialNumber, &b_MaterialNumber);
   fChain->SetBranchAddress("Type", &Type, &b_Type);
   fChain->SetBranchAddress("NumberOfCollision", &NumberOfCollision, &b_NumberOfCollision);
   fChain->SetBranchAddress("X", &X, &b_X);
   fChain->SetBranchAddress("Y", &Y, &b_Y);
   fChain->SetBranchAddress("Z", &Z, &b_Z);
   fChain->SetBranchAddress("U", &U, &b_U);
   fChain->SetBranchAddress("V", &V, &b_V);
   fChain->SetBranchAddress("W", &W, &b_W);
   fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("Time", &Time, &b_Time);
   Notify();
}

/***************************************************************************/

Bool_t PtracSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

/***************************************************************************/

void PtracSelector::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

/***************************************************************************/

Int_t PtracSelector::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

#endif // #ifdef PtracSelector_cxx
