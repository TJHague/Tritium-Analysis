#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"

void ecc(int kin, TString target){
  if(kin>5 && target=="H1"){
    exit(0);
  }
  //Load Runs
  TString targdat = "../runlists/"; targdat += target; targdat += "_kin"; targdat += kin; targdat += ".dat";
  TString EMdat   = "../runlists/"; EMdat  += "EM_kin";                   EMdat   += kin;  EMdat  += ".dat";

  TString targlist = gGet_InputFile_Var(targdat,2);
  TString EMlist  = gGet_InputFile_Var(EMdat ,2);

  vector<Int_t> targvec = gGet_RunNoChain(targlist);
  vector<Int_t> EMvec  = gGet_RunNoChain(EMlist);

  TChain *targ = new TChain("T");
  TChain *em = new TChain("T");

  for(int i=0; i<targvec.size(); i++){
    TChain *tmp = LoadRun(targvec[i]);
    targ->Add(tmp);
    delete tmp;
  }
  for(int i=0; i<EMvec.size(); i++){
    TChain *tmp = LoadRun(EMvec[i]);
    em->Add(tmp);
    delete tmp;
  }

  //First construct the cut

  TCut cut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0);
  TCut emcut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0);

  //Get Normalization value
  TF1 *upfit = new TF1("upfit","gaus",-.18,-.08);
  TF1 *downfit = new TF1("downfit","gaus",.08,.18);
  TF1 *emupfit = new TF1("emupfit","gaus",-.18,-.08);
  TF1 *emdownfit = new TF1("emdownfit","gaus",.08,.18);

  TCanvas *c1 = new TCanvas();
  c1->cd(0);
  TH1D *z = new TH1D(Form("z_kin%d",kin),Form("Target Z for Kinematic %d",kin),180,-.18,.18);
  targ->Draw((kin==16) ? Form("rpr.z>>z_kin%d",kin) : Form("rpl.z>>z_kin%d",kin) ,cut);

  z->Fit("downfit","L","SAME",.12,.15);
  z->Fit("upfit","L","SAME",-.13,-.10);

  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *emz = new TH1D(Form("emz_kin%d",kin),Form("Empty Z for Kinematic %d",kin),180,-.18,.18);
  em->Draw((kin==16) ? Form("rpr.z>>emz_kin%d",kin) : Form("rpl.z>>emz_kin%d",kin) ,emcut);

  emz->Fit("emdownfit","L","SAME",.12,.15);
  emz->Fit("emupfit","L","SAME",-.13,-.10);

  //Can get standard dev with GetParameter(2) and center with GetParameter(1)
  double up_center = upfit->GetParameter(1);
  double up_stdev = upfit->GetParameter(2);
  double down_center = downfit->GetParameter(1);
  double down_stdev = downfit->GetParameter(2);
  double emup_center = emupfit->GetParameter(1);
  double emup_stdev = emupfit->GetParameter(2);
  double emdown_center = emdownfit->GetParameter(1);
  double emdown_stdev = emdownfit->GetParameter(2);
  double targ_up_count = targ->GetEntries(cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,up_center,up_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,up_center,up_stdev)));
  double em_up_count = em->GetEntries(emcut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,up_center,up_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,up_center,up_stdev)));
  double targ_down_count = targ->GetEntries(cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,down_center,down_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,down_center,down_stdev)));
  double em_down_count = em->GetEntries(emcut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,down_center,down_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,down_center,down_stdev)));

  ofstream fitout(target+".txt",ofstream::app);
  fitout << "Kinematic " << kin << endl;
  fitout << "For Target:" << endl;
  fitout << "Upstream " << up_center << " +/- " << up_stdev << endl;
  fitout << "Downstream " << down_center << " +/- " << down_stdev << endl;
  fitout << "For Empty:" << endl;
  fitout << "Upstream " << emup_center << " +/- " << emup_stdev << endl;
  fitout << "Downstream " << emdown_center << " +/- " << emdown_stdev << endl << endl;
  fitout << "Upstream Counts - Target: " << targ_up_count << "    Empty: " << em_up_count << endl;
  fitout << "Downstream Counts - Target: " << targ_down_count << "    Empty: " << em_down_count << endl;
  fitout.close();

  //Get the center of the upstream and downstream cuts
  //This will define where the contimanation comes from
  int it;
  if(kin<=5){
    it=kin;
  }else if(kin==16){
    it=11;
  }else{
    it=((kin-5)/2)+5;
  }
  double center = (up_cut[it] + down_cut[it])/2.;

  //Plot EM/Targ for z region of the kinematic
  TString x = (kin==16) ? "EKRxe.x_bj" : "EKLxe.x_bj";
  TString emx = (kin==16) ? "EKRx.x_bj" : "EKLx.x_bj";

  TH1D *ecc_plot = new TH1D(Form("ecc_kin%d",kin),Form("Kinematic %d Endcap Contamination",kin),33,0,0.99);
  TH1D *em_down  = new TH1D("em_down","Temporary EM Plot",33,0,0.99);
  TH1D *targ_plot = new TH1D("targtmp","Temporary Target Plot",33,0,0.99);

  TCanvas *c3 = new TCanvas();
  c3->cd(0);
  em->Draw(Form("%s>>ecc_kin%d",emx.Data(),kin),emcut + TCut(Form((kin==16) ? "rpr.z>%f" : "rpl.z>%f" ,up_cut[it])) + TCut(Form((kin==16) ? "rpr.z<%f" : "rpl.z<%f" ,center)));
  TCanvas *c4 = new TCanvas();
  c4->cd(0);
  em->Draw(Form("%s>>em_down",emx.Data()),emcut + TCut(Form((kin==16) ? "rpr.z>%f" : "rpl.z>%f" ,center)) + TCut(Form((kin==16) ? "rpr.z<%f" : "rpl.z<%f" ,down_cut[it])));
  TCanvas *c5 = new TCanvas();
  c5->cd(0);
  targ->Draw(x+">>targtmp",cut + EC(kin));

  ecc_plot->Scale(targ_up_count/em_up_count);
  em_down->Scale(targ_down_count/em_down_count);
  ecc_plot->Add(em_down);
  ecc_plot->Divide(targ_plot);

  TFile *f = new TFile(target+".root","UPDATE");
  ecc_plot->Write();
  f->Close();
}
