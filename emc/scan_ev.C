/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

#include "TString.h"
#include <vector>

//Kin should be integer of kinematic
void scan_ev(Int_t kin, TString targ, TString folder, Int_t iter=0){
  Int_t arm=0;
  //Load runs
  TString set="";
  if(kin>=7){
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

  TString datfile = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; datfile += targ; datfile += "_kin"; datfile += kin; datfile += set.Data(); datfile += ".dat";
  TString runlist = gGet_InputFile_Var(datfile,2);
  vector<Int_t> vec = gGet_RunNoChain(runlist);
  int size = vec.size();
  int runvec[200]; 
  for(int i=0;i<size;i++){
    runvec[i] = vec[i];
  }

  /*ofstream run_info(Form("%s/run_info.csv",folder.Data()),ofstream::app);
  run_info << "Run" << "," << "Livetime" << "," << "Average Current" << "," << "Charge" << endl;*/
  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<size; i++){
    cout << runvec[i] << endl;
    TChain* T = LoadRun(runvec[i],"T");

    if(!gSystem->AccessPathName(TString::Format("/lustre/expphy/cache/halla/triton/prod/marathon/pass2/kin%d%s/tritium_%d.root",kin,set.Data(),run),kFileExists)){
      rootfile->Add(TString::Format("/lustre/expphy/cache/halla/triton/prod/marathon/pass2/kin%d%s/tritium_%d.root",kin,set.Data(),run));
      cout << "Added file: tritium_" << run << ".root" << endl;
    }else{
      cout << "Requested run has not been replayed. Exiting." << endl << endl;
      return;
    }

    while(!gSystem->AccessPathName(TString::Format("/lustre/expphy/cache/halla/triton/prod/marathon/pass2/kin%d%s/tritium_%d_%d.root",kin,set.Data(),run,i),kFileExists)){
      rootfile->Add(TString::Format("/lustre/expphy/cache/halla/triton/prod/marathon/pass2/kin%d%s/tritium_%d_%d.root",kin,set.Data(),run,i));
      cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
      i=i+1;
    }

    ofstream event_list(Form("%s/%d.csv",folder.Data(),runvec[i]),ofstream::trunc);
    event_list << "Cherenkov,Preshower,Shower,Momentum,nTracks,Phi,Theta,dp,Z,x,Q2,W2,x no EL,Q2 no EL,W2 no EL,charge,current,renewed,T2,T2scalar" << endl;
    //run_info << runvec[i] << "," << endl;

    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2, xnel, Q2nel, W2nel;
    //Int_t Iev=0;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    //T->SetBranchAddress("evLeftT2",&T2s);

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

    T->SetBranchAddress("EKLxe.x_bj",&x_bj);
    T->SetBranchAddress("EKLxe.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLxe.W2"  ,&W2  );
    
    T->SetBranchAddress("EKLx.x_bj",&xnel );
    T->SetBranchAddress("EKLx.Q2"  ,&Q2nel);
    T->SetBranchAddress("EKLx.W2"  ,&W2nel);

    Int_t events = T->GetEntries();
    //Double_t trig_rec  = 0;
    //Double_t trig_scal = 0;
    //Double_t charge    = 0;

    cout << "start of loop" << endl;
    for(Int_t j=0; j<events; j++){
/*      if(j%10000==0){
        cout << j << " working..." << endl;
        event_list.close();
        event_list.open(Form("%s/%d.csv",folder.Data(),runvec[i]),ofstream::trunc);
      }*/
      if(n>2){
        cout << n << endl;
      }
      T->GetEntry(j);
      if(j>=1225650&&j<1225660){
        cout << "j=" << j << endl;
        cout << cer << endl;
        cout << prl1 << endl;
        cout << prl2 << endl;
        cout << p << endl;
        cout << ph << endl;
        cout << th << endl;
        cout << dp << endl;
        cout << z << endl;
        cout << x_bj << endl;
        cout << Q2 << endl;
        cout << W2 << endl;
        cout << xnel << endl;
        cout << Q2nel << endl;
        cout << W2nel << endl;
        cout << Q << endl;
        cout << I << endl;
        cout << updated << endl;
        cout << T2 << endl;
        cout << T2s << endl;
      }
      event_list << double(cer);
      event_list << ",";
      event_list << double(prl1);
      event_list << ",";
      event_list << double(prl2);
      event_list << ",";
      event_list << double(p);
      event_list << ",";
      event_list << double(n);
      event_list << ",";
      event_list << double(ph);
      event_list << ",";
      event_list << double(th);
      event_list << ",";
      event_list << double(dp);
      event_list << ",";
      event_list << double(z);
      event_list << ",";
      event_list << double(x_bj);
      event_list << ",";
      event_list << double(Q2);
      event_list << ",";
      event_list << double(W2);
      event_list << ",";
      event_list << double(xnel);
      event_list << ",";
      event_list << double(Q2nel);
      event_list << ",";
      event_list << double(W2nel);
      event_list << ",";
      event_list << double(Q);
      event_list << ",";
      event_list << double(I);
      event_list << ",";
      event_list << double(updated);
      event_list << ",";
      event_list << double(T2);
      //event_list << ",";
      //event_list << double(T2s);
      event_list << endl;
    }
    cout << "perhaps???" << endl;
    
    event_list.close();
    cout << "past close" << endl;

    delete T;
    cout << "past delete" << endl;
  }
  //run_info.close();
}
