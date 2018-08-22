/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void emc(Int_t kin, Int_t arm=0){
  //Load runs
  TString He3dat = "~/work/replay_tritium/replay/scripts/Runlist/"; He3dat = "He3_kin"; He3dat += kin; He3dat += ".dat";
  TString D2dat  = "~/work/replay_tritium/replay/scripts/Runlist/"; D2dat  = "D2_kin";  D2dat  += kin; D2dat  += ".dat";
  TString He3list = gGet_InputFile_Var(He3dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  vector<Int_t> He3vec = gGet_RunNoChain(He3list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t He3charge = 0;
  Double_t D2charge  = 0;
  TH1F *He3full = new TH1F("He3full","Full Kinematic Helium-3 Yield" ,50,0,1);
  TH1F *D2full  = new TH1F("D2full" ,"Full Kinematic Deuterium Yield",50,0,1);
  TH1F *EMCfull = new TH1F("EMCfull","Full Kinematic EMC Ratio"      ,50,0,1);

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<He3vec.size(); i++){
    TChain* T = LoadRun(He3vec[i],"T");

    TH1F *He3part = new TH1F("He3part","Partial Kinematic Helium-3 Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;

    for(int i=0;i<events;i++){
      T->GetEntry(i);
      if(updated && I>0.){
        He3charge += Q;
      }
      if(T2==1){
        trig_rec++;
      }
      if(i==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    T->Draw("ELKx.x_bj>>He3part",PID+ACC,"");
    He3part->Scale(1./lt);
    He3full->Add(He3part);

    delete T;
    delete He3part;
  }

  //Deuterium Yield
  for(Int_t i=0; i<D2vec.size(); i++){
    TChain* T = LoadRun(D2vec[i],"T");

    TH1F *D2part  = new TH1F("D2part","Partial Kinematic Deuterium Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;

    for(int i=0;i<events;i++){
      T->GetEntry(i);
      if(updated && I>0.){
        D2charge += Q;
      }
      if(T2==1){
        trig_rec++;
      }
      if(i==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    T->Draw("ELKx.x_bj>>D2part",PID+ACC,"");
    D2part->Scale(1./lt);
    D2full->Add(D2part);

    delete T;
    delete D2part;
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing
  He3full->Scale(He3charge);
  D2full ->Scale(D2charge);

  //EMC is He3/D
  EMCfull->Add(He3full);
  EMCfull->Divide(D2full);
}
