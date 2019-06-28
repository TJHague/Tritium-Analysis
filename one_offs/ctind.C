#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"
#include "../headers/rootalias.h"

void ctind(int run){

    TCanvas *c1 = new TCanvas("c1",TString(run));
    c1->Divide(2,1);
    cout << run << endl << endl;
    TChain *rootfile = LoadRun(run);
    c1->cd(1);
    rootfile->Draw("haBDSPOS:Entry$");
    c1->cd(2);
    if(run<90000){
      rootfile->Draw("LeftBCM.current_dnew:Entry$");
    }else{
      rootfile->Draw("RightBCM.current_dnew:Entry$");
    }
}
