#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"

void gfit(int kin, TString target){
  if(kin>5 && target=="H1"){
    exit(0);
  }
  //Load Runs
  TString targdat = "../runlists/"; targdat += target; targdat += "_kin"; targdat += kin; targdat += ".dat";

  TString targlist = gGet_InputFile_Var(targdat,2);

  vector<Int_t> targvec = gGet_RunNoChain(targlist);

  TChain *targ = new TChain("T");

  for(int i=0; i<targvec.size(); i++){
    TChain *tmp = LoadRun(targvec[i]);
    targ->Add(tmp);
    delete tmp;
  }

  //First construct the cut

  TCut cut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0) + Trig2(kin==16?1:0);

  //Get Normalization value
  TF1 *upfit = new TF1("upfit","gaus",-.18,-.08);
  TF1 *downfit = new TF1("downfit","gaus",.08,.18);

  TCanvas *c1 = new TCanvas();
  c1->cd(0);
  TH1D *z = new TH1D(Form("z_kin%d",kin),Form("Target Z for Kinematic %d",kin),360,-.18,.18);
  targ->Draw((kin==16) ? Form("rpr.z>>z_kin%d",kin) : Form("rpl.z>>z_kin%d",kin) ,cut);

  z->Fit("downfit","L","SAME",.12,.15);
  z->Fit("upfit","L","SAME",-.13,-.1);
}
