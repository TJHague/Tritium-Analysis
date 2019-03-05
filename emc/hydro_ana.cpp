/*
 *Main file for EMC analysis of tritium data. This file will do the H1/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void hydro_ana(Int_t kin, TString folder, Int_t nbins, Double_t low, Double_t high, Int_t iter=0){
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

  TString H1dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; H1dat += "H1_kin"; H1dat += kin; H1dat += set.Data(); H1dat += ".dat";
  TString D2dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; D2dat  += "D2_kin";  D2dat  += kin; D2dat  += set.Data(); D2dat  += ".dat";
  TString H1list = gGet_InputFile_Var(H1dat,2);
  TString D2list  = gGet_InputFile_Var(D2dat ,2);
  //cout << H1list << endl;
  vector<Int_t> H1vec = gGet_RunNoChain(H1list);
  vector<Int_t> D2vec  = gGet_RunNoChain(D2list);

  Double_t H1charge = 0;
  Double_t D2charge  = 0;
  //TH1D *H1full = new TH1D("H1full","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //TH1D *D2full  = new TH1D("D2full" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  //H1full->Sumw2();
  //D2full->Sumw2();
  //yieldHistogram *H1full = new yieldHistogram("Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //yieldHistogram *D2full  = new yieldHistogram("Full Kinematic Deuterium Yield",nbins,low,high);
  //yieldHistogram *EMCfull = new yieldHistogram("Full Kinematic EMC Ratio"      ,nbins,low,high);

  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<H1vec.size(); i++){
    //cout << "on run " << H1vec[i] << endl;
    TChain* T = LoadRun(H1vec[i],"T");

    yieldHistogram *H1part = new yieldHistogram("Partial Kinematic Hydrogen Yield",nbins,low,high);

    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2;
    //Double_t p[1] = {0};
    Int_t Iev=0;
    cout <<"ok1" << endl;
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    //PID variables
    T->SetBranchAddress("L.cer.asum_c",&cer);
    T->SetBranchAddress("L.prl1.e",&prl1);
    T->SetBranchAddress("L.prl2.e",&prl2);
    T->SetBranchAddress("L.tr.p",&p);
    T->SetBranchAddress("L.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("L.tr.tg_ph",&ph);
    T->SetBranchAddress("L.tr.tg_th",&th);
    T->SetBranchAddress("L.tr.tg_dp",&dp);
    T->SetBranchAddress("rpl.z",&z);

    T->SetBranchAddress("EKLx.x_bj",&x_bj);
    T->SetBranchAddress("EKLx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLx.W2"  ,&W2  );

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
          H1part->addCount(x_bj, Q2);
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
    //T->Draw("EKLx.x_bj>>H1part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    cout << lt << endl;
    H1part->setLivetime(lt);
    //cout << "H1 Livetime: " << lt << endl;
    cout << avgI << endl;
    H1part->setAvgI(avgI);
    //cout << "H1 Nuclei: " << H1Nuclei(avgI) << endl;
    cout << charge << endl;
    H1part->setCharge(charge);
    cout << "ok3" << endl;
    cout << folder << "/kin" << kin << set << "/H1/" << H1vec[i] << ".dat" << endl;
    cout << Form("%s/kin%d%s/H1/%d.dat",folder.Data(),kin,set.Data(),H1vec[i]) << endl;
    H1part->save(Form("%s/kin%d%s/H1/%d.dat",folder.Data(),kin,set.Data(),H1vec[i]));
    //TH1D *tmp = H1part->getTH1(Form("H1_%d",H1vec[i]));
    //tmp->Scale(1. / H1part->getLivetime());
    //tmp->Scale(1. / H1Nuclei(H1part->getAvgI()));
    H1charge += H1part->getCharge();
    //H1full->Add(tmp);

    //delete tmp;
    delete T;
    delete H1part;
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
    T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
    T->SetBranchAddress("LeftBCM.current_dnew",&I);
    T->SetBranchAddress("LeftBCM.isrenewed",&updated);
    T->SetBranchAddress("DL.bit2",&T2);
    T->SetBranchAddress("evLeftT2",&T2s);

    //PID variables
    T->SetBranchAddress("L.cer.asum_c",&cer);
    T->SetBranchAddress("L.prl1.e",&prl1);
    T->SetBranchAddress("L.prl2.e",&prl2);
    T->SetBranchAddress("L.tr.p",&p);
    T->SetBranchAddress("L.tr.n",&n);

    //Acceptance variables
    T->SetBranchAddress("L.tr.tg_ph",&ph);
    T->SetBranchAddress("L.tr.tg_th",&th);
    T->SetBranchAddress("L.tr.tg_dp",&dp);
    T->SetBranchAddress("rpl.z",&z);

    T->SetBranchAddress("EKLx.x_bj",&x_bj);
    T->SetBranchAddress("EKLx.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLx.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      if((updated==1) && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&(W2cut(W2)==true)){
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
  H1full->Scale(1./H1charge);
  D2full ->Scale(1./D2charge);

  //Endcap Contamination
  H1full->Scale((1.-H1ECC(kin)));
  D2full ->Scale((1.-D2ECC(kin)));

  //Scale per nucleon
  H1full->Scale(1./3.);
  D2full ->Scale(1./2.);

 for(Int_t i=1; i<H1full->GetNbinsX()+1; i++){
    Double_t bin = H1full->GetBinContent(i);
    bin *= (1. - H1Positrons(H1full->GetBinCenter(i)));
    H1full->SetBinContent(i, bin);
  }
  for(Int_t i=1; i<D2full->GetNbinsX()+1; i++){
    Double_t bin = D2full->GetBinContent(i);
    bin *= (1. - D2Positrons(D2full->GetBinCenter(i)));
    D2full->SetBinContent(i, bin);
  }

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratio->Sumw2();
  ratio->Add(H1full);
  ratio->Divide(D2full);

  TFile *f = new TFile(Form("%s/fullKin/kin%d%s.root",folder.Data(),kin,set.Data()),"RECREATE");
  H1full->Write();
  D2full->Write();
  ratio->Write();
  
  delete f;
  delete H1full;
  delete D2full;
  delete ratio;*/
}
