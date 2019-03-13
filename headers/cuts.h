//0 is Left, else is Right
//
//Functions that return TCut are probably unnecessary now. Going to leave them for the time being in case I need them later. - Aug 24, 2018
//Oh hey, they're going to be useful :D - Feb 21, 2019

TCut ACC(Int_t arm=0){
  TCut ACC="";
  if(arm==0){
    ACC += "TMath::Abs(L.tr.tg_dp)<0.04";
    ACC += "((L.tr.tg_ph/0.035)**2) + (((L.tr.tg_th-0.0025)/0.0675)**2) < 1";
  }else{
    ACC += "TMath::Abs(R.tr.tg_dp)<0.04";
    ACC += "((R.tr.tg_ph/0.035)**2) + (((R.tr.tg_th-0.0025)/0.0675)**2) < 1";
  }
  return ACC;
}

Bool_t ACC(Double_t ph, Double_t th, Double_t dp, Int_t arm=0){
  if(arm==0){
    if((TMath::Abs(ph)<0.033)){
      if((th>-0.06)&&(th<0.06)){
        if((TMath::Abs(dp)<0.04)){
          return true;
        }
      }
    }
  }else{
    if((ph>-0.03)&&(ph<0.033)){
      if((th>-0.06)&&(th<0.06)){
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
    PID += "L.cer.asum_c>1500";
    PID += "(L.prl1.e+L.prl2.e)/L.tr.p[0]/1000>0.7";
    PID += "L.tr.n==1";
  }else{
    PID += "R.cer.asum_c>2000";
    PID += "(R.ps.e+R.sh.e)/R.tr.p[0]/1000>0.7";
    PID += "R.tr.n==1";
  }
  return PID;
}

TCut PID_noep(Int_t arm=0){
  TCut PID="";
  if(arm==0){
    PID += "L.cer.asum_c>1500";
    PID += "L.tr.n==1";
  }else{
    PID += "R.cer.asum_c>2000";
    PID += "R.tr.n==1";
  }
  return PID;
}


Bool_t PID(Double_t cer, Double_t prl1, Double_t prl2, Double_t p, Double_t n, Int_t arm=0){
  if(arm==0){
    if(cer>1500.){
      if((prl1+prl2)/p/1000.>0.7){
        if(n==1){
          return true;
        }
      }
    }
  }else{
    if(cer>2000){
      if((prl1+prl2)/p/1000.>0.7){
        if(n==1)
          return true;
      }
    }
  }
  return false;
}

const double up_cut[12] = {-0.08, -0.08, -0.08, -0.08, -0.08, -0.09, -0.09, -0.095, -0.095, -0.1, -0.1, -0.105};
const double down_cut[12] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.105, 0.105, 0.11};

TCut EC(int kin){
  TCut EC="";
  TString zval = (kin==16) ? "rpr.z" : "rpl.z";
  int it;
  if(kin<=5){
    it=kin;
  }else if(kin==16){
    it=11;
  }else{
    it=((kin-5)/2)+5;
  }
  EC += Form("%s>%f",zval.Data(),up_cut[it]);
  EC += Form("%s<%f",zval.Data(),down_cut[it]);
  return EC;
}

Bool_t EC(Double_t z, int kin){
  int it;
  if(kin<=5){
    it=kin;
  }else if(kin==16){
    it=11;
  }else{
    it=((kin-5)/2)+5;
  }
  return ((z>up_cut[it])&&(z<down_cut[it]));
}

TCut Trig2(Int_t arm=0){
  TCut T2="";
  if(arm==0){
    T2 += "DL.bit2==1";
  }else{
    T2 += "DR.bit5==1";
  }
  return T2;
}

TCut emW2(Int_t arm=0){
  TCut W2="";
  if(arm==0){
    W2 += "EKLx.W2>3";
  }else{
    W2 += "EKRx.W2>3";
  }
  return W2;
}

TCut W2(Int_t arm=0){
  TCut W2="";
  if(arm==0){
    W2 += "EKLxe.W2>3";
  }else{
    W2 += "EKRxe.W2>3";
  }
  return W2;
}

Bool_t W2cut(Double_t W2){
  if(W2>3){
    return true;
  }
  return false;
}
