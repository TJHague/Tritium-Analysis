/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */
#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

vector<double> kin_pcount(Int_t, TString);

void time_norm(TString targ){
  double time[17];
  double x1[17];
  double x2[17];
  double ele[17];

  TGraph* x1n = new TGraph(12);
  x1n->SetTitle("Time Normalized x1");
  TGraph* x2n = new TGraph(12);
  x2n->SetTitle("Time Normalized x2");
  TGraph* elen = new TGraph(12);
  elen->SetTitle("Time Normalized Electrons");

  int j = 0;
  for(int i=0; i<=16; i++){
    if(i%2!=0||i<=5||i==16){
      vector<double> tmp = kin_pcount(i, targ);
      time[i] = tmp[0];
      x1[i] = tmp[1];
      x2[i] = tmp[2];
      ele[i] = tmp[3];

      x1n->SetPoint(j,i,x1[i]/time[i]);
      x2n->SetPoint(j,i,x2[i]/time[i]);
      elen->SetPoint(j,i,ele[i]/time[i]);
      j++;
    }else{
      time[i] = 0;
      x1[i] = 0;
      x2[i] = 0;
      ele[i] = 0;
    }
  }

  TCanvas* c1 = new TCanvas();
  TCanvas* c2 = new TCanvas();
  TCanvas* c3 = new TCanvas();
  c1->cd();
  x1n->Draw();
  c2->cd();
  x2n->Draw();
  c3->cd();
  elen->Draw();
}

vector<double> kin_pcount(Int_t kin, TString targ){

  TString dat = "/work/halla/triton/tjhague/data_qual/runlists/"; dat += targ; dat += "_kin"; dat += kin; dat += ".dat";
  TString list = gGet_InputFile_Var(dat,2);
  vector<Int_t> vec = gGet_RunNoChain(list);

  double time_tot = 0;
  double x1_tot = 0;
  double x2_tot = 0;
  double ele_tot = 0;

  /****************************************************************************
   * Here is where it needs to turn into a loop over each run.
   * Probably start with Helium-3 then go to Deuterium
   ****************************************************************************/
  TCut x1_cut, x2_cut, ele_cut;
  if(kin!=16){
    x1_cut = "DL.bit2==1&&(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000.<0.05&&L.cer.asum_c>3500&&L.cer.asum_c<4500";
    x2_cut = "DL.bit2==1&&(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000.>0.08&&(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000.<0.12&&L.cer.asum_c<1000";
    ele_cut = "DL.bit2==1&&L.cer.asum_c>2000&&(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000.>0.75";
  }else{
    x1_cut = "DR.bit5==1&&(R.ps.e+R.sh.e)/R.tr.p[0]/1000.<0.06";
    x2_cut = "DR.bit5==1&&(R.ps.e+R.sh.e)/R.tr.p[0]/1000.>0.06&&(R.ps.e+R.sh.e)/R.tr.p[0]/1000.<0.12";
    ele_cut = "DR.bit5==1&&R.cer.asum_c>2000&&(R.ps.e+R.sh.e)/R.tr.p[0]/1000.>0.75";
  }

  //Helium-3 Yield
  for(Int_t i=0; i<vec.size(); i++){
    TChain* T = LoadRun(vec[i],"T");

    Double_t clock;
    T->SetBranchAddress("V1495ClockCount",&clock);

    Int_t events = T->GetEntries();
    Int_t x1 = T->GetEntries(x1_cut); 
    Int_t x2 = T->GetEntries(x2_cut);
    Int_t ele = T->GetEntries(ele_cut);

    T->GetEntry(events-1);
    time_tot += clock;

    x1_tot += x1;
    x2_tot += x2;
    ele_tot += ele;

    delete T;
  }

  vector<double> ret;
  ret.push_back(time_tot);
  ret.push_back(x1_tot);
  ret.push_back(x2_tot);
  ret.push_back(ele_tot);

  return ret;
}
