/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void ind_cor(Int_t kin, TString folder, Int_t nbins, Double_t low, Double_t high, Int_t iter=0){
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
  vector<Double_t> He3boilerr;
  vector<Double_t> D2boilerr;
  He3boilerr.resize(nbins,0.);
  D2boilerr.resize(nbins,0.);
  TH1D *He3 = new TH1D("He3","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *He3nocor = new TH1D("He3nocor","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *He3lt = new TH1D("He3lt","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *He3boiling = new TH1D("He3boiling","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *He3pos = new TH1D("He3pos","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *He3ecc = new TH1D("He3ecc","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  TH1D *D2  = new TH1D("D2" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  TH1D *D2nocor  = new TH1D("D2nocor" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  TH1D *D2lt  = new TH1D("D2lt" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  TH1D *D2boiling  = new TH1D("D2boiling" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  TH1D *D2pos  = new TH1D("D2pos" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  TH1D *D2ecc  = new TH1D("D2ecc" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  He3->Sumw2();
  He3lt ->Sumw2();
  He3boiling->Sumw2();
  He3pos->Sumw2();
  D2->Sumw2();
  D2lt->Sumw2();
  D2boiling->Sumw2();
  D2pos->Sumw2();

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(unsigned int i=0; i<He3vec.size(); i++){
    yieldHistogram *He3part = new yieldHistogram(Form("%s/kin%d%s/He3/%d.dat",folder.Data(),kin,set.Data(),He3vec[i]));

    TH1D *tmp = He3part->getTH1(Form("He3_%d",He3vec[i]));
    TH1D *tmp2 = (TH1D*) tmp->Clone();
    TH1D *tmp3 = (TH1D*) tmp->Clone();
    tmp->Scale(1. / He3Nuclei(0));
    He3nocor->Add(tmp);
    He3ecc->Add(tmp);
    He3pos->Add(tmp);
    tmp->Scale(1. / He3part->getLivetime());
    tmp3->Scale(1. / He3part->getLivetime());
    He3lt->Add(tmp);
    tmp3->Scale(1. / He3Nuclei(He3part->getAvgI()));
    tmp2->Scale(1. / He3Nuclei(He3part->getAvgI()));
    He3boiling->Add(tmp2);
    He3charge += He3part->getCharge();
    for(int j=1; j<=nbins; j++){
      if(tmp3->GetBinContent(j)!=0){
        double c = tmp3->GetBinContent(j);
        double e = tmp3->GetBinError(j);
        //The errors are correlated run-to-run so they must be summed and added to the total yield
        //tmp3->SetBinError(j,TMath::Sqrt((e*e)+(c*c*He3BoilingError(He3part->getAvgI())/TMath::Power(He3Boiling(He3part->getAvgI()),2.))));
        He3boilerr[j-1] += TMath::Sqrt(He3BoilingError(He3part->getAvgI()))*c/He3Boiling(He3part->getAvgI());
      }
    }
    He3->Add(tmp3);

    delete tmp;
    delete tmp2;
    delete He3part;
  }

  //Deuterium Yield
  for(unsigned int i=0; i<D2vec.size(); i++){
    yieldHistogram *D2part  = new yieldHistogram(Form("%s/kin%d%s/D2/%d.dat",folder.Data(),kin,set.Data(),D2vec[i]));
    
    TH1D *tmp = D2part->getTH1(Form("D2_%d",D2vec[i]));
    TH1D *tmp2 = (TH1D*) tmp->Clone();
    TH1D *tmp3 = (TH1D*) tmp->Clone();
    tmp->Scale(1. / D2Nuclei(0));
    D2nocor->Add(tmp);
    D2ecc->Add(tmp);
    D2pos->Add(tmp);
    tmp->Scale(1. / D2part->getLivetime());
    tmp3->Scale(1. / D2part->getLivetime());
    D2lt->Add(tmp);
    tmp3->Scale(1. / D2Nuclei(D2part->getAvgI()));
    tmp2->Scale(1. / D2Nuclei(D2part->getAvgI()));
    D2boiling->Add(tmp2);
    D2charge += D2part->getCharge();
    for(int j=1; j<=nbins; j++){
      if(tmp3->GetBinContent(j)!=0){
        double c = tmp3->GetBinContent(j);
        double e = tmp3->GetBinError(j);
        //The errors are correlated run-to-run so they must be summed and added to the total yield
        //tmp3->SetBinError(j,TMath::Sqrt((e*e)+(c*c*D2BoilingError(D2part->getAvgI())/TMath::Power(D2Boiling(D2part->getAvgI()),2.))));
        D2boilerr[j-1] += TMath::Sqrt(D2BoilingError(D2part->getAvgI()))*c/D2Boiling(D2part->getAvgI());
      }
    }
    D2->Add(tmp3);

    delete tmp;
    delete tmp2;
    delete D2part;
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  //Charge normalizing - now taken care of when converting yieldHistogram to TH1
  He3->Scale(1./He3charge);
  He3nocor->Scale(1./He3charge);
  He3lt->Scale(1./He3charge);
  He3boiling->Scale(1./He3charge);
  He3ecc->Scale(1./He3charge);
  He3pos->Scale(1./He3charge);
  D2 ->Scale(1./D2charge);
  D2nocor ->Scale(1./D2charge);
  D2lt ->Scale(1./D2charge);
  D2boiling ->Scale(1./D2charge);
  D2ecc->Scale(1./D2charge);
  D2pos ->Scale(1./D2charge);

  //Endcap Contamination
  He3->Scale((1.-He3ECC(kin)));
  D2->Scale((1.-D2ECC(kin)));
  He3ecc->Scale((1.-He3ECC(kin)));
  D2ecc ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3->Scale(1./3.);
  He3nocor->Scale(1./3.);
  He3lt->Scale(1./3.);
  He3boiling->Scale(1./3.);
  He3pos->Scale(1./3.);
  He3ecc->Scale(1./3.);
  D2 ->Scale(1./2.);
  D2nocor ->Scale(1./2.);
  D2lt ->Scale(1./2.);
  D2boiling ->Scale(1./2.);
  D2pos ->Scale(1./2.);
  D2ecc ->Scale(1./2.);

  //ofstream berr(Form("%s/boiling_err.txt",folder.Data()),ofstream::app);
  for(Int_t i=1; i<He3->GetNbinsX()+1; i++){
    Double_t bin = He3pos->GetBinContent(i);
    bin *= (1. - He3Positrons(He3pos->GetBinCenter(i)));
    He3pos->SetBinContent(i, bin);
    He3pos->SetBinError(i, He3pos->GetBinError(i)*(1. - He3Positrons(He3pos->GetBinCenter(i))));
    bin = He3->GetBinContent(i);
    double correction = (1. - He3Positrons(He3->GetBinCenter(i)));
    bin *= correction;
    He3->SetBinContent(i, bin);
    double e = He3->GetBinError(i);
    //Scale the error with corrections applied after calculation
    He3boilerr[i-1] /= He3charge;
    He3boilerr[i-1] *= (1-He3ECC(kin));
    He3boilerr[i-1] /= 3.;
    He3boilerr[i-1] *= (1. - He3Positrons(He3->GetBinCenter(i)));
    He3->SetBinError(i,TMath::Sqrt((e*e*correction*correction) + (He3boilerr[i-1]*He3boilerr[i-1]) + (bin*bin*He3PositronsError(He3->GetBinCenter(i))/(correction*correction))));
    //He3->SetBinError(i,TMath::Sqrt((e*e) + (He3boilerr[i-1]*He3boilerr[i-1])));
    //berr << "He3_" << kin << set.Data() << "," << He3->GetBinCenter(i) << "," << bin << "," << He3boilerr[i-1] << endl;
    //He3->SetBinError(i,TMath::Sqrt((e*e)+(bin*bin*He3PositronsError(He3->GetBinCenter(i))*He3PositronsError(He3->GetBinCenter(i))/(correction*correction))));
  }
  for(Int_t i=1; i<D2->GetNbinsX()+1; i++){
    Double_t bin = D2pos->GetBinContent(i);
    bin *= (1. - D2Positrons(D2pos->GetBinCenter(i)));
    D2pos->SetBinContent(i, bin);
    D2pos->SetBinError(i, D2pos->GetBinError(i)*(1. - D2Positrons(D2pos->GetBinCenter(i))));
    bin = D2->GetBinContent(i);
    double correction = (1. - D2Positrons(D2->GetBinCenter(i)));
    bin *= correction;
    D2->SetBinContent(i, bin);
    double e = D2->GetBinError(i);
    D2boilerr[i-1] /= D2charge;
    D2boilerr[i-1] *= (1-D2ECC(kin));
    D2boilerr[i-1] /= 3.;
    D2boilerr[i-1] *= (1. - D2Positrons(D2->GetBinCenter(i)));
    D2->SetBinError(i,TMath::Sqrt((e*e*correction*correction) + (D2boilerr[i-1]*D2boilerr[i-1]) + (bin*bin*D2PositronsError(D2->GetBinCenter(i))/(correction*correction))));
    //D2->SetBinError(i,TMath::Sqrt((e*e) + (D2boilerr[i-1]*D2boilerr[i-1])));
    //berr << "D2_" << kin << set.Data() << "," << D2->GetBinCenter(i) << "," << bin << "," << D2boilerr[i-1] << endl;
    //D2->SetBinError(i,TMath::Sqrt((e*e)+(bin*bin*D2PositronsError(D2->GetBinCenter(i))*D2PositronsError(D2->GetBinCenter(i))/(correction*correction))));
  }

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratio->Sumw2();
  ratio->Add(He3);
  ratio->Divide(D2);

  TH1D *rationocor = new TH1D("emcnocor",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  rationocor->Sumw2();
  rationocor->Add(He3nocor);
  rationocor->Divide(D2nocor);

  TH1D *ratiolt = new TH1D("emclt",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratiolt->Sumw2();
  ratiolt->Add(He3lt);
  ratiolt->Divide(D2lt);

  TH1D *ratioboiling = new TH1D("emcboiling",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratioboiling->Sumw2();
  ratioboiling->Add(He3boiling);
  ratioboiling->Divide(D2boiling);

  TH1D *ratiopos = new TH1D("emcpos",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratiopos->Sumw2();
  ratiopos->Add(He3pos);
  ratiopos->Divide(D2pos);

  TH1D *ratioecc = new TH1D("emcecc",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratioecc->Sumw2();
  ratioecc->Add(He3ecc);
  ratioecc->Divide(D2ecc);

  TFile *f = new TFile(Form("%s/fullKin/kin%d%s.root",folder.Data(),kin,set.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  ratio->Write();
  He3nocor->Write();
  D2nocor->Write();
  rationocor->Write();
  He3lt->Write();
  D2lt->Write();
  ratiolt->Write();
  He3boiling->Write();
  D2boiling->Write();
  ratioboiling->Write();
  He3pos->Write();
  D2pos->Write();
  ratiopos->Write();
  He3ecc->Write();
  D2ecc->Write();
  ratioecc->Write();
  
  delete f;
  delete He3;
  delete D2;
  delete ratio;
  delete He3lt;
  delete D2lt;
  delete ratiolt;
  delete He3boiling;
  delete D2boiling;
  delete ratioboiling;
  delete He3pos;
  delete D2pos;
  delete ratiopos;
  delete He3ecc;
  delete D2ecc;
  delete ratioecc;
  delete He3nocor;
  delete D2nocor;
  delete rationocor;
}
