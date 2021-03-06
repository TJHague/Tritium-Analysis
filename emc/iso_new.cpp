void iso_new(TString folder, int nbins){
  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  TH1D *base = (TH1D*) f->Get("emc");
  //base->SetDirectory(0);

  TH1D* emc_iso = (TH1D*) base->Clone("emciso");
  TH1D* emcnorm = (TH1D*) base->Clone("emcisonorm");
  TH1D* emcbodek = (TH1D*) base->Clone("emcisonormbodek");
  TH1D* emcwhit = (TH1D*) base->Clone("emcisowhitlow");

  for(int i=1; i<=nbins; i++){
    double c = emc_iso->GetBinContent(i);
    cout << i << " " << emc_iso->GetBinCenter(i) << " " << c << endl;
    if(c!=0){
      cout << "test" << endl;
      //No norm
      //Get f2n/f2p data for data point
      double f2r = f2r_data(emc_iso->GetBinCenter(i));

      //Calculate and apply isoscalar correction
      double iso = 0.5 * (1. + f2r);
      iso /= (1./3.) * (2. + f2r);
      emc_iso->SetBinContent(i,c*iso);

      //Marathon Norm
      f2r = f2r_data(emc_iso->GetBinCenter(i),1);

      iso = 0.5 * (1. + f2r);
      iso /= (1./3.) * (2. + f2r);
      emcnorm->SetBinContent(i,c*iso);

      //Bodek Norm
      f2r = f2r_data(emc_iso->GetBinCenter(i),2);

      iso = 0.5 * (1. + f2r);
      iso /= (1./3.) * (2. + f2r);
      emcbodek->SetBinContent(i,c*iso);

      //Whitlow Norm
      f2r = f2r_data(emc_iso->GetBinCenter(i),3);

      iso = 0.5 * (1. + f2r);
      iso /= (1./3.) * (2. + f2r);
      emcwhit->SetBinContent(i,c*iso);
    }
  }

  emc_iso->Write();
  emcnorm->Write();
  emcbodek->Write();
  emcwhit->Write();
  f->Close();

  /*delete f;
  delete base;
  delete emc_iso;*/
}
