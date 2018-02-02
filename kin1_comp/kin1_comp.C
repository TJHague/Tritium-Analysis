//The goal of this script is to compare the Left and Right arm at Kin1
//Author: Tyler Hague
//
//Jan 30 2018 - Began writing outline via comments

#include "TRI_Tools.h"

void kin1_comp(TString L_run="", TString R_run=""){
  //Load in runs using method in Electron_Count.C
  //Create a TChain for each arm
  if(L_run==""){
    cout << "What is the left arm run list file? ";
    cin >> L_run;
  }
  if(R_run==""{
    cout << "What is the right arm run list file? ";
    cin >> R_run;
  }

  TString L_list = gGet_InputFile_Var(L_run,0);
  TStrgin R_list = gGet_InputFile_Var(R_run,0);

  TChain* L_t = (TChain*) gGetTree(gGet_RunNoChain(L_list),"T");
  TChain* R_t = (TChain*) gGetTree(gGet_RunNoChain(R_list),"T");

  //Calculate the charge for each arm
  Double_t R_Q, L_Q, R_I, L_I, L_updated, R_updated;
  L_t->SetBranchAddress("LeftBCM.charge_dnew",&L_Q);
  R_t->SetBranchAddress("RightBCM.charge_dnew",&R_Q);
  L_t->SetBranchAddress("LeftBCM.current_dnew",&L_I);
  R_t->SetBranchAddress("RightBCM.current_dnew",&R_I);
  L_t->SetBranchAddress("LeftBCM.isrenewed",&L_updated);
  R_t->SetBranchAddress("RightBCM.isrenewed",&R_updated);

  Int_t L_events = L_t->GetEntries();
  Int_t R_events = R_t->GetEntries();

  Double_t L_charge = 0.;
  Double_t R_charge = 0.;

  for(int i=0;i<L_events;i++){
    L_t->GetEntry(i);
    if(L_updated && L_I>9.){
      L_charge += L_Q;
    }
  }
  for(int i=0;i<R_events;i++){
    R_t->GetEntry(i);
    if(R_updated && R_I>9.){
      R_charge += R_Q;
    }
  }

  L_norm = 1./(L_charge*0.0003299);
  R_norm = 1./(R_charge*0.0003299);
  //Set up the cuts
  TString L_cut = "LeftBCM.current_dnew>9.";
  L_cut += "&&L.cer.asum_c>2000";
  L_cut += "&&((L.prl1.e+L.prl2.e)/(1000.*L.tr.p[0]))>0.7";
  L_cut += "&&L.tr.n=1";
  L_cut += "&&((DL.evtypebits>>2)&1)";
  TString R_cut = "RightBCM.current_dnew>9.";
  R_cut += "&&R.cer.asum_c>2000";
  R_cut += "&&((R.ps.e+R.sh.e)/(1000.*R.tr.p[0]))>0.7";
  R_cut += "&&R.tr.n=1";
  R_cut += "&&((DR.evtypebits>>5)&1)";

  //Plot x for both arms and look for any overlap
  L_t->Draw("EKLc.x_bj>>hL(40,0.1,0.3)",L_cut.Data(),"");
  R_t->Draw("EKRc.x_bj>>hR(40,0.1,0.3)",R_cut.Data(),"SAME");

  //Normalize the charge
  hL->Scale(L_norm);
  hR->Scale(R_norm);
}
