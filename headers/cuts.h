//0 is Left, else is Right
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
    ACC += "TMath::Abs(L.tr.tg_dp)<0.04";
  }
  return ACC;
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

TCut EC(Int_t arm=0){
  TCut EC="";
  if(arm==0){
    EC += "TMath::Abs(L.tr.vz)<0.1";
  }else{
    EC += "TMath::Abs(R.tr.vz)<0.1";
  }
  return EC;
}

TCut Trig2(Int_t arm=0){
  TCut T2="";
  if(arm==0){
    T2 += "DL.evtypebits>>2&1";
  }else{
    T2 += "DR.evtypebits>>5&1";
  }
  return T2;
}
