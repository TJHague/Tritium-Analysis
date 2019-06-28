/*
 *Main file for EMC analysis of tritium data. This file will do the H3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
//Arm - 0 is left, right otherwise
void process_other(Int_t kin, TString folder, Int_t nbins, Double_t low, Double_t high, Int_t iter=0){
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

  TString H3dat = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; H3dat += "H3_kin"; H3dat += kin; H3dat += set.Data(); H3dat += ".dat";
  TString H3list = gGet_InputFile_Var(H3dat,2);
  //cout << H3list << endl;
  vector<Int_t> H3vec = gGet_RunNoChain(H3list);
  TString H1dat, H1list;
  vector<Int_t> H1vec;
  if(kin<=4){
    H1dat  = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; H1dat  += "H1_kin";  H1dat  += kin; H1dat  += set.Data(); H1dat  += ".dat";
    H1list  = gGet_InputFile_Var(H1dat ,2);
    H1vec  = gGet_RunNoChain(H1list);
  }

  Double_t H3charge = 0;
  Double_t H1charge  = 0;

  Double_t bcm_gain = 0.00033518;
  Double_t bcm_offset = 0.0217;
  Double_t clock_rate = 103700.0;

  //TH1D *H3full = new TH1D("H3full","Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //TH1D *H1full  = new TH1D("H1full" ,"Full Kinematic Deuterium Yield",nbins,low,high);
  //H3full->Sumw2();
  //H1full->Sumw2();
  //yieldHistogram *H3full = new yieldHistogram("Full Kinematic Helium-3 Yield" ,nbins,low,high);
  //yieldHistogram *H1full  = new yieldHistogram("Full Kinematic Deuterium Yield",nbins,low,high);
  //yieldHistogram *EMCfull = new yieldHistogram("Full Kinematic EMC Ratio"      ,nbins,low,high);

  /****************************************************************************
   * Here is where it needs to turn into a loop over each run.
   * Probably start with Helium-3 then go to Deuterium
   ****************************************************************************/

  //Helium-3 Yield
  for(unsigned int i=0; i<H3vec.size(); i++){
    //cout << "on run " << H3vec[i] << endl;
    TChain* T = LoadRun(H3vec[i],"T");

    yieldHistogram *H3part = new yieldHistogram("Partial Kinematic Helium-3 Yield",nbins,low,high);

    //Calculate charge and live time
    //Todo:
    //  Make this work for both arms
    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, x_bj, Q2, n, W2, dnew, drate, clock;
    Double_t z[100]={0}, p[100]={0}, ph[100]={0}, th[100]={0}, dp[100]={0};
    //Double_t p[1] = {0};
    Int_t Iev=0;
    cout <<"ok1" << endl;
    /*T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
      T->SetBranchAddress("LeftBCM.current_dnew",&I);
      T->SetBranchAddress("LeftBCM.isrenewed",&updated);*/
    T->SetBranchAddress("evLeftdnew",&dnew);
    cout << "blah ";
    T->SetBranchAddress("evLeftdnew_r",&drate);
    cout << "blah1 ";
    T->SetBranchAddress("V1495ClockCount",&clock);
    cout << "blah2 ";
    T->SetBranchAddress("DL.bit2",&T2);
    cout << "blah3" << endl;
    T->SetBranchAddress("evLeftT2",&T2s);

    //PID variables
    T->SetBranchAddress("L.cer.asum_c",&cer);
    T->SetBranchAddress("L.prl1.e",&prl1);
    T->SetBranchAddress("L.prl2.e",&prl2);
    T->SetBranchAddress("L.tr.p",p);
    T->SetBranchAddress("L.tr.n",&n);


    //Acceptance variables
    T->SetBranchAddress("L.tr.tg_ph",ph);
    T->SetBranchAddress("L.tr.tg_th",th);
    T->SetBranchAddress("L.tr.tg_dp",dp);
    T->SetBranchAddress("rpl.z",z);

    T->SetBranchAddress("EKLxe.x_bj",&x_bj);
    T->SetBranchAddress("EKLxe.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLxe.W2"  ,&W2  );

    Int_t events = T->GetEntries();
    //cout << events << endl;
    Double_t trig_rec  = 0;
    Double_t trig_scal = 0;
    Double_t charge    = 0;

    cout << "ok2" << endl;
    for(Int_t j=0; j<events; j++){
      T->GetEntry(j);
      /*if((updated==1) && I>0.){
        charge += Q;
        avgI += I;
        Iev++;
        }*/
      if(drate>0.&&drate<100000){//Maximum prevents bad readings from messing up calculation
        avgI += drate*bcm_gain;
        Iev++;
      }
      if(T2==1){
        trig_rec++;
        if((PID(cer, prl1, prl2, p[0], n, arm)==true)&&(ACC(ph[0], th[0], dp[0], 0, 0, arm)==true)&&(EC(z[0], kin)==true)&&(W2cut(W2)==true)&&drate>0.){
          H3part->addCount(x_bj, Q2);
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
        charge = (dnew*bcm_gain) + (clock*bcm_offset/clock_rate);
      }
      if(j%10000==0){
        cout << j << endl;
      } 
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    //T->Draw("EKLx.x_bj>>H3part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
    cout << lt << endl;
    H3part->setLivetime(lt);
    //cout << "H3 Livetime: " << lt << endl;
    cout << avgI << endl;
    H3part->setAvgI(avgI);
    //cout << "H3 Nuclei: " << H3Nuclei(avgI) << endl;
    cout << charge << endl;
    H3part->setCharge(charge);
    cout << "ok3" << endl;
    cout << folder << "/kin" << kin << set << "/H3/" << H3vec[i] << ".dat" << endl;
    cout << Form("%s/kin%d%s/H3/%d.dat",folder.Data(),kin,set.Data(),H3vec[i]) << endl;
    H3part->save(Form("%s/kin%d%s/H3/%d.dat",folder.Data(),kin,set.Data(),H3vec[i]));
    //TH1D *tmp = H3part->getTH1(Form("H3_%d",H3vec[i]));
    //tmp->Scale(1. / H3part->getLivetime());
    //tmp->Scale(1. / H3Nuclei(H3part->getAvgI()));
    H3charge += H3part->getCharge();
    //H3full->Add(tmp);

    //delete tmp;
    delete T;
    delete H3part;
  }

  if(kin<=4){
    for(unsigned int i=0; i<H1vec.size(); i++){
      TChain* T = LoadRun(H1vec[i],"T");

      yieldHistogram *H1part  = new yieldHistogram("Partial Kinematic Deuterium Yield",nbins,low,high);

      //Calculate charge and live time
      //Todo:
      //  Make this work for both arms
      Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, x_bj, Q2, n, W2, dnew, drate, clock;
      Double_t z[100]={0}, p[100]={0}, ph[100]={0}, th[100]={0}, dp[100]={0};
      Int_t Iev = 0;
      /*T->SetBranchAddress("LeftBCM.charge_dnew",&Q);
        T->SetBranchAddress("LeftBCM.current_dnew",&I);
        T->SetBranchAddress("LeftBCM.isrenewed",&updated);*/
      T->SetBranchAddress("evLeftdnew",&dnew);
      T->SetBranchAddress("evLeftdnew_r",&drate);
      T->SetBranchAddress("V1495ClockCount",&clock);
      T->SetBranchAddress("DL.bit2",&T2);
      T->SetBranchAddress("evLeftT2",&T2s);

      //PID variables
      T->SetBranchAddress("L.cer.asum_c",&cer);
      T->SetBranchAddress("L.prl1.e",&prl1);
      T->SetBranchAddress("L.prl2.e",&prl2);
      T->SetBranchAddress("L.tr.p",p);
      T->SetBranchAddress("L.tr.n",&n);

      //Acceptance variables
      T->SetBranchAddress("L.tr.tg_ph",ph);
      T->SetBranchAddress("L.tr.tg_th",th);
      T->SetBranchAddress("L.tr.tg_dp",dp);
      T->SetBranchAddress("rpl.z",z);

      T->SetBranchAddress("EKLxe.x_bj",&x_bj);
      T->SetBranchAddress("EKLxe.Q2"  ,&Q2  );
      T->SetBranchAddress("EKLxe.W2"  ,&W2  );

      Int_t events = T->GetEntries();
      Double_t trig_rec  = 0;
      Double_t trig_scal = 0;
      Double_t charge    = 0;

      for(Int_t j=0; j<events; j++){
        T->GetEntry(j);
        /*if((updated==1) && I>0.){
          charge += Q;
          avgI += I;
          Iev++;
          }*/
        if(drate>0.&&drate<100000){//Maximum prevents bad readings from messing up calculation
          avgI += drate*bcm_gain;
          Iev++;
        }
        if(T2==1){
          trig_rec++;
          if((PID(cer, prl1, prl2, p[0], n, arm)==true)&&(ACC(ph[0], th[0], dp[0], arm)==true)&&(EC(z[0], kin)==true)&(W2cut(W2)==true)&&drate>0.){
            H1part->addCount(x_bj, Q2);
          }
        }
        if(j==(events-1)){
          trig_scal = T2s;
          charge = (dnew*bcm_gain) + (clock*bcm_offset/clock_rate);
        }
      }
      Double_t lt = trig_rec/trig_scal;
      avgI /= Iev;
      //T->Draw("EKLx.x_bj>>H1part",PID(arm)+ACC(arm)+EC(arm)+Trig2(arm),"");
      H1part->setLivetime(lt);
      //cout << "H1 Livetime: " << lt << endl;
      H1part->setAvgI(avgI);
      //cout << "H1 Nuclei: " << H1Nuclei(avgI) << endl;
      H1part->setCharge(charge);
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
  }

  /****************************************************************************
   * End the loop
   * Now start processing the results
   ****************************************************************************/

  /*//Charge normalizing - now taken care of when converting yieldHistogram to TH1
    H3full->Scale(1./H3charge);
    H1full ->Scale(1./H1charge);

  //Endcap Contamination
  H3full->Scale((1.-H3ECC(kin)));
  H1full ->Scale((1.-H1ECC(kin)));

  //Scale per nucleon
  H3full->Scale(1./3.);
  H1full ->Scale(1./2.);

  for(Int_t i=1; i<H3full->GetNbinsX()+1; i++){
  Double_t bin = H3full->GetBinContent(i);
  bin *= (1. - H3Positrons(H3full->GetBinCenter(i)));
  H3full->SetBinContent(i, bin);
  }
  for(Int_t i=1; i<H1full->GetNbinsX()+1; i++){
  Double_t bin = H1full->GetBinContent(i);
  bin *= (1. - H1Positrons(H1full->GetBinCenter(i)));
  H1full->SetBinContent(i, bin);
  }

  TH1D *ratio = new TH1D("emc",Form("Kinematic %d EMC Ratio",kin),nbins,low,high);

  ratio->Sumw2();
  ratio->Add(H3full);
  ratio->Divide(H1full);

  TFile *f = new TFile(Form("%s/fullKin/kin%d%s.root",folder.Data(),kin,set.Data()),"RECREATE");
  H3full->Write();
  H1full->Write();
  ratio->Write();

  delete f;
  delete H3full;
  delete H1full;
  delete ratio;*/
}
