#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

void check_target(TString runlist){

  TString list = gGet_InputFile_Var(runlist,2);
  
  vector<Int_t> runvec = gGet_RunNoChain(list);

  for(int i=0; i<runvec.size(); i++){
    TCanvas *c1 = new TCanvas("c1",TString(runvec[i]));
    c1->Divide(2,1);
    cout << runvec[i] << endl << endl;
    TChain *rootfile = LoadRun(runvec[i]);
    c1->cd(1);
    rootfile->Draw("haBDSPOS:Entry$");
    c1->cd(2);
    if(runvec[i]<90000){
      rootfile->Draw("LeftBCM.current_dnew:Entry$");
    }else{
      rootfile->Draw("RightBCM.current_dnew:Entry$");
    }
    gPad->WaitPrimitive();
  }
}
