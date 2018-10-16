/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

void merge_all(TString folder){
  
  TH1D *hist[12];

  //Combine iterations
  TFile *a = new TFile(Form("%s/cleanedKin/kin0.root",folder.Data()));
  hist[0] = (TH1D*) gDirectory->Get("emc");
  hist[0]->SetDirectory(0);
  delete a;

  TFile *b = new TFile(Form("%s/cleanedKin/kin1.root",folder.Data()));
  hist[1] = (TH1D*) gDirectory->Get("emc");
  hist[1]->SetDirectory(0);
  delete b;

  TFile *c = new TFile(Form("%s/cleanedKin/kin2.root",folder.Data()));
  hist[2] = (TH1D*) gDirectory->Get("emc");
  hist[2]->SetDirectory(0);
  delete c;

  TFile *d = new TFile(Form("%s/cleanedKin/kin3.root",folder.Data()));
  hist[3] = (TH1D*) gDirectory->Get("emc");
  hist[3]->SetDirectory(0);
  delete d;

  TFile *e = new TFile(Form("%s/cleanedKin/kin4.root",folder.Data()));
  hist[4] = (TH1D*) gDirectory->Get("emc");
  hist[4]->SetDirectory(0);
  delete e;

  TFile *f = new TFile(Form("%s/cleanedKin/kin5.root",folder.Data()));
  hist[5] = (TH1D*) gDirectory->Get("emc");
  hist[5]->SetDirectory(0);
  delete f;

  TFile *g = new TFile(Form("%s/cleanedKin/kin7.root",folder.Data()));
  hist[6] = (TH1D*) gDirectory->Get("emc");
  hist[6]->SetDirectory(0);
  delete g;

  TFile *h = new TFile(Form("%s/cleanedKin/kin9.root",folder.Data()));
  hist[7] = (TH1D*) gDirectory->Get("emc");
  hist[7]->SetDirectory(0);
  delete h;

  TFile *k = new TFile(Form("%s/cleanedKin/kin11.root",folder.Data()));
  hist[8] = (TH1D*) gDirectory->Get("emc");
  hist[8]->SetDirectory(0);
  delete k;

  TFile *l = new TFile(Form("%s/cleanedKin/kin13.root",folder.Data()));
  hist[9] = (TH1D*) gDirectory->Get("emc");
  hist[9]->SetDirectory(0);
  delete l;

  TFile *m = new TFile(Form("%s/cleanedKin/kin15.root",folder.Data()));
  hist[10] = (TH1D*) gDirectory->Get("emc");
  hist[10]->SetDirectory(0);
  delete m;

  TFile *n = new TFile(Form("%s/cleanedKin/kin16.root",folder.Data()));
  hist[11] = (TH1D*) gDirectory->Get("emc");
  hist[11]->SetDirectory(0);
  delete n;

  if((hist[0]->GetNbinsX() != hist[1]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[2]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[3]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[4]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[5]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[6]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[7]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[8]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[9]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[10]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[11]->GetNbinsX())){
    cout << "Plots to be combined do not have the same number of bins." << endl;
    cout << "Make sure they are plotted the same way before using this." << endl;
    return;
  }

  TH1D *emc = new TH1D("emc","He-3 EMC Ratio",hist[0]->GetNbinsX(),hist[0]->GetXaxis()->GetXmin(),hist[0]->GetXaxis()->GetXmax());

  for(int i = 1; i <= hist[0]->GetNbinsX(); i++){
    double c = 0;
    double e = 0;
    for(int j = 0; j <= 11; j++){
      double c1 = hist[j]->GetBinContent(i);
      if(c1 != 0){
        double e1 = hist[j]->GetBinError(i);
        c += c1/(e1*e1);
        e += 1./(e1*e1);
      }
    }
    if(c != 0){
      c /= e;
      e = 1./TMath::Sqrt(e);
      e = TMath::Sqrt((e*e)+(c*0.005*c*0.005));
      emc->SetBinContent(i,c);
      emc->SetBinError(i,e);
    }
  }

  gStyle->SetErrorX(0.);
  emc->SetAxisRange(0.9,1.3,"Y");
  emc->SetAxisRange(0,1,"X");

  TFile *out = new TFile(Form("%s/He3EMC.root",folder.Data()),"RECREATE");
  emc->Write();

  delete emc;
  delete out;

}
