/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void emcR(Int_t kin, TString folder, Int_t nbins, Double_t low, Double_t high, Int_t iter=0){
  Int_t arm=1;
  //Load runs
  TString set="";
  if(kin>=7){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
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
  //TH1D *He3full = new TH1D("He3full","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //TH1D *D2full  = new TH1D("D2full" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  //He3full->Sumw2();
  //D2full->Sumw2();
  //yieldHistogram *He3full = new yieldHistogram("Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //yieldHistogram *D2full  = new yieldHistogram("Full Kinematic Deuterium Yield",nbins,low,high);
  //yieldHistogram *EMCfull = new yieldHistogram("Full Kinematic EMC Ratio"      ,nbins,low,high);

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<He3vec.size(); i++){
    //cout << "on run " << He3vec[i] << endl;
    TChain* T = LoadRun(He3vec[i],"T");

    yieldHistogram *He3part = new yieldHistogram("Partial Kinematic Helium-3 Yield",nbins,low,high);

    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2;
    //Double_t p[1] = {0};
    Int_t Iev=0;
    cout <<"ok1" << endl;

    T->SetBranchAddress("RightBCM.charge_dnew",&Q);
    T->SetBranchAddress("RightBCM.current_dnew",&I);
    T->SetBranchAddress("RightBCM.isrenewed",&updated);
    T->SetBranchAddress("DR.bit5",&T2);
    T->SetBranchAddress("evRightT5",&T2s);

    //PID variables
    T->SetBranchAddress("R.cer.asum_c",&cer);
    T->SetBranchAddress("R.ps.e",&prl1);
    T->SetBranchAddress("R.sh.e",&prl2);
    T->SetBranchAddress("R.tr.p",&p);
    T->SetBranchAddress("R.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("R.tr.tg_ph",&ph);
    T->SetBranchAddress("R.tr.tg_th",&th);
    T->SetBranchAddress("R.tr.tg_dp",&dp);
    T->SetBranchAddress("rpr.z",&z);

    T->SetBranchAddress("EKRx.x_bj",&x_bj);
    T->SetBranchAddress("EKRx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKRx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    //cout << events << endl;
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    cout << "ok2" << endl;
    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if((updated==1) && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          He3part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
      if(j%10000==0){
        cout << j << endl;
      } 
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>He3part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    He3part->setLivetime(lt);
    //cout << "He3 Livetime: " << lt << endl;
    He3part->setAvgI(avgI);
    //cout << "He3 Nuclei: " << He3Nuclei(avgI) << endl;
    He3part->setCharge(charge);
    cout << "ok3" << endl;
    cout << folder << "/kin" << kin << set << "/He3/" << He3vec[i] << ".dat" << endl;
    He3part->save(Form("%s/kin%d%s/He3/%d.dat",folder.Data(),kin,set.Data(),He3vec[i]));
    //TH1D *tmp = He3part->getTH1(Form("He3_%d",He3vec[i]));
    //tmp->Scale(1. / He3part->getLivetime());
    //tmp->Scale(1. / He3Nuclei(He3part->getAvgI()));
    He3charge += He3part->getCharge();
    //He3full->Add(tmp);

    //delete tmp;
    delete T;
    delete He3part;
  }

  //Deuterium Yield
  for(Int_t i=0; i<D2vec.size(); i++){
    TChain* T = LoadRun(D2vec[i],"T");

    yieldHistogram *D2part  = new yieldHistogram("Partial Kinematic Deuterium Yield",nbins,low,high);
    
    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, p, ph, th, dp, z, x_bj, Q2, n, W2;
    Int_t Iev = 0;
    T->SetBranchAddress("RightBCM.charge_dnew",&Q);
    T->SetBranchAddress("RightBCM.current_dnew",&I);
    T->SetBranchAddress("RightBCM.isrenewed",&updated);
    T->SetBranchAddress("DR.bit5",&T2);
    T->SetBranchAddress("evRightT5",&T2s);

    //PID variables
    T->SetBranchAddress("R.cer.asum_c",&cer);
    T->SetBranchAddress("R.ps.e",&prl1);
    T->SetBranchAddress("R.sh.e",&prl2);
    T->SetBranchAddress("R.tr.p",&p);
    T->SetBranchAddress("R.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("R.tr.tg_ph",&ph);
    T->SetBranchAddress("R.tr.tg_th",&th);
    T->SetBranchAddress("R.tr.tg_dp",&dp);
    T->SetBranchAddress("rpr.z",&z);

    T->SetBranchAddress("EKRx.x_bj",&x_bj);
    T->SetBranchAddress("EKRx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKRx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if(updated && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          D2part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>D2part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    D2part->setLivetime(lt);
    //cout << "D2 Livetime: " << lt << endl;
    D2part->setAvgI(avgI);
    //cout << "D2 Nuclei: " << D2Nuclei(avgI) << endl;
    D2part->setCharge(charge);
    D2part->save(Form("%s/kin%d%s/D2/%d.dat",folder.Data(),kin,set.Data(),D2vec[i]));
    //TH1D *tmp = D2part->getTH1(Form("D2_%d",D2vec[i]));
    //tmp->Scale(1. / D2part->getLivetime());
    //tmp->Scale(1. / D2Nuclei(D2part->getAvgI()));
    D2charge += D2part->getCharge();
    //D2full->Add(tmp);

    //delete tmp;
    delete T;
    delete D2part;
  }

  /****************************************************************************
  * End the loop
  * Now start processing the results
  ****************************************************************************/

  /*//Charge normalizing - now taken care of when converting yieldHistogram to TH1
  He3full->Scale(1./He3charge);
  D2full ->Scale(1./D2charge);

  //Endcap Contamination
  He3full->Scale((1.-He3ECC(kin)));
  D2full ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  He3full->Scale(1./3.);
  D2full ->Scale(1./2.);

 for(Int_t i=1; i<He3full->GetNbinsX()+1; i++){
    Double_t bin = He3full->GetBinContent(i);
    bin *= (1. - He3Positrons(He3full->GetBinCenter(i)));
    He3full->SetBinContent(i, bin);
  }
  for(Int_t i=1; i<D2full->GetNbinsX()+1; i++){
    Double_t bin = D2full->GetBinContent(i);
    bin *= (1. - D2Positrons(D2full->GetBinCenter(i)));
    D2full->SetBinContent(i, bin);
  }

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratio->Sumw2();
  ratio->Add(He3full);
  ratio->Divide(D2full);

  TFile *f = new TFile(Form("%s/fullKin/kin%d%s.root",folder.Data(),kin,set.Data()),"RECREATE");
  He3full->Write();
  D2full->Write();
  ratio->Write();

  delete f;
  delete He3full;
  delete D2full;
  delete ratio;*/
}
