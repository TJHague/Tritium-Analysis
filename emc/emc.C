/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"

void emc(){
  //Load runs
  //Todo:
  //  This won't actually load the runs becuase of the tape directory structure
  //  gGetTree may need rewritten/overloaded/or just write my own function
  TString list = gGet_InputFile_Var(runlist,2);
  vector<Int_t> runvec = gGet_RunNoChain(list);
  TChain* T = (TChain*) gGetTree(runvec,"T");
  TChain* E = (TChain*) gGetTree(runvec,"evLeft");
  
  //Calculate charge and live time
  //Todo:
  //  Make this work for both arms
  Double_t Q, I, updated, T2, T2s;
  T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
  T->SetBranchAddress("LeftBCM.current_dnew",&I);
  T->SetBranchAddress("LeftBCM.isrenewed",&updated);
  T->SetBranchAddress("DL.bit2",&T2);
  E->SetBranchAddress("evLeftT2",&T2s);

  Int_t events = T->GetEntries();
  Double_t charge = 0;
  Double_t trig_rec = 0;
  Double_t trig_scal = 0;

  for(int i=0;i<events;i++){
    T->GetEntry(i);
    if(updated && I>0.){
      charge += Q;
    }
    if(T2==1){
      trig_rec++;
    }
    if(i==(events-1)){
      trig_scal = T2s;
    }
  }
  Double_t lt = trig_rec/trig_scal;
}
