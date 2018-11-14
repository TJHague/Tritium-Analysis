/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

void merge_all_new(TString folder, TString h_name, TString outf="He3EMC", bool zero_err=false, TString inf="cleanedKin"){
  
  TH1D *hist[12];

  //Combine iterations
  TFile *a = new TFile(Form("%s/%s/kin0.root",folder.Data(),inf.Data()));
  hist[0] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[0]->SetDirectory(0);
  delete a;

  TFile *b = new TFile(Form("%s/%s/kin1.root",folder.Data(),inf.Data()));
  hist[1] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[1]->SetDirectory(0);
  delete b;

  TFile *c = new TFile(Form("%s/%s/kin2.root",folder.Data(),inf.Data()));
  hist[2] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[2]->SetDirectory(0);
  delete c;

  TFile *d = new TFile(Form("%s/%s/kin3.root",folder.Data(),inf.Data()));
  hist[3] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[3]->SetDirectory(0);
  delete d;

  TFile *e = new TFile(Form("%s/%s/kin4.root",folder.Data(),inf.Data()));
  hist[4] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[4]->SetDirectory(0);
  delete e;

  TFile *f = new TFile(Form("%s/%s/kin5.root",folder.Data(),inf.Data()));
  hist[5] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[5]->SetDirectory(0);
  delete f;

  TFile *g = new TFile(Form("%s/%s/kin7.root",folder.Data(),inf.Data()));
  hist[6] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[6]->SetDirectory(0);
  delete g;

  TFile *h = new TFile(Form("%s/%s/kin9.root",folder.Data(),inf.Data()));
  hist[7] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[7]->SetDirectory(0);
  delete h;

  TFile *k = new TFile(Form("%s/%s/kin11.root",folder.Data(),inf.Data()));
  hist[8] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[8]->SetDirectory(0);
  delete k;

  TFile *l = new TFile(Form("%s/%s/kin13.root",folder.Data(),inf.Data()));
  hist[9] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[9]->SetDirectory(0);
  delete l;

  TFile *m = new TFile(Form("%s/%s/kin15.root",folder.Data(),inf.Data()));
  hist[10] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[10]->SetDirectory(0);
  delete m;

  TFile *n = new TFile(Form("%s/%s/kin16.root",folder.Data(),inf.Data()));
  hist[11] = (TH1D*) gDirectory->Get(h_name.Data());
  hist[11]->SetDirectory(0);
  delete n;

  if((hist[0]->GetNbinsX() != hist[1]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[2]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[3]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[4]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[5]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[6]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[7]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[8]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[9]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[10]->GetNbinsX()) || (hist[0]->GetNbinsX() != hist[11]->GetNbinsX())){
    cout << "Plots to be combined do not have the same number of bins." << endl;
    cout << "Make sure they are plotted the same way before using this." << endl;
    return;
  }

  TH1D *emc = new TH1D(h_name.Data(),"He-3 EMC Ratio",hist[0]->GetNbinsX(),hist[0]->GetXaxis()->GetXmin(),hist[0]->GetXaxis()->GetXmax());

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
      if(!zero_err){
        emc->SetBinError(i,e);
      }else{
        emc->SetBinError(i,0);
      }
    }
  }

  gStyle->SetErrorX(0.);
  emc->SetAxisRange(0.9,1.3,"Y");
  emc->SetAxisRange(0,1,"X");

  TFile *out = new TFile(Form("%s/%s.root",folder.Data(),outf.Data()),"UPDATE");
  emc->Write();

  delete emc;
  delete out;

}
