/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

#include "../headers/rootalias.h"

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
  }else if(kin<=15){
    /* Kin 7-15 had 2 iterations
     * First combine them into one histogram with a weighted average
     * Then, move that to a new histogram and omit the first and last bin
     */
    
    //Combine iterations
    TFile *f = new TFile(Form("fullKin/kin%d_1st.root",kin));
    TH1D *i1 = (TH1D*) gDirectory->Get("emc");
    i1->SetDirectory(0);
    delete f;

    *f = new TFile(Form("fullKin/kin%d_2nd.root",kin));
    TH1D *i2 = (TH1D*) gDirectory->Get("emc");
    i2->SetDirectory(0);
    delete f;

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
        emc->SetBinContent(i,(c1/(e1*e1))+(c2/(e2*e2))*((e1*e1)+(e2*e2)));
        emc->SetBinError(i,Sqrt((e1*e1)+(e2*e2)));
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

  }else{ //kin 16
    /* Kin 16 had 3 iterations
     * First combine them into one histogram with a weighted average
     * Then, move that to a new histogram and omit the first and last bin
     */
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
