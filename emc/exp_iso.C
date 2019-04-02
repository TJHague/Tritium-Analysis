{
  TFile *f = new TFile("p2_allerr/He3.root");
  gStyle->SetErrorX(0);

  TH1D *iw = (TH1D*) f->Get("emc");
  iw->SetDirectory(0);
  f->Close();

  ofstream of("p2_allerr/emc_raw.dat");

  for(int i=0;i<33;i++){
    double c = iw->GetBinContent(i);
    if(c!=0){
      of << iw->GetBinCenter(i) << "," << c << "," << iw->GetBinError(i) << endl;
    }
  }
  of.close();
}
