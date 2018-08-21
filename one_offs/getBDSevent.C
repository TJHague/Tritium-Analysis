#include "../headers/rootalias.h"

void getBDSevent(Int_t run, Int_t event){
  TChain *T = LoadRun(run);

  Double_t BDS;
  T->SetBranchAddress("haBDSPOS",&BDS);
  T->GetEntry(event);
  cout << "BDS position at event " << event << " is: " << BDS << endl << endl;
}
