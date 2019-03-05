#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

void check_yvz(TString runlist){

  TString list = gGet_InputFile_Var(runlist,2);

  vector<Int_t> runvec = gGet_RunNoChain(list);

  TCut CK = "L.cer.asum_c>2000";
  TCut EPcut = "(L.prl1.e+L.prl2.e)/(1000*L.tr.p[0])>0.75";
  TCut Trk1 = "L.tr.n==1";
  TCut Trk2 = "L.tr.n<3";
  TCut GAS = "L.tr.vz>-0.09 && L.tr.vz<0.1"; 
  TCut ACC = "L.tr.tg_ph<0.03&&L.tr.tg_ph>-0.03&&L.tr.tg_th>-0.06&&L.tr.tg_th<0.06&&L.tr.tg_dp>-0.04&&L.tr.tg_dp<0.04";
  TCut ACC2 = "L.tr.tg_ph>-0.03&&(L.tr.tg_th-0.333*L.tr.tg_ph-0.06<0)&&(L.tr.tg_th+0.333*L.tr.tg_ph-0.06)<0&&L.tr.tg_ph<0.03&&(L.tr.tg_th+0.333*L.tr.tg_ph+0.06>0)&&(L.tr.tg_th-0.333*L.tr.tg_ph+0.06)>0&&L.tr.tg_dp>-0.045&&L.tr.tg_dp<0.045";
  TCut trigger2="(DL.evtypebits>>2)&1";
  TCut trigger ="(DL.evtypebits>>1)&1";

  for(int i=0; i<runvec.size(); i++){
    cout << runvec[i] << endl << endl;
    TChain *rootfile = LoadRun(runvec[i]);
    TH2F *yz=new TH2F("yz","y_tar VS z_tar(CK,Trk1,E/P and T2/T5)",200,-0.2,0.2,150,-0.1,0.1); 
    if(runvec[i]<90000){
      rootfile->Draw("L.tr.tg_y:L.tr.vz>>yz",CK+Trk1+trigger2+EPcut,"COLZ");
    }else{
      rootfile->Draw("RightBCM.current_dnew:Entry$");
    }
    gPad->WaitPrimitive();
    delete yz;
  }
}
