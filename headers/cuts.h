//0 is Left, else is Right
//
//Functions that return TCut are probably unnecessary now. Going to leave them for the time being in case I need them later. - Aug 24, 2018
TCut ACC(Int_t arm=0){
  TCut ACC="";
  if(arm==0){
    ACC += "TMath::Abs(L.tr.tg_ph)<0.03";
    ACC += "L.tr.tg_th>-0.05";
    ACC += "L.tr.tg_th<0.06";
    ACC += "TMath::Abs(L.tr.tg_dp)<0.04";
  }else{
    ACC += "R.tr.tg_ph>-0.03";
    ACC += "R.tr.tg_ph<0.02";
    ACC += "R.tr.tg_th>-0.05";
    ACC += "R.tr.tg_th<0.055";
    ACC += "TMath::Abs(R.tr.tg_dp)<0.04";
  }
  return ACC;
}

Bool_t ACC(Double_t ph, Double_t th, Double_t dp, Int_t arm=0){
  if(arm==0){
    if((TMath::Abs(ph)<0.03)){
      if((th>-0.05)&&(th<0.06)){
        if((TMath::Abs(dp)<0.04)){
          return true;
        }
      }
    }
  }else{
    if((ph>-0.03)&&(ph<0.02)){
      if((th>-0.05)&&(th<0.055)){
        if((TMath::Abs(dp)<0.04)){
          return true;
        }
      }
    }
  }
  return false;
}

TCut PID(Int_t arm=0){
  TCut PID="";
  if(arm==0){
    PID += "L.cer.asum_c>2000";
    PID += "(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000>0.75";
    PID += "L.tr.n==1";
  }else{
    PID += "R.cer.asum_c>2000";
    PID += "(R.ps.e+R.sh.e)/R.tr.p[0]/1000>0.8";
    PID += "R.tr.n==1";
  }
  return PID;
}

Bool_t PID(Double_t cer, Double_t prl1, Double_t prl2, Double_t p, Double_t n, Int_t arm=0){
  if(arm==0){
    if(cer>2000.){
      if((prl1+prl2)/p/1000.>0.75){
        if(n==1){
          return true;
        }
      }
    }
  }else{
    if(cer>2000){
      if((prl1+prl2)/p/1000.>0.8){
        if(n==1)
          return true;
      }
    }
  }
  return false;
}

TCut EC(Int_t arm=0){
  TCut EC="";
  if(arm==0){
    EC += "TMath::Abs(L.tr.vz)<0.1";
  }else{
    EC += "TMath::Abs(R.tr.vz)<0.1";
  }
  return EC;
}

Bool_t EC(Double_t z, Int_t arm=0){
  if(arm==0){
    if(TMath::Abs(z)<0.1){
      return true;
    }
  }else{
    if(TMath::Abs(z)<0.1){
      return true;
    }
  }
  return false;
}

TCut Trig2(Int_t arm=0){
  TCut T2="";
  if(arm==0){
    T2 += "DL.bit2==1";
  }else{
    T2 += "DR.evtypebits>>5&1";
  }
  return T2;
}

Bool_t W2cut(Double_t W2){
  if(W2>2.5){
    return true;
  }
  return false;
}
