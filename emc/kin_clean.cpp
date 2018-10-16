/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

void kin_clean(int kin, TString folder){
  if(kin<=5){
    /* Kin 0-5 only had 1 iteration
     * Move the data to a new histogram and omit the first and last bin
     */

    TFile *f = new TFile(Form("%s/radKin/kin%d.root",folder.Data(),kin));
    TH1D *emc = (TH1D*) gDirectory->Get("emc");
    emc->SetDirectory(0);
    delete f;

    bool first = true;
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
    cleaned->Sumw2();
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
    TFile *out = new TFile(Form("%s/cleanedKin/kin%d.root",folder.Data(),kin),"RECREATE");
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
    TFile *f = new TFile(Form("%s/radKin/kin%d_1st.root",folder.Data(),kin));
    TH1D *i1 = (TH1D*) gDirectory->Get("emc");
    i1->SetDirectory(0);
    delete f;

    TFile *g = new TFile(Form("%s/radKin/kin%d_2nd.root",folder.Data(),kin));
    TH1D *i2 = (TH1D*) gDirectory->Get("emc");
    i2->SetDirectory(0);
    delete g;

    if(i1->GetNbinsX() != i2->GetNbinsX()){
      cout << "Plots to be combined do not have the same number of bins." << endl;
      cout << "Make sure they are plotted the same way before using this." << endl;
      return;
    }

    TH1D *emc = new TH1D("combined","He-3 EMC Ratio",i1->GetNbinsX(),i1->GetXaxis()->GetXmin(),i1->GetXaxis()->GetXmax());
    emc->Sumw2();

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
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
    cleaned->Sumw2();
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
    TFile *out = new TFile(Form("%s/cleanedKin/kin%d.root",folder.Data(),kin),"RECREATE");
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
    TFile *f = new TFile(Form("%s/radKin/kin%d_1st.root",folder.Data(),kin));
    TH1D *i1 = (TH1D*) gDirectory->Get("emc");
    i1->SetDirectory(0);
    delete f;

    TFile *g = new TFile(Form("%s/radKin/kin%d_2nd.root",folder.Data(),kin));
    TH1D *i2 = (TH1D*) gDirectory->Get("emc");
    i2->SetDirectory(0);
    delete g;

    TFile *h = new TFile(Form("%s/radKin/kin%d_3rd.root",folder.Data(),kin));
    TH1D *i3 = (TH1D*) gDirectory->Get("emc");
    i3->SetDirectory(0);
    delete h;


    if(i1->GetNbinsX() != i2->GetNbinsX() || i1->GetNbinsX() != i3->GetNbinsX()){
      cout << "Plots to be combined do not have the same number of bins." << endl;
      cout << "Make sure they are plotted the same way before using this." << endl;
      return;
    }

    TH1D *emc = new TH1D("combined","He-3 EMC Ratio",i1->GetNbinsX(),i1->GetXaxis()->GetXmin(),i1->GetXaxis()->GetXmax());
    emc->Sumw2();

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
    TH1D *cleaned = new TH1D("emc","He-3 EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
    cleaned->Sumw2();
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
    TFile *out = new TFile(Form("%s/cleanedKin/kin%d.root",folder.Data(),kin),"RECREATE");
    cleaned->Write();

    delete emc;
    delete cleaned;
    delete out;

  }
}
