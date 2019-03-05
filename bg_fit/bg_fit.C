#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"
#include "../headers/functions.h"

double four2Dgaus(double *x, double *par){
  return bivargaus(x, par) + bivargaus(x, &par[6]) + bivargaus(x, &par[12]) + bivargaus(x, &par[18]);
}

double three2Dgaus(double *x, double *par){
  return bivargaus(x, par) + bivargaus(x, &par[6]) + bivargaus(x, &par[12]);
}

void bg_fit(int kin){
  //Load Runs
  TString He3dat = "../runlists/"; He3dat += "He3_kin"; He3dat += kin; He3dat += ".dat";

  TString He3list = gGet_InputFile_Var(He3dat ,2);

  vector<Int_t> He3vec = gGet_RunNoChain(He3list);

  TChain *He3 = new TChain("T");

  for(int i=0; i<He3vec.size(); i++){
    TChain *tmp = LoadRun(He3vec[i]);
    He3->Add(tmp);
    delete tmp;
  }

  //Build cut
  TCut cut = ACC(kin==16?1:0) + PID_noep(kin==16?1:0) + W2(kin==16?1:0) + EC(kin==16?1:0);

  TString EP1 = (kin==16) ? "R.ps.e/R.tr.p[0]/1000." : "L.prl1.e/L.tr.p[0]/1000.";
  TString EP2 = (kin==16) ? "R.sh.e/R.tr.p[0]/1000." : "L.prl2.e/L.tr.p[0]/1000.";

  TH2D *ep_plot = new TH2D("ep_plot","E1/P:E2/P for He3",150,0,1.5,150,0,1.5);
  He3->Draw(Form("%s:%s>>ep_plot",EP1.Data(),EP2.Data()),cut,"colz");

  TF2 *gaus3 = new TF2("gaus3",three2Dgaus,0,1.5,0,1.5,18);
  gaus3->SetParameter(1,0.35);
  gaus3->SetParameter(3,0.65);
  gaus3->SetParameter(7,0.1);
  gaus3->SetParameter(9,0.1);
  gaus3->SetParameter(13,0.05);
  gaus3->SetParameter(15,0.05);

  ep_plot->Fit("gaus3","LR");
}
