{
  TFile *f = new TFile("p2_test/He3.root");
  gStyle->SetErrorX(0);

  TH1D *iw = (TH1D*) f->Get("emcisonorm");
  TH1D *ir = (TH1D*) f->Get("emc");
  iw->SetDirectory(0);
  ir->SetDirectory(0);
  //ir = (TH1D*) iw->Clone("emcisonorm2");
  f->Close();

  ofstream ofi("p2_test/emc_raw.dat");

  for(int i=0;i<33;i++){
    double c = iw->GetBinContent(i);
    if(c!=0){
      ofi << iw->GetBinCenter(i) << "," << c << "," << iw->GetBinError(i) << endl;
    }
  }


  iw->Scale(1.027);
  ofstream of("p2_test/emc_norm.dat");

  for(int i=0;i<33;i++){
    double c = iw->GetBinContent(i);
    if(c!=0){
      of << iw->GetBinCenter(i) << "," << c << "," << iw->GetBinError(i) << endl;
    }
  }
  of.close();

  ofstream ofr("p2_test/emc_noiso.dat");

  for(int i=0;i<33;i++){
    double c = ir->GetBinContent(i);
    if(c!=0){
      ofr << ir->GetBinCenter(i) << "," << c << "," << ir->GetBinError(i) << endl;
    }
  }
  ofr.close();

}
