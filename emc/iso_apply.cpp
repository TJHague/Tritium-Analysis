Double_t f2rationorm(Double_t x){
  Double_t f2r = 0.9377;
  f2r -= x*1.224;
  f2r += x*x*0.7604;
  return f2r;
}

Double_t f2ratio(Double_t x){
  Double_t f2r = 1.019;
  f2r -= x*1.317;
  f2r += x*x*0.8194;
  return f2r;
}

Double_t f2ratiobodek(Double_t x){
  Double_t f2r = 0.9548;
  f2r -= x*1.243;
  f2r += x*x*0.7723;
  return f2r;
}

Double_t f2ratiowhitlow(Double_t x){
  Double_t f2r = 0.9706;
  f2r -= x*1.261;
  f2r += x*x*0.7838;
  return f2r;
}

Double_t He3iso(Double_t x, Double_t norm=1){
  Double_t f2r=0;
  if(norm==0){
    f2r = f2ratio(x);
  }else if(norm==2){
    f2r = f2ratiobodek(x);
  }else if(norm==3){
    f2r = f2ratiowhitlow(x);
  }else{
    f2r = f2rationorm(x);
  }
  Double_t iso = 0.5 * (1. + f2r);
  iso /= (1./3.) * (2. + f2r);
  return iso;
}

void iso_apply(TString folder, TString inhist=""){
  string tmp;
  ifstream iso_in("/work/halla/triton/tjhague/Tritium-Analysis/emc/x_for_iso.dat");
  getline(iso_in,tmp);

  TFile *a = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  TH1D *emc = (TH1D*) gDirectory->Get(Form("emc%s",inhist.Data()));
  TH1D *emciso = (TH1D*) emc->Clone(Form("emc%siso",inhist.Data()));
  TH1D *emcisonorm = (TH1D*) emc->Clone(Form("emc%sisonorm",inhist.Data()));
  TH1D *emcisobodek = (TH1D*) emc->Clone(Form("emc%sisobodek",inhist.Data()));
  TH1D *emcisowhitlow = (TH1D*) emc->Clone(Form("emc%sisowhitlow",inhist.Data()));

  for(int j=1; j<=emciso->GetNbinsX(); j++){
    double avgx;
    for(int k=0;k<3;k++){
      iso_in >> avgx;
    }
    if(emciso->GetBinContent(j)!=0){
      cout << emciso->GetBinCenter(j) << endl;
      cout << avgx << endl;
      cout << He3iso(avgx) << endl;
      emciso->SetBinContent(j,emciso->GetBinContent(j)*He3iso(avgx,0));
      emcisonorm->SetBinContent(j,emcisonorm->GetBinContent(j)*He3iso(avgx));
      emcisobodek->SetBinContent(j,emcisobodek->GetBinContent(j)*He3iso(avgx,2));
      emcisowhitlow->SetBinContent(j,emcisowhitlow->GetBinContent(j)*He3iso(avgx,3));
    }
    for(int k=0;k<3;k++){
      iso_in >> avgx;
    }
  }
  emciso->Write();
  emcisonorm->Write();
  emcisobodek->Write();
  emcisowhitlow->Write();
  delete emc;
  delete emciso;
  delete emcisonorm;
  delete emcisobodek;
  delete emcisowhitlow;
  delete a;

  iso_in.close();
}
