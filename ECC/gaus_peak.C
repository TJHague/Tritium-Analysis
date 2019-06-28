#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"

void gaus_peak(int kin){
  //Load Runs
  TString EMdat = "./runlists/"; EMdat += "H3_kin"; EMdat += kin; EMdat += ".dat";

  TString EMlist = gGet_InputFile_Var(EMdat ,2);

  vector<Int_t> EMvec = gGet_RunNoChain(EMlist);

  TChain *em = new TChain("T");

  for(int i=0; i<EMvec.size(); i++){
    TChain *tmp = LoadRun(EMvec[i]);
    em->Add(tmp);
    delete tmp;
  }

  //Plot
  //Construct the cut
  TCut cut = ACC(kin==16?1:0) + PID(kin==16?1:0) + W2(kin==16?1:0);
  cut += (kin==16) ? "abs(rpr.z)<.18" : "abs(rpl.z)<.18";

  TH1D *z = new TH1D(Form("z_kin%d",kin),Form("Target Z for Kinematic %d",kin),180,-.18,.18);
  em->Draw((kin==16) ? Form("rpr.z>>z_kin%d",kin) : Form("rpl.z>>z_kin%d",kin) ,cut);

  TF1 *upfit = new TF1("upfit","gaus",-.18,-.08);
  TF1 *downfit = new TF1("downfit","gaus",.08,.18);

  z->Fit("upfit","L","SAME",-.18,-.08);
  z->Fit("downfit","L","SAME",.08,.18);
}
