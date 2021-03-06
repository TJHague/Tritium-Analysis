/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

#include "../headers/cuts.h"
#include "../headers/corrections.h"

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void emctmp(Int_t kin, Int_t arm=0){
  //Load runs
  TString He3dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; He3dat += "He3_kin"; He3dat += kin; He3dat += ".dat";
  TString D2dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; D2dat  += "D2_kin";  D2dat  += kin; D2dat  += ".dat";
  TString He3list = gGet_InputFile_Var(He3dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  cout << He3list << endl;
  vector<Int_t> He3vec = gGet_RunNoChain(He3list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t He3charge = 0;
  Double_t D2charge  = 0;
  TH1D *He3full = new TH1D("He3full","Full Kinematic Helium-3 Yield" ,50,0,1);
  TH1D *D2full  = new TH1D("D2full" ,"Full Kinematic Deuterium Yield",50,0,1);
  TH1D *EMCfull = new TH1D("EMCfull","Full Kinematic EMC Ratio"      ,50,0,1);

  TFile *f = new TFile(Form("kin%d.root",kin),"RECREATE");
  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(unsigned int i=0; i<He3vec.size(); i++){
    cout << "on run " << He3vec[i] << endl;
    TChain* T = LoadRun(He3vec[i],"T");

    TString name = Form("He3part_%d",He3vec[i]);
    TH1D *He3part = new TH1D(name.Data(),"Partial Kinematic Helium-3 Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, p, ph, th, dp, z, x_bj, n;
    Int_t Iev=0;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    //PID variables
    T->SetBranchAddress("L.cer.asum_c",&cer);
    T->SetBranchAddress("L.prl1.e",&prl1);
    T->SetBranchAddress("L.prl2.e",&prl2);
    T->SetBranchAddress("L.tr.p",&p);
    T->SetBranchAddress("L.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("L.tr.tg_ph",&ph);
    T->SetBranchAddress("L.tr.tg_th",&th);
    T->SetBranchAddress("L.tr.tg_dp",&dp);
    T->SetBranchAddress("rpl.z",&z);

    T->SetBranchAddress("EKLx.x_bj",&x_bj);

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;

    for(Int_t j = 0; j < events; j++){
      T->GetEntry(j);
      cout << "event " << j << endl;
      cout << "updated " << updated << endl;
      cout << "I " << I << endl;
      cout << "T2 " << T2 << endl;
      cout << "events " << events << endl;
      if(updated && I>0.){
        cout << "charge & current" << endl;
        He3charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        cout << "cer " << cer << endl;
        cout << "prl1 " << prl1 << endl;
        cout << "prl2 " << prl2 << endl;
        cout << "p " << p << endl;
        cout << "n " << n << endl;
        if((PID(cer, prl1, prl2, p, n, arm)==true)){
          cout << "pass PID" << endl;
          if((ACC(ph, th, dp, arm)==true)){
            cout << "pass ACC" << endl;
            if((EC(z, arm)==true)){
              cout << "pass z" << endl;
              He3part->Fill(x_bj);
              cout << "filled" << endl;
            }
          }
        }
        cout << "end of t2 section" << endl;
      }
      if(j==(events-1)){
        cout << "last event" << endl;
        trig_scal = T2s;
      }
    }
    cout << "got out" << endl;
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw(Form("EKLx.x_bj>>He3part_%d",He3vec[i]),PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"goff");
    He3part->Write();
    cout << "wrote" << endl;
    He3part->Scale(1./lt);
    He3part->Scale(1./He3Nuclei(avgI));
    cout << "scaled" << endl;
    He3full->Add(He3part);
    cout << "added" << endl;

/*    delete T;
    cout << "deleted T" << endl;
    delete He3part;
    cout << "deleted He3part" << endl;*/
  }

  //Deuterium Yield
  for(unsigned int i=0; i<D2vec.size(); i++){
    TChain* T = LoadRun(D2vec[i],"T");

    TString name = Form("D2part_%d",D2vec[i]);
    TH1D *D2part  = new TH1D(name.Data(),"Partial Kinematic Deuterium Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, p, ph, th, dp, z, x_bj, n;
    Int_t Iev = 0;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    //PID variables
    T->SetBranchAddress("L.cer.asum_c",&cer);
    T->SetBranchAddress("L.prl1.e",&prl1);
    T->SetBranchAddress("L.prl2.e",&prl2);
    T->SetBranchAddress("L.tr.p",&p);
    T->SetBranchAddress("L.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("L.tr.tg_ph",&ph);
    T->SetBranchAddress("L.tr.tg_th",&th);
    T->SetBranchAddress("L.tr.tg_dp",&dp);
    T->SetBranchAddress("rpl.z",&z);

    T->SetBranchAddress("EKLx.x_bj",&x_bj);

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;

    for(Int_t j = 0; j < events; j++){
      T->GetEntry(j);
      if(updated && I>0.){
        D2charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)){
          D2part->Fill(x_bj);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw(Form("EKLx.x_bj>>D2part_%d",D2vec[i]),PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"goff");
    D2part->Write();
    D2part->Scale(1./lt);
    D2part->Scale(1./D2Nuclei(avgI));
    D2full->Add(D2part);

    /*delete T;
    delete D2part;*/
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing
  He3full->Scale(1./He3charge);
  D2full ->Scale(1./D2charge);

  //Endcap Contamination
  He3full->Scale((1.-He3ECC(kin)));
  D2full ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3full->Scale(1./3.);
  D2full ->Scale(1./2.);

  //Bin by bin Positron Subtraction
  for(Int_t i=1; i<He3full->GetNbinsX()+1; i++){
    Double_t bin = He3full->GetBinContent(i);
    bin *= (1. - He3Positrons(He3full->GetBinCenter(i)));
    He3full->SetBinContent(i, bin);
  }
  for(Int_t i=1; i<D2full->GetNbinsX()+1; i++){
    Double_t bin = D2full->GetBinContent(i);
    bin *= (1. - D2Positrons(D2full->GetBinCenter(i)));
    D2full->SetBinContent(i, bin);
  }

  //EMC is He3/D
  EMCfull->Add(He3full);
  EMCfull->Divide(D2full);

  TCanvas *EMC = new TCanvas("EMC","Helium-3 EMC");
  EMCfull->Draw();

  EMCfull->Write();
}
