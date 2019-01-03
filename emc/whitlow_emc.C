Double_t Ch[12] = {1.417,-0.108,1.486,-5.979,3.524,-0.011,-0.619,1.385,0.270,-2.179,4.722,-4.363};
Double_t Cd[12] = {0.948,-0.115,1.861,-4.733,2.348,-0.065,-0.224,1.085,0.213,-1.687,3.409,-3.255};

Double_t beta(Double_t x, Double_t Q2, bool deut){
  if(deut){
    Double_t exponent = -1.*TMath::Min(20., 7.7*((1./x)+(PROTON_MASS*PROTON_MASS/Q2)-1.));
    Double_t b = TMath::Power(1.-TMath::Power(TMath::E(),exponent),-1.);
    return b;
  }else{
    return 1.;
  }
}

Double_t Ax(Double_t x){
  Double_t ret = 1.22*TMath::Power(TMath::E(),3.2*x);
  return ret;
}

Double_t l2(Double_t x, Double_t Q2, bool deut){
  if(Q2<=(Ax(x))){
    if(deut){
      return (Cd[5]+(Cd[6]*x)+(Cd[7]*x*x));
    }else{
      return (Ch[5]+(Ch[6]*x)+(Ch[7]*x*x));
    }
  }else{
    return 0.;
  }
}

Double_t l1(Double_t x, bool deut){
  if(deut){
    return (Cd[8]+(Cd[9]*x)+(Cd[10]*x*x)+(Cd[11]*x*x*x));
  }else{
    return (Ch[8]+(Ch[9]*x)+(Ch[10]*x*x)+(Ch[11]*x*x*x));
  }
}

Double_t f2thr(Double_t x, bool deut){
  Double_t ret = 0.;
  if(deut){
    for(Int_t i=0;i<5;i++){
      ret += Cd[i]*TMath::Power((1.-x),i+3);
    }
    return ret;
  }
  else{
    for(Int_t i=0;i<5;i++){
      ret += Ch[i]*TMath::Power((1.-x),i+3);
    }
    return ret;
  }
}

Double_t f2(Double_t x, Double_t Q2, bool deut){
  Double_t ret = 0.;
  ret = beta(x,Q2,deut)*f2thr(x,deut);
  ret *= (1.+l1(x,deut)*TMath::Log(Q2/Ax(x))+l2(x,Q2,deut)*TMath::Power(TMath::Log(Q2/Ax(x)),2.));
  if(deut){
    return 2.*ret;
  }else{
    return ret;
  }
}

void whitlow_emc(TString folder){
  string tmp;
  ifstream in("/work/halla/triton/tjhague/Tritium-Analysis/emc/x_for_iso.dat");
  getline(in,tmp);

  TH1D *w = new TH1D("whitlow_emc","Whitlow EMC Prediction",33,0,0.99);
  for(int i=1; i<=33; i++){
    double blah, He3x, He3Q2, D2x, D2Q2;
    in >> blah >> blah >> He3x >> He3Q2 >> D2x >> D2Q2;
    if(He3x!=0){
      double He3p = f2(He3x,He3Q2,false);
      double He3d = f2(He3x,He3Q2,true);
      double He3 = He3d + He3p;
      He3 /= 3.;
      double D2 = f2(D2x,D2Q2,true);
      D2 /= 2.;
      w->SetBinContent(i,(He3/D2));
    }
  }
  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  w->Write();
  delete w;
  delete f;
  in.close();
}
