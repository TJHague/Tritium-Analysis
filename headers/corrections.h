//const Double_t Na = 6.0221409e23; //Avogadro's Number
//Error calculations return absolute error squared

Double_t He3Positrons(Double_t x){
  return TMath::Exp((-8.42406685*x) - 2.63480349);
}

Double_t He3PositronsError(Double_t x){
  double e2 = x*((x*0.18265823) - 0.04746606);
  e2 += (x*-0.04746606) + 0.01300976;
  e2 *= TMath::Power(He3Positrons(x),2.);
  return e2;
}

Double_t H3Positrons(Double_t x){
  return TMath::Exp((-8.42875968*x) - 2.61143195);
}

Double_t H3PositronsError(Double_t x){
  double e2 = x*((x*0.11225664) - 0.02803864);
  e2 += (x*-0.02803864) + 0.0073564;
  e2 *= TMath::Power(H3Positrons(x),2.);
  return e2;
}

Double_t D2Positrons(Double_t x){
  return TMath::Exp((-9.15721674*x) - 2.46454072);
}

Double_t D2PositronsError(Double_t x){
  double e2 = x*((x*0.08534386) - 0.0208084);
  e2 += (x*-0.0208084 ) + 0.00529717;
  e2 *= TMath::Power(D2Positrons(x),2.);
  return e2;
}

Double_t H1Positrons(Double_t x){
  return TMath::Exp((-10.1463193*x) - 2.38884408);
}

Double_t H1PositronsError(Double_t x){
  double e2 = x*((x*0.4684634) - 0.10968713);
  e2 += (x*-0.10968713) + 0.02644275;
  e2 *= TMath::Power(H1Positrons(x),2.);
  return e2;
}

Double_t He3Boiling(Double_t I){
  return (1 + (-0.004759*I) + (8.69e-05*I*I));
}

Double_t He3BoilingError(Double_t I){
  double e2 = 2.27472736e-5 + (I*-3.87742681e-6) + (I*I*1.33896582e-7);
  e2 += I*(-3.87742681e-6 + (I*8.29429505e-7) + (I*I*-3.12627997e-8));
  e2 += I*I*(1.33896582e-7 + (I*-3.12627997e-8) + (I*I*1.23486170e-9));
  return e2;
}

Double_t H3Boiling(Double_t I){
  return (1 + (-0.007399*I) + (0.0001293*I*I));
}

Double_t H3BoilingError(Double_t I){
  double e2 = 1.86657952e-5 + (I*-3.19917239e-6) + (I*I*1.11529912e-7);
  e2 += I*(-3.19917239e-6 + (I*6.78338365e-7) + (I*I*-2.56740813e-8));
  e2 += I*I*(1.11529912e-7 + (I*-2.56740813e-8) + (I*I*1.01360349e-9));
  return e2;
}

Double_t D2Boiling(Double_t I){
  return (1 + (-0.006651*I) + (0.0001147*I*I));
}

Double_t D2BoilingError(Double_t I){
  double e2 = 3.29666199e-5 + (I*-5.43509145e-6) + (I*I*1.85710186e-7);
  e2 += I*(-5.43509145e-6 + (I*1.02699833e-6) + (I*I*-3.73633538e-8));
  e2 += I*I*(1.85710186e-7 + (I*-3.73633538e-8) + (I*I*1.42075428e-9));
  return e2;
}

Double_t H1Boiling(Double_t I){
  return (1 + (-0.008529*I) + (0.0001527*I*I));
}

Double_t H1BoilingError(Double_t I){
  double e2 = 3.51614436e-5 + (I*-5.65699024e-6) + (I*I*1.89975924e-7);
  e2 += I*(-5.65699024e-6 + (I*1.08065286e-6) + (I*I*-3.91159021e-8));
  e2 += I*I*(1.89975924e-7 + (I*-3.91159021e-8) + (I*I*1.48091481e-9));
  return e2;
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
}

Double_t He3ECC(Int_t kin){
  if(kin==0){
    return 0.0271195;
  }else if(kin==1){
    return 0.0251296;
  }else if(kin==2){
    return 0.0229473;
  }else if(kin==3){
    return 0.0183983;
  }else if(kin==4){
    return 0.0172781;
  }else if(kin==5){
    return 0.0157382;
  }else if(kin==7){
    return 0.0125219;
  }else if(kin==9){
    return 0.0097427;
  }else if(kin==11){
    return 0.0071525;
  }else if(kin==13){
    return 0.0057231;
  }else if(kin==15){
    return 0.0044973;
  }else if(kin==16){
    return 0.0032718;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t H3ECC(Int_t kin){
  if(kin==0){
    return 0.0234217;
  }else if(kin==1){
    return 0.0218539;
  }else if(kin==2){
    return 0.0202861;
  }else if(kin==3){
    return 0.0163029;
  }else if(kin==4){
    return 0.0153788;
  }else if(kin==5){
    return 0.0144547;
  }else if(kin==7){
    return 0.0117678;
  }else if(kin==9){
    return 0.009389;
  }else if(kin==11){
    return 0.0069394;
  }else if(kin==13){
    return 0.0055396;
  }else if(kin==15){
    return 0.0043623;
  }else if(kin==16){
    return 0.0032;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t D2ECC(Int_t kin){
  if(kin==0){
    return 0.0186017;
  }else if(kin==1){
    return 0.0113827;
  }else if(kin==2){
    return 0.0102172;
  }else if(kin==3){
    return 0.0082068;
  }else if(kin==4){
    return 0.0093849;
  }else if(kin==5){
    return 0.0086493;
  }else if(kin==7){
    return 0.0083141;
  }else if(kin==9){
    return 0.0057051;
  }else if(kin==11){
    return 0.0051219;
  }else if(kin==13){
    return 0.0034977;
  }else if(kin==15){
    return 0.002765;
  }else if(kin==16){
    return 0.0026865;
  }

/* Old Values
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
  }*/
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}

Double_t H1ECC(Int_t kin){
  if(kin==0){
    return 0.02922687;
  }else if(kin==1){
    return 0.02387363;
  }else if(kin==2){
    return 0.02094824;
  }else if(kin==3){
    return 0.01708867;
  }else if(kin==4){
    return 0.01767261;
  }
  cout << "Something went wrong. Returning no endcap contamination." << endl;
  return 0;
}
