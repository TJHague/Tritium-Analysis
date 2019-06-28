/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void check_errors(Int_t kin, TString folder, Int_t nbins, Double_t low, Double_t high, Int_t iter=0){
  Int_t arm=0;
  //Load runs
  TString set="";
  if(kin>=7){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
    }else if(iter==3 && kin==15){
      set = "_3rd";
    }else{
      cout << "1st or 2nd iteration of that kin?" << endl;
      exit(0);
    }
  }

  TString He3dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; He3dat += "He3_kin"; He3dat += kin; He3dat += set.Data(); He3dat += ".dat";
  TString D2dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; D2dat  += "D2_kin";  D2dat  += kin; D2dat  += set.Data(); D2dat  += ".dat";
  TString He3list = gGet_InputFile_Var(He3dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  //cout << He3list << endl;
  vector<Int_t> He3vec = gGet_RunNoChain(He3list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t He3charge = 0;
  Double_t D2charge  = 0;
  TH1D *He3 = new TH1D("He3","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *D2  = new TH1D("D2" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  He3->Sumw2();
  D2->Sumw2();

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  ofstream berr(Form("%s/boiling_err.txt",folder.Data()),ofstream::app);
  //Helium-3 Yield
  for(unsigned int i=0; i<He3vec.size(); i++){
    yieldHistogram *He3part = new yieldHistogram(Form("%s/kin%d%s/He3/%d.dat",folder.Data(),kin,set.Data(),He3vec[i]));

    TH1D *tmp3 = He3part->getTH1(Form("He3_%d",He3vec[i]));
    tmp3->Scale(1. / He3part->getLivetime());
    tmp3->Scale(1. / He3Nuclei(He3part->getAvgI()));
    He3charge += He3part->getCharge();
    for(int j=1; j<=nbins; j++){
      if(tmp3->GetBinContent(j)!=0){
        double c = tmp3->GetBinContent(j);
        double e = tmp3->GetBinError(j);
        tmp3->SetBinError(j,TMath::Sqrt((e*e)+(c*c*He3BoilingError(He3part->getAvgI())/TMath::Power(He3Boiling(He3part->getAvgI()),2.))));
        berr << "He3_" << He3vec[i] << "," << TMath::Sqrt((c*c*He3BoilingError(He3part->getAvgI()))/TMath::Power(He3Boiling(He3part->getAvgI()),2.)) << endl;
      }
    }
    He3->Add(tmp3);

    delete He3part;
  }

  //Deuterium Yield
  for(unsigned int i=0; i<D2vec.size(); i++){
    yieldHistogram *D2part  = new yieldHistogram(Form("%s/kin%d%s/D2/%d.dat",folder.Data(),kin,set.Data(),D2vec[i]));
    
    TH1D *tmp3 = D2part->getTH1(Form("D2_%d",D2vec[i]));
    tmp3->Scale(1. / D2part->getLivetime());
    tmp3->Scale(1. / D2Nuclei(D2part->getAvgI()));
    D2charge += D2part->getCharge();
    for(int j=1; j<=nbins; j++){
      if(tmp3->GetBinContent(j)!=0){
        double c = tmp3->GetBinContent(j);
        double e = tmp3->GetBinError(j);
        tmp3->SetBinError(j,TMath::Sqrt((e*e)+(c*c*D2BoilingError(D2part->getAvgI())*D2BoilingError(D2part->getAvgI())/(D2Boiling(D2part->getAvgI())*D2Boiling(D2part->getAvgI())))));
        berr << "D2_" << D2vec[i] << "," << TMath::Sqrt((c*c*D2BoilingError(D2part->getAvgI())*D2BoilingError(D2part->getAvgI()))) << endl;
      }
    }
    D2->Add(tmp3);

    delete D2part;
  }
  berr.close();

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing - now taken care of when converting yieldHistogram to TH1
  He3->Scale(1./He3charge);
  D2 ->Scale(1./D2charge);
  
  //Endcap Contamination
  He3->Scale((1.-He3ECC(kin)));
  D2->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3->Scale(1./3.);
  D2 ->Scale(1./2.);
 
  ofstream perr(Form("%s/positron_err.txt",folder.Data()),ofstream::app);
  for(Int_t i=1; i<He3->GetNbinsX()+1; i++){
    double bin = He3->GetBinContent(i);
    double correction = (1. - He3Positrons(He3->GetBinCenter(i)));
    bin *= (1. - He3Positrons(He3->GetBinCenter(i)));
    He3->SetBinContent(i, bin);
    double e = He3->GetBinError(i);
    He3->SetBinError(i,TMath::Sqrt((e*e)+(bin*bin*He3PositronsError(He3->GetBinCenter(i))*He3PositronsError(He3->GetBinCenter(i))/(correction*correction))));
    perr << "He3_" << kin << set.Data() << "," << He3->GetBinCenter(i) << "," << TMath::Sqrt((bin*bin*He3PositronsError(He3->GetBinCenter(i))*He3PositronsError(He3->GetBinCenter(i))/(correction*correction))) << endl;
  }
  for(Int_t i=1; i<D2->GetNbinsX()+1; i++){
    double bin = D2->GetBinContent(i);
    double correction = (1. - D2Positrons(D2->GetBinCenter(i)));
    bin *= (1. - D2Positrons(D2->GetBinCenter(i)));
    D2->SetBinContent(i, bin);
    double e = D2->GetBinError(i);
    D2->SetBinError(i,TMath::Sqrt((e*e)+(bin*bin*D2PositronsError(D2->GetBinCenter(i))*D2PositronsError(D2->GetBinCenter(i))/(correction*correction))));
    perr << "D2_" << kin << set.Data() << "," << D2->GetBinCenter(i) << "," << TMath::Sqrt((bin*bin*D2PositronsError(D2->GetBinCenter(i))*D2PositronsError(D2->GetBinCenter(i))/(correction*correction))) << endl;
  }
  delete He3;
  delete D2;
}
