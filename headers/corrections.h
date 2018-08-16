Double_t He3Positrons(Double_t x){
  return 0.1009*TMath::Exp(-1.*x*8.913);
}

Double_t H3Positrons(Double_t x){
  return 0.0778*TMath::Exp(-1.*x*8.026);
}

Double_t D2Positrons(Double_t x){
  return 0.;
}

Double_t He3Boiling(Double_t I){
  return (1 + (-0.004759*I) + (8.69e-05*I*I));
}

Double_t H3Boiling(Double_t I){
  return (1 + (-0.007399*I) + (3.16e-05*I*I));
}

Double_t D2Boiling(Double_t I){
  return 1.;
}

Double_t He3ECC(Int_t kin){
  if(kin==1){
    return 0.028307946;
  }else if(kin==2){
    return 0.026412663;
  }
}
