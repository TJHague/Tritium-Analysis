//0 is Left, else is Right
//
//Functions that return TCut are probably unnecessary now. Going to leave them for the time being in case I need them later. - Aug 24, 2018
//Oh hey, they're going to be useful :D - Feb 21, 2019

TCut ACC(Int_t arm=0){
  TCut ACC="";
  if(arm==0){
    TString ph = "L.tr.tg_ph";
    TString th = "L.tr.tg_th";
    ACC = TString(ph+"+1.1*"+th+">-0.077").Data();
    ACC += TString(ph+"+3.3*"+th+">-0.198").Data();
    ACC += TString(ph+"-3.3*"+th+"<0.198").Data();
    ACC += TString(ph+"-1.1*"+th+"<0.077").Data();
    ACC += TString(ph+"<0.033").Data();
    ACC += TString(ph+"+1.1*"+th+"<0.088").Data();
    ACC += TString(ph+"+3.3*"+th+"<0.231").Data();
    ACC += TString(ph+"-3.3*"+th+">-0.231").Data();
    ACC += TString(ph+"-1.1*"+th+">-0.088").Data();
    ACC += TString(ph+">-0.033").Data();
    ACC += "L.tr.tg_dp>-0.035";
    ACC += "L.tr.tg_dp<0.045";
  }else{
    TString ph = "R.tr.tg_ph";
    TString th = "R.tr.tg_th";
    ACC += TString("0.015*"+ph+"-0.037*"+th+"<0.00222").Data();
    ACC += TString(ph+"<0.037").Data();
    ACC += TString(ph+"+14*"+th+"<0.877").Data();
    ACC += TString(ph+">-0.033").Data();
    ACC += TString(ph+"+6.6*"+th+">-0.396").Data();
    ACC += "R.tr.tg_dp>-0.03";
    ACC += "R.tr.tg_dp<0.045";

    //Focal Plane Cut
    ACC += "(0.16*R.tr.x)-(0.95*R.tr.th)<0.0325";
    ACC += "R.tr.x<0.5";
    ACC += "(0.155*R.tr.x)-(0.95*R.tr.th)>-0.027";
    ACC += "R.tr.x>-0.45";
  }
  return ACC;
}

Bool_t ACC(Double_t ph, Double_t th, Double_t dp, Double_t fpx=0, Double_t fpth=0, Int_t arm=0){
  if(arm==0){
    if(((ph+(1.1*th))>-0.077)&&((ph+(3.3*th))>-0.198)&&((ph-(3.3*th))<0.198)&&((ph-(1.1*th))<0.077)&&((ph+(1.1*th))<0.088)&&((ph+(3.3*th))<0.231)&&((ph-(3.3*th))>-0.231)&&((ph-(1.1*th))>-0.088)&&(ph<0.033)&&(ph>-0.033)){
      if(dp>-0.035&&dp<0.045){
        return true;
      }
    }
  }else{
    if((((0.015*ph)-(0.037*th))<0.00222) && (ph<0.037) && ((ph+(14*th))<0.877) && (ph>-0.033) && ((ph+(6.6*th))>-0.396)){
      if(dp>-0.03 && dp<0.045){
        if((((0.16*fpx)-(0.95*fpth))<0.0325) && (fpx<0.5) && (fpx>-0.45) && (((0.155*fpx)-(0.95*fpth))>-0.027) ){
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
  //cout << "z>" << up_cut[it] << "&&z<" << down_cut[it] << endl;
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
  if(W2>3.){
    return true;
  }
  return false;
}
