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
#include "../classes/yieldHistogram.h"
#include "../classes/yieldHistogram.cpp"

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void emc(Int_t kin, Int_t iter=0, Int_t arm=0){
  //Load runs
  TString set="";
  if(kin>=7&&arm==0){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
    }else if(iter==3 && kin==15){
      set = "_3rd";
    }else{
      cout << "1st or 2nd iteration of that kin?" << endl;
      exit(0);
    }
  }

  TString He3dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; He3dat += "He3_kin"; He3dat += kin; He3dat += set.Data(); He3dat += ".dat";
  TString D2dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; D2dat  += "D2_kin";  D2dat  += kin; D2dat  += set.Data(); D2dat  += ".dat";
  TString He3list = gGet_InputFile_Var(He3dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  //cout << He3list << endl;
  vector<Int_t> He3vec = gGet_RunNoChain(He3list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t He3charge = 0;
  Double_t D2charge  = 0;
  TH1D *He3full = new TH1D("He3full","Full Kinematic Helium-3 Yield" ,50,0,1);
  TH1D *D2full  = new TH1D("D2full" ,"Full Kinematic Deuterium Yield",50,0,1);
  He3full->Sumw2();
  D2full->Sumw2();
  //yieldHistogram *He3full = new yieldHistogram("Full Kinematic Helium-3 Yield" ,50,0,1);
  //yieldHistogram *D2full  = new yieldHistogram("Full Kinematic Deuterium Yield",50,0,1);
  //yieldHistogram *EMCfull = new yieldHistogram("Full Kinematic EMC Ratio"      ,50,0,1);

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<He3vec.size(); i++){
    //cout << "on run " << He3vec[i] << endl;
    TChain* T = LoadRun(He3vec[i],"T");

    yieldHistogram *He3part = new yieldHistogram("Partial Kinematic Helium-3 Yield",50,0,1);

    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2;
    //Double_t p[1] = {0};
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
    T->SetBranchAddress("EKLx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    //cout << events << endl;
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if((updated==1) && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          He3part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>He3part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    He3part->setLivetime(lt);
    //cout << "He3 Livetime: " << lt << endl;
    He3part->setAvgI(avgI);
    //cout << "He3 Nuclei: " << He3Nuclei(avgI) << endl;
    He3part->setCharge(charge);
    He3part->save(Form("He3run_histos/kin%d%s/%d.dat",kin,set.Data(),He3vec[i]));
    TH1D *tmp = He3part->getTH1(Form("He3_%d",He3vec[i]));
    tmp->Scale(1. / He3part->getLivetime());
    tmp->Scale(1. / He3Nuclei(He3part->getAvgI()));
    He3charge += He3part->getCharge();
    He3full->Add(tmp);

    delete tmp;
    delete T;
    delete He3part;
  }

  //Deuterium Yield
  for(Int_t i=0; i<D2vec.size(); i++){
    TChain* T = LoadRun(D2vec[i],"T");

    yieldHistogram *D2part  = new yieldHistogram("Partial Kinematic Deuterium Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, p, ph, th, dp, z, x_bj, Q2, n, W2;
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
    T->SetBranchAddress("EKLx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if(updated && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&(W2cut(W2)==true)){
          D2part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>D2part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    D2part->setLivetime(lt);
    //cout << "D2 Livetime: " << lt << endl;
    D2part->setAvgI(avgI);
    //cout << "D2 Nuclei: " << D2Nuclei(avgI) << endl;
    D2part->setCharge(charge);
    D2part->save(Form("D2run_histos/kin%d%s/%d.dat",kin,set.Data(),D2vec[i]));
    TH1D *tmp = D2part->getTH1(Form("D2_%d",D2vec[i]));
    tmp->Scale(1. / D2part->getLivetime());
    tmp->Scale(1. / D2Nuclei(D2part->getAvgI()));
    D2charge += D2part->getCharge();
    D2full->Add(tmp);

    delete tmp;
    delete T;
    delete D2part;
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing - now taken care of when converting yieldHistogram to TH1
  He3full->Scale(1./He3charge);
  D2full ->Scale(1./D2charge);

  //Endcap Contamination
  He3full->Scale((1.-He3ECC(kin)));
  D2full ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3full->Scale(1./3.);
  D2full ->Scale(1./2.);

  //Bin by bin Positron Subtraction
/*  vector<Double_t> He3x = He3full->getAvgx();
  vector<Double_t> D2x  = D2full->getAvgx();

  for(Int_t i = 0; i < He3x.size(); i++){
    He3full->scaleBin(i, He3Positrons(He3x[i]));
  }
  for(Int_t i = 0; i < D2x.size(); i++){
    D2full->scaleBin(i, D2Positrons(D2x[i]));
  }

  for(Int_t i = 0; i < 50; i++){
    He3full->scaleBin(i, He3Positrons((0.02 * i) + 0.01));
    D2full ->scaleBin(i, D2Positrons((0.02 * i) + 0.01));
  }*/
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

  //He3full->save(Form("He3run_histos/kin%d/full.dat", kin));
  //D2full ->save(Form("D2run_histos/kin%d/full.dat",  kin));

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),50,0,1);

  ratio->Sumw2();
  ratio->Add(He3full);
  ratio->Divide(D2full);

  TFile *f = new TFile(Form("fullKin/kin%d%s.root",kin,set.Data()),"RECREATE");
  ratio->Write();
  //ratio->Draw("P");

 
  //EMC is He3/D
  //EMCfull->Add(He3full);
  //EMCfull->Divide(D2full);

  //TCanvas *EMC = new TCanvas("EMC","Helium-3 EMC");
  //EMCfull->Draw();
  
  delete f;
  delete He3full;
  delete D2full;
  delete ratio;
}

void emcR(Int_t kin, Int_t iter=0, Int_t arm=1){
  //Load runs
  TString set="";
  if(kin>=7){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
    }else{
      cout << "1st or 2nd iteration of that kin?" << endl;
      exit(0);
    }
  }

  TString He3dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; He3dat += "He3_kin"; He3dat += kin; He3dat += set.Data(); He3dat += ".dat";
  TString D2dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; D2dat  += "D2_kin";  D2dat  += kin; D2dat  += set.Data(); D2dat  += ".dat";
  TString He3list = gGet_InputFile_Var(He3dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  //cout << He3list << endl;
  vector<Int_t> He3vec = gGet_RunNoChain(He3list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t He3charge = 0;
  Double_t D2charge  = 0;
  TH1D *He3full = new TH1D("He3full","Full Kinematic Helium-3 Yield" ,50,0,1);
  TH1D *D2full  = new TH1D("D2full" ,"Full Kinematic Deuterium Yield",50,0,1);
  He3full->Sumw2();
  D2full->Sumw2();
  //yieldHistogram *He3full = new yieldHistogram("Full Kinematic Helium-3 Yield" ,50,0,1);
  //yieldHistogram *D2full  = new yieldHistogram("Full Kinematic Deuterium Yield",50,0,1);
  //yieldHistogram *EMCfull = new yieldHistogram("Full Kinematic EMC Ratio"      ,50,0,1);

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<He3vec.size(); i++){
    //cout << "on run " << He3vec[i] << endl;
    TChain* T = LoadRun(He3vec[i],"T");

    yieldHistogram *He3part = new yieldHistogram("Partial Kinematic Helium-3 Yield",50,0,1);

    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2;
    //Double_t p[1] = {0};
    Int_t Iev=0;

    T->SetBranchAddress("RightBCM.charge_dnew",&Q);
    T->SetBranchAddress("RightBCM.current_dnew",&I);
    T->SetBranchAddress("RightBCM.isrenewed",&updated);
    T->SetBranchAddress("DR.bit5",&T2);
    T->SetBranchAddress("evRightT5",&T2s);

    //PID variables
    T->SetBranchAddress("R.cer.asum_c",&cer);
    T->SetBranchAddress("R.ps.e",&prl1);
    T->SetBranchAddress("R.sh.e",&prl2);
    T->SetBranchAddress("R.tr.p",&p);
    T->SetBranchAddress("R.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("R.tr.tg_ph",&ph);
    T->SetBranchAddress("R.tr.tg_th",&th);
    T->SetBranchAddress("R.tr.tg_dp",&dp);
    T->SetBranchAddress("rpr.z",&z);

    T->SetBranchAddress("EKRx.x_bj",&x_bj);
    T->SetBranchAddress("EKRx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKRx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    //cout << events << endl;
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if((updated==1) && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          He3part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>He3part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    He3part->setLivetime(lt);
    //cout << "He3 Livetime: " << lt << endl;
    He3part->setAvgI(avgI);
    //cout << "He3 Nuclei: " << He3Nuclei(avgI) << endl;
    He3part->setCharge(charge);
    He3part->save(Form("He3run_histos/kin%d%s/%d.dat",kin,set.Data(),He3vec[i]));
    TH1D *tmp = He3part->getTH1(Form("He3_%d",He3vec[i]));
    tmp->Scale(1. / He3part->getLivetime());
    tmp->Scale(1. / He3Nuclei(He3part->getAvgI()));
    He3charge += He3part->getCharge();
    He3full->Add(tmp);

    delete tmp;
    delete T;
    delete He3part;
  }

  //Deuterium Yield
  for(Int_t i=0; i<D2vec.size(); i++){
    TChain* T = LoadRun(D2vec[i],"T");

    yieldHistogram *D2part  = new yieldHistogram("Partial Kinematic Deuterium Yield",50,0,1);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, p, ph, th, dp, z, x_bj, Q2, n, W2;
    Int_t Iev = 0;
    T->SetBranchAddress("RightBCM.charge_dnew",&Q);
    T->SetBranchAddress("RightBCM.current_dnew",&I);
    T->SetBranchAddress("RightBCM.isrenewed",&updated);
    T->SetBranchAddress("DR.bit5",&T2);
    T->SetBranchAddress("evRightT5",&T2s);

    //PID variables
    T->SetBranchAddress("R.cer.asum_c",&cer);
    T->SetBranchAddress("R.ps.e",&prl1);
    T->SetBranchAddress("R.sh.e",&prl2);
    T->SetBranchAddress("R.tr.p",&p);
    T->SetBranchAddress("R.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("R.tr.tg_ph",&ph);
    T->SetBranchAddress("R.tr.tg_th",&th);
    T->SetBranchAddress("R.tr.tg_dp",&dp);
    T->SetBranchAddress("rpr.z",&z);

    T->SetBranchAddress("EKRx.x_bj",&x_bj);
    T->SetBranchAddress("EKRx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKRx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if(updated && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          D2part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>D2part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    D2part->setLivetime(lt);
    //cout << "D2 Livetime: " << lt << endl;
    D2part->setAvgI(avgI);
    //cout << "D2 Nuclei: " << D2Nuclei(avgI) << endl;
    D2part->setCharge(charge);
    D2part->save(Form("D2run_histos/kin%d%s/%d.dat",kin,set.Data(),D2vec[i]));
    TH1D *tmp = D2part->getTH1(Form("D2_%d",D2vec[i]));
    tmp->Scale(1. / D2part->getLivetime());
    tmp->Scale(1. / D2Nuclei(D2part->getAvgI()));
    D2charge += D2part->getCharge();
    D2full->Add(tmp);

    delete tmp;
    delete T;
    delete D2part;
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing - now taken care of when converting yieldHistogram to TH1
  He3full->Scale(1./He3charge);
  D2full ->Scale(1./D2charge);

  //Endcap Contamination
  He3full->Scale((1.-He3ECC(kin)));
  D2full ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3full->Scale(1./3.);
  D2full ->Scale(1./2.);

  //Bin by bin Positron Subtraction
/*  vector<Double_t> He3x = He3full->getAvgx();
  vector<Double_t> D2x  = D2full->getAvgx();

  for(Int_t i = 0; i < He3x.size(); i++){
    He3full->scaleBin(i, He3Positrons(He3x[i]));
  }
  for(Int_t i = 0; i < D2x.size(); i++){
    D2full->scaleBin(i, D2Positrons(D2x[i]));
  }

  for(Int_t i = 0; i < 50; i++){
    He3full->scaleBin(i, He3Positrons((0.02 * i) + 0.01));
    D2full ->scaleBin(i, D2Positrons((0.02 * i) + 0.01));
  }*/
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

  //He3full->save(Form("He3run_histos/kin%d/full.dat", kin));
  //D2full ->save(Form("D2run_histos/kin%d/full.dat",  kin));

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),50,0,1);

  ratio->Sumw2();
  ratio->Add(He3full);
  ratio->Divide(D2full);

  TFile *f = new TFile(Form("fullKin/kin%d%s.root",kin,set.Data()),"RECREATE");
  He3full->Write();
  D2full->Write();
  ratio->Write();
  //ratio->Draw("P");

 
  //EMC is He3/D
  //EMCfull->Add(He3full);
  //EMCfull->Divide(D2full);

  //TCanvas *EMC = new TCanvas("EMC","Helium-3 EMC");
  //EMCfull->Draw();

  delete f;
  delete He3full;
  delete D2full;
  delete ratio;
}


/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

void kin_clean(int kin){
  if(kin<=5){
    /* Kin 0-5 only had 1 iteration
     * Move the data to a new histogram and omit the first and last bin
     */

    TFile *f = new TFile(Form("fullKin/kin%d.root",kin));
    TH1D *emc = (TH1D*) gDirectory->Get("emc");
    emc->SetDirectory(0);
    delete f;

    bool first = true;
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),0,1);
    for(int i = 1; i <= emc->GetNbinsX(); i++){
      double c = emc->GetBinContent(i);
      if(c != 0){
        if(!first){
          double d = emc->GetBinContent(i+1);
          if(d != 0){
            cleaned->SetBinContent(i,c);
            cleaned->SetBinError(i,emc->GetBinError(i));
          }else{
            break;
          }
        }else{
          first = false;
        }
      }
    }
    TFile *out = new TFile(Form("cleanedKin/kin%d.root",kin),"RECREATE");
    cleaned->Write();

    delete emc;
    delete cleaned;
    delete out;
  }else if(kin<=16 && kin!=15){
    /* Kin 7-15 had 2 iterations
     * First combine them into one histogram with a weighted average
     * Then, move that to a new histogram and omit the first and last bin
     */
    
    //Combine iterations
    TFile *f = new TFile(Form("fullKin/kin%d_1st.root",kin));
    TH1D *i1 = (TH1D*) gDirectory->Get("emc");
    i1->SetDirectory(0);
    delete f;

    TFile *g = new TFile(Form("fullKin/kin%d_2nd.root",kin));
    TH1D *i2 = (TH1D*) gDirectory->Get("emc");
    i2->SetDirectory(0);
    delete g;

    if(i1->GetNbinsX() != i2->GetNbinsX()){
      cout << "Plots to be combined do not have the same number of bins." << endl;
      cout << "Make sure they are plotted the same way before using this." << endl;
      return;
    }

    TH1D *emc = new TH1D("combined","He-3 EMC Ratio",i1->GetNbinsX(),0,1);

    for(int i = 1; i <= i1->GetNbinsX(); i++){
      if(i1->GetBinContent(i)!=0 || i2->GetBinContent(i)!=0){
        double c1 = i1->GetBinContent(i);
        double c2 = i2->GetBinContent(i);
        double e1 = i1->GetBinError(i);
        double e2 = i2->GetBinError(i);
        emc->SetBinContent(i,((c1*e2*e2)+(c2*e1*e1))/((e1*e1)+(e2*e2)));
        emc->SetBinError(i,TMath::Sqrt(1./((1./(e1*e1))+(1./(e2*e2)))));
      }
    }

    //Remove first and last bin
    bool first = true;
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),0,1);
    for(int i = 1; i <= emc->GetNbinsX(); i++){
      double c = emc->GetBinContent(i);
      if(c != 0){
        if(!first){
          double d = emc->GetBinContent(i+1);
          if(d != 0){
            cleaned->SetBinContent(i,c);
            cleaned->SetBinError(i,emc->GetBinError(i));
          }else{
            break;
          }
        }else{
          first = false;
        }
      }
    }
    TFile *out = new TFile(Form("cleanedKin/kin%d.root",kin),"RECREATE");
    cleaned->Write();

    delete emc;
    delete cleaned;
    delete out;
  
  }else if(kin==15){
    /* Kin 15 had 3 iterations
     * First combine them into one histogram with a weighted average
     * Then, move that to a new histogram and omit the first and last bin
     */

    //Combine iterations
    TFile *f = new TFile(Form("fullKin/kin%d_1st.root",kin));
    TH1D *i1 = (TH1D*) gDirectory->Get("emc");
    i1->SetDirectory(0);
    delete f;

    TFile *g = new TFile(Form("fullKin/kin%d_2nd.root",kin));
    TH1D *i2 = (TH1D*) gDirectory->Get("emc");
    i2->SetDirectory(0);
    delete g;

    TFile *h = new TFile(Form("fullKin/kin%d_3rd.root",kin));
    TH1D *i3 = (TH1D*) gDirectory->Get("emc");
    i3->SetDirectory(0);
    delete h;


    if(i1->GetNbinsX() != i2->GetNbinsX() || i1->GetNbinsX() != i3->GetNbinsX()){
      cout << "Plots to be combined do not have the same number of bins." << endl;
      cout << "Make sure they are plotted the same way before using this." << endl;
      return;
    }

    TH1D *emc = new TH1D("combined","He-3 EMC Ratio",i1->GetNbinsX(),0,1);

    for(int i = 1; i <= i1->GetNbinsX(); i++){
      if(i1->GetBinContent(i) != 0 || i2->GetBinContent(i) != 0 || i3->GetBinContent(i) != 0){
        double c1 = i1->GetBinContent(i);
        double c2 = i2->GetBinContent(i);
        double c3 = i3->GetBinContent(i);
        double e1 = i1->GetBinError(i);
        double e2 = i2->GetBinError(i);
        double e3 = i3->GetBinError(i);
        emc->SetBinContent(i,((c1*e2*e2*e3*e3)+(c2*e1*e1*e3*e3)+(c3*e1*e1*e2*e2))/((e1*e1*e2*e2)+(e1*e1*e3*e3)+(e2*e2*e3*e3)));
        emc->SetBinError(i,TMath::Sqrt(1./((1./(e1*e1))+(1./(e2*e2))+(1./(e3*e3)))));
      }
    }

    bool first = true;
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),0,1);
    for(int i = 1; i <= emc->GetNbinsX(); i++){
      double c = emc->GetBinContent(i);
      if(c != 0){
        if(!first){
          double d = emc->GetBinContent(i+1);
          if(d != 0){
            cleaned->SetBinContent(i,c);
            cleaned->SetBinError(i,emc->GetBinError(i));
          }else{
            break;
          }
        }else{
          first = false;
        }
      }
    }
    TFile *out = new TFile(Form("cleanedKin/kin%d.root",kin),"RECREATE");
    cleaned->Write();

    delete emc;
    delete cleaned;
    delete out;

  }
}



/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

void merge_all(){
  
  TH1D *hist[12];

  //Combine iterations
  TFile *a = new TFile("cleanedKin/kin0.root");
  hist[0] = (TH1D*) gDirectory->Get("emc");
  hist[0]->SetDirectory(0);
  delete a;

  TFile *b = new TFile("cleanedKin/kin1.root");
  hist[1] = (TH1D*) gDirectory->Get("emc");
  hist[1]->SetDirectory(0);
  delete b;

  TFile *c = new TFile("cleanedKin/kin2.root");
  hist[2] = (TH1D*) gDirectory->Get("emc");
  hist[2]->SetDirectory(0);
  delete c;

  TFile *d = new TFile("cleanedKin/kin3.root");
  hist[3] = (TH1D*) gDirectory->Get("emc");
  hist[3]->SetDirectory(0);
  delete d;

  TFile *e = new TFile("cleanedKin/kin4.root");
  hist[4] = (TH1D*) gDirectory->Get("emc");
  hist[4]->SetDirectory(0);
  delete e;

  TFile *f = new TFile("cleanedKin/kin5.root");
  hist[5] = (TH1D*) gDirectory->Get("emc");
  hist[5]->SetDirectory(0);
  delete f;

  TFile *g = new TFile("cleanedKin/kin7.root");
  hist[6] = (TH1D*) gDirectory->Get("emc");
  hist[6]->SetDirectory(0);
  delete g;

  TFile *h = new TFile("cleanedKin/kin9.root");
  hist[7] = (TH1D*) gDirectory->Get("emc");
  hist[7]->SetDirectory(0);
  delete h;

  TFile *k = new TFile("cleanedKin/kin11.root");
  hist[8] = (TH1D*) gDirectory->Get("emc");
  hist[8]->SetDirectory(0);
  delete k;

  TFile *l = new TFile("cleanedKin/kin13.root");
  hist[9] = (TH1D*) gDirectory->Get("emc");
  hist[9]->SetDirectory(0);
  delete l;

  TFile *m = new TFile("cleanedKin/kin15.root");
  hist[10] = (TH1D*) gDirectory->Get("emc");
  hist[10]->SetDirectory(0);
  delete m;

  TFile *n = new TFile("cleanedKin/kin16.root");
  hist[11] = (TH1D*) gDirectory->Get("emc");
  hist[11]->SetDirectory(0);
  delete n;

  if((hist[0]->GetNbinsX() != hist[1]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[2]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[3]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[4]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[5]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[6]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[7]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[8]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[9]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[10]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[11]->GetNbinsX())){
    cout << "Plots to be combined do not have the same number of bins." << endl;
    cout << "Make sure they are plotted the same way before using this." << endl;
    return;
  }

  TH1D *emc = new TH1D("emc","He-3 EMC Ratio",hist[0]->GetNbinsX(),0,1);

  for(int i = 1; i <= hist[0]->GetNbinsX(); i++){
    double c = 0;
    double e = 0;
    for(int j = 0; j <= 11; j++){
      double c1 = hist[j]->GetBinContent(i);
      if(c1 != 0){
        double e1 = hist[j]->GetBinError(i);
        c += c1/(e1*e1);
        e += 1./(e1*e1);
      }
    }
    if(c != 0){
      c /= e;
      e = 1./TMath::Sqrt(e);
      e = TMath::Sqrt((e*e)+(c*0.005*c*0.005));
      emc->SetBinContent(i,c);
      emc->SetBinError(i,e);
    }
  }

  gStyle->SetErrorX(0.);
  emc->SetAxisRange(0.9,1.3,"Y");

  TFile *out = new TFile("He3EMC.root","RECREATE");
  emc->Write();

  delete emc;
  delete out;

}
