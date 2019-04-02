Double_t f2rationorm(Double_t x){
  Double_t f2r = 0.90975462;
  f2r -= x*1.07242453;
  f2r += x*x*0.58567877;
  f2r += x*x*x*0.01183161;
  return f2r;
}

Double_t f2ratio(Double_t x){
  Double_t f2r = 0.984;
  f2r -= x*1.16;
  f2r += x*x*0.647;
  f2r += x*x*x*5.09e-4;
  return f2r;
}

Double_t f2ratiobodek(Double_t x){
  Double_t f2r = 0.92550528;
  f2r -= x*1.08991741;
  f2r += x*x*0.59837244;
  f2r += x*x*x*0.00955333;
  return f2r;
}

Double_t f2ratiowhitlow(Double_t x){
  Double_t f2r = 0.94071938;
  f2r -= x*1.10692089;
  f2r += x*x*0.61076815;
  f2r += x*x*x*0.00729377;
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
  ifstream iso_in(Form("%s/x_for_iso.dat",folder.Data()));
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
      emciso->SetBinError(j,emciso->GetBinError(j)*He3iso(avgx,0));
      emcisonorm->SetBinContent(j,emcisonorm->GetBinContent(j)*He3iso(avgx));
      emcisonorm->SetBinError(j,emcisonorm->GetBinError(j)*He3iso(avgx));
      emcisobodek->SetBinContent(j,emcisobodek->GetBinContent(j)*He3iso(avgx,2));
      emcisobodek->SetBinError(j,emcisobodek->GetBinError(j)*He3iso(avgx,2));
      emcisowhitlow->SetBinContent(j,emcisowhitlow->GetBinContent(j)*He3iso(avgx,3));
      emcisowhitlow->SetBinError(j,emcisowhitlow->GetBinError(j)*He3iso(avgx,3));
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
