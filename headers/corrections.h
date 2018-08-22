const Double_t Na = 6.0221409e23; //Avogadro's Number

Double_t He3Positrons(Double_t x){
  return 0.1009*TMath::Exp(-1.*x*8.913);
}

Double_t H3Positrons(Double_t x){
  return 0.0778*TMath::Exp(-1.*x*8.026);
}

Double_t D2Positrons(Double_t x){
  return 0.087557*TMath::Exp(-1*x*8.446762);
}

Double_t H1Positrons(Double_t x){
  return 0.079031*TMath::Exp(-1*x*8.587641);
}

Double_t He3Boiling(Double_t I){
  return (1 + (-0.004759*I) + (8.69e-05*I*I));
}

Double_t H3Boiling(Double_t I){
  return (1 + (-0.007399*I) + (0.0001293*I*I));
}

Double_t D2Boiling(Double_t I){
  return (1 + (-0.006651*I) + (0.0001147*I*I));
}

Double_t H1Boiling(Double_t I){
  return (1 + (-0.008529*I) + (0.0001527*I*I));
}

Double_t He3Thickness(Double_t I){
  return 0.0534*He3Boiling(I);
}

Double_t H3Thickness(Double_t I){
  return 0.077*H3Boiling(I);
}

Double_t D2Thickness(Double_t I){
  return 0.1422*D2Boiling(I);
}

Double_t H1Thickness(Double_t I){
  return 0.0708*H1Boiling(I);
}

Double_t He3Nuclei(Double_t I){
  return Na*He3Thickness(I)/3.0160293;
}

Double_t H3Nuclei(Double_t I){
  return Na*H3Thickness(I)/3.0160492;
}

Double_t D2Nuclei(Double_t I){
  return Na*D2Thickness(I)/2.01410178;
}

Double_t H1Nuclei(Double_t I){
  return Na*H1Thickness(I)/1.00794;

Double_t He3ECC(Int_t kin){
  if(kin==1){
    return 0.028307946;
  }else if(kin==2){
    return 0.026412663;
  }else if(kin==3){
    return 0.021025946;
  }else if(kin==5){
    return 0.017594063;
  }else if(kin==7){
    return 0.013816999;
  }else if(kin==9){
    return 0.011232989;
  }else if(kin==11){
    return 0.008003037;
  }else if(kin==13){
    return 0.006503249;
  }else if(kin==15){
    return 0.005162994;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t H3ECC(Int_t kin){
  if(kin==1){
    return 0.021853892;
  }else if(kin==2){
    return 0.020286097;
  }else if(kin==3){
    return 0.016302875;
  }else if(kin==5){
    return 0.014454685;
  }else if(kin==7){
    return 0.01176781;
  }else if(kin==9){
    return 0.009388988;
  }else if(kin==11){
    return 0.006939431;
  }else if(kin==13){
    return 0.0055396;
  }else if(kin==15){
    return 0.004362285;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t D2ECC(Int_t kin){
  if(kin==1){
    return 0.011656013;
  }else if(kin==2){
    return 0.010794583;
  }else if(kin==3){
    return 0.008394455;
  }else if(kin==4){
    return 0.008052026;
  }else if(kin==5){
    return 0.007242350;
  }else if(kin==7){
    return 0.005573921;
  }else if(kin==9){
    return 0.004512027;
  }else if(kin==11){
    return 0.003280984;
  }else if(kin==13){
    return 0.002642317;
  }else if(kin==15){
    return 0.002147393;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t H1ECC(Int_t kin){
  if(kin==1){
    return 0.025429549;
  }else if(kin==2){
    return 0.023649616;
  }else if(kin==3){
    return 0.022536696;
  }else if(kin==4){
    return 0.021100542;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}
