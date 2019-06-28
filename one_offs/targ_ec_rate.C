#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"

void targ_ec_rate(int kin, TString targ_s, double insig, double outsig){
  if(kin>5 && targ_s=="H1"){
    exit(0);
  }

  //Load Runs
  TString dat = "../runlists/"; dat += targ_s; dat += "_kin"; dat += kin; dat += ".dat";

  TString list  = gGet_InputFile_Var(dat ,2);

  vector<Int_t> vec  = gGet_RunNoChain(list);

  TChain *targ = new TChain("T");

  double q = 0;

  Double_t bcm_gain = 0.00033518;
  Double_t bcm_offset = 0.0217;
  Double_t clock_rate = 103700.0;

  cout << "Load " << targ_s << endl;
  for(int i=0; i<vec.size(); i++){
    cout << vec[i] << endl;
    TChain *tmp = LoadRun(vec[i]);
    targ->Add(tmp);
    double dn;
    double cl;
    if(kin==16){
      tmp->SetBranchAddress("evRightdnew",&dn);
    }else{
      tmp->SetBranchAddress("evLeftdnew",&dn);
    }
    tmp->SetBranchAddress("V1495ClockCount",&cl);
    tmp->GetEntry(tmp->GetEntries()-1);
    q += (dn*bcm_gain) + (cl*bcm_offset/clock_rate);
    delete tmp;
    cout << q << endl;
  }

  //First construct the cut

  TCut cut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0) + Trig2(kin==16?1:0);
  TCut emcut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0) + Trig2(kin==16?1:0);

  //Get Normalization value
  TF1 *upfit = new TF1("emupfit","gaus",-.18,-.08);
  TF1 *downfit = new TF1("emdownfit","gaus",.08,.18);

  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *z = new TH1D(Form("%sz_kin%d",targ_s.Data(),kin),Form("%s Z for Kinematic %d",targ_s.Data(),kin),360,-.18,.18);
  targ->Draw((kin==16) ? Form("rpr.z>>%sz_kin%d",targ_s.Data(),kin) : Form("rpl.z>>%sz_kin%d",targ_s.Data(),kin) ,emcut);

  z->Fit("emdownfit","L","SAME",.12,.15);
  z->Fit("emupfit","L","SAME",-.13,-.1);
  
  //Can get standard dev with GetParameter(2) and center with GetParameter(1)
  double up_center = upfit->GetParameter(1);
  double up_stdev = upfit->GetParameter(2);
  double down_center = downfit->GetParameter(1);
  double down_stdev = downfit->GetParameter(2);

  double up_count = targ->GetEntries(cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,up_center,outsig*up_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,up_center,insig*up_stdev)));
  double down_count = targ->GetEntries(cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,down_center,insig*down_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,down_center,outsig*down_stdev)));

  up_count /= q;
  down_count /= q;

  ofstream f(Form("targ_rate/%s.txt",targ_s.Data()),ofstream::app);

  f << "Kin " << kin << endl;
  f << "Upstream:   " << up_count << endl;
  f << "Downstream: " << down_count << endl << endl;

  f.close();

  /*TFile *f = new TFile(outfolder+"/em_dm.root","UPDATE");
  em_up->Write();
  em_down->Write();
  f->Close();*/
}
