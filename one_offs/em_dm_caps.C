#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"
#include "../headers/cuts.h"

void em_dm_caps(int kin, double insig, double outsig, TString outfolder=""){
  //Load Runs
  TString EMdat   = "../runlists/"; EMdat  += "EM_kin";                   EMdat   += kin;  EMdat  += ".dat";
  TString DMdat   = "../runlists/"; DMdat  += "DM_kin";                   DMdat   += kin;  DMdat  += ".dat";

  TString EMlist  = gGet_InputFile_Var(EMdat ,2);
  TString DMlist  = gGet_InputFile_Var(DMdat ,2);

  vector<Int_t> EMvec  = gGet_RunNoChain(EMlist);
  vector<Int_t> DMvec  = gGet_RunNoChain(DMlist);

  TChain *em = new TChain("T");
  TChain *dm = new TChain("T");

  double emq = 0;
  double dmq = 0;

  Double_t bcm_gain = 0.00033518;
  Double_t bcm_offset = 0.0217;
  Double_t clock_rate = 103700.0;

  cout << "Load EM" << endl;
  for(int i=0; i<EMvec.size(); i++){
    cout << EMvec[i] << endl;
    TChain *tmp = LoadRun(EMvec[i]);
    em->Add(tmp);
    double dn;
    double cl;
    if(kin==16){
      tmp->SetBranchAddress("evRightdnew",&dn);
    }else{
      tmp->SetBranchAddress("evLeftdnew",&dn);
    }
    tmp->SetBranchAddress("V1495ClockCount",&cl);
    tmp->GetEntry(tmp->GetEntries()-1);
    emq += (dn*bcm_gain) + (cl*bcm_offset/clock_rate);
    delete tmp;
    cout << emq << endl;
  }
  cout << "Load DM" << endl;
  for(int i=0; i<DMvec.size(); i++){
    cout << DMvec[i] << endl;
    TChain *tmp = LoadRun(DMvec[i]);
    dm->Add(tmp);
    double dn;
    double cl;
    if(kin==16){
      tmp->SetBranchAddress("evRightdnew",&dn);
    }else{
      tmp->SetBranchAddress("evLeftdnew",&dn);
    }
    tmp->SetBranchAddress("V1495ClockCount",&cl);
    tmp->GetEntry(tmp->GetEntries()-1);
    dmq += (dn*bcm_gain) + (cl*bcm_offset/clock_rate);
    delete tmp;
    cout << dmq << endl;
  }

  //First construct the cut

  TCut cut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0) + Trig2(kin==16?1:0);
  TCut emcut = ACC(kin==16?1:0) + PID(kin==16?1:0) + emW2(kin==16?1:0) + Trig2(kin==16?1:0);

  //Get Normalization value
  TF1 *emupfit = new TF1("emupfit","gaus",-.18,-.08);
  TF1 *emdownfit = new TF1("emdownfit","gaus",.08,.18);
  TF1 *dmupfit = new TF1("dmupfit","gaus",-.18,-.08);
  TF1 *dmdownfit = new TF1("dmdownfit","gaus",.08,.18);

  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *emz = new TH1D(Form("emz_kin%d",kin),Form("Empty Z for Kinematic %d",kin),360,-.18,.18);
  em->Draw((kin==16) ? Form("rpr.z>>emz_kin%d",kin) : Form("rpl.z>>emz_kin%d",kin) ,emcut);

  emz->Fit("emdownfit","L","SAME",.12,.15);
  emz->Fit("emupfit","L","SAME",-.13,-.1);
  
  TCanvas *c1 = new TCanvas();
  c1->cd(0);
  TH1D *dmz = new TH1D(Form("dmz_kin%d",kin),Form("Dummy Z for Kindmatic %d",kin),360,-.18,.18);
  dm->Draw((kin==16) ? Form("rpr.z>>dmz_kin%d",kin) : Form("rpl.z>>dmz_kin%d",kin) ,emcut);

  dmz->Fit("dmdownfit","L","SAME",.12,.15);
  dmz->Fit("dmupfit","L","SAME",-.13,-.1);

  //Can get standard dev with GetParameter(2) and center with GetParameter(1)
  double emup_center = emupfit->GetParameter(1);
  double emup_stdev = emupfit->GetParameter(2);
  double emdown_center = emdownfit->GetParameter(1);
  double emdown_stdev = emdownfit->GetParameter(2);
  double dmup_center = dmupfit->GetParameter(1);
  double dmup_stdev = dmupfit->GetParameter(2);
  double dmdown_center = dmdownfit->GetParameter(1);
  double dmdown_stdev = dmdownfit->GetParameter(2);

  //Plot EM/Targ for z region of the kinematic
  TString x = (kin==16) ? "EKRx.x_bj" : "EKLx.x_bj";

  //Define Plots
  TH1D *em_up = new TH1D(Form("em_up_kin%d",kin),Form("Kinematic %d EM/DM Upstream Endcap",kin),33,0,0.99);
  TH1D *em_down = new TH1D(Form("em_down_kin%d",kin),Form("Kinematic %d EM/DM Downstream Endcap",kin),33,0,0.99);
  TH1D *dm_up  = new TH1D("dm_up","Temporary DM Plot",33,0,0.99);
  TH1D *dm_down  = new TH1D("dm_down","Temporary DM Plot",33,0,0.99);
  em_up->Sumw2();
  em_down->Sumw2();
  dm_up->Sumw2();
  dm_down->Sumw2();

  TCanvas *c3 = new TCanvas();
  c3->cd(0);
  em->Draw(Form("%s>>em_up_kin%d",x.Data(),kin),cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,emup_center,outsig*emup_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,emup_center,insig*emup_stdev)));
 
  TCanvas *c4 = new TCanvas();
  c4->cd(0);
  dm->Draw(Form("%s>>dm_up",x.Data()),cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,dmup_center,outsig*emup_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,dmup_center,insig*emup_stdev)));
 
  TCanvas *c5 = new TCanvas();
  c5->cd(0);
  em->Draw(Form("%s>>em_down_kin%d",x.Data(),kin),cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,emdown_center,insig*emdown_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,emdown_center,outsig*emdown_stdev)));
 
  TCanvas *c6 = new TCanvas();
  c6->cd(0);
  dm->Draw(Form("%s>>dm_down",x.Data()),cut + TCut(Form((kin==16) ? "rpr.z>%f-%f" : "rpl.z>%f-%f" ,dmdown_center,insig*dmdown_stdev)) + TCut(Form((kin==16) ? "rpr.z<%f+%f" : "rpl.z<%f+%f" ,dmdown_center,outsig*dmdown_stdev)));
  
  TH1D *tmp1 = (TH1D*) em_up->Clone("tmp1");
  tmp1->Scale(1/emq);
  TH1D *tmp2 = (TH1D*) em_down->Clone("tmp2");
  tmp2->Scale(1/emq);
  
  TCanvas *c7 = new TCanvas();
  c7->cd(0);
  tmp1->Draw();

  TCanvas *c8 = new TCanvas();
  c8->cd(0);
  tmp2->Draw();

  //Divide EM/DM
  //How do I normalize? Charge?
  em_up->Divide(dm_up);
  dm_up->Scale(1/dmq);
  em_up->Scale(dmq/emq);
  em_down->Divide(dm_down);
  dm_down->Scale(1/dmq);
  em_down->Scale(dmq/emq);

  /*TFile *f = new TFile(outfolder+"/em_dm.root","UPDATE");
  em_up->Write();
  em_down->Write();
  f->Close();*/
}
