/* Some kinematics have more than one iteration. They must be combined by a weighted average.
 * All kinematics also need the edge points dropped. These are at the edge of our acceptance where we don't trust the results (and have low statistics)
 *
 * This is part of a set of code to get the He3/D2 EMC ratio.
 */

#include "../headers/rootalias.h"

void kin_clean(int kin){
  if(kin<=5){
    TFile *f = new TFile(Form("fullKin/kin%d",kin));
    TH1D *emc = (TH1D*) gDirectory->Get("emc");
    emc->SetDirectory(0);
    delete f;



  }else if(kin<=15){

  }else{ //kin 16

  }
}
