/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

//Kin should be integer of kinematic
void ev_scan(Int_t kin, TString targ, TString folder, Int_t iter=0){
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

  TString datfile = "/work/halla/triton/tjhague/replay_tritium/replay/scripts/Runlist/"; datfile += targ; datfile += "_kin"; datfile += kin; datfile += set.Data(); datfile += ".dat";
  TString runlist = gGet_InputFile_Var(datfile,2);
  vector<Int_t> runvec = gGet_RunNoChain(runlist);

  ofstream run_info(Form("%s/run_info.csv",folder.Data()),ofstream::app);
  run_info << "Run" << "," << "Livetime" << "," << "Average Current" << "," << "Charge" << endl;
  /****************************************************************************
  * Here is where it needs to turn into a loop over each run.
  * Probably start with Helium-3 then go to Deuterium
  ****************************************************************************/
  
  //Helium-3 Yield
  for(Int_t i=0; i<runvec.size(); i++){
    TChain* T = LoadRun(runvec[i],"T");
    ofstream event_list(Form("%s/%d.csv",folder.Data(),runvec[i]),ofstream::trunc);
    event_list << "Cherenkov" << "," << "Preshower" << "," << "Shower" << "," << "Momentum" << "," << "Phi" << "," << "Theta" << "," << "dp" << "," << "Z" << "," << "Bjorkan X" << "," << "Q2" << "," << "W2" << endl;

    Double_t Q, I, updated, T2, T2s, avgI=0, cer, prl1, prl2, ph, th, dp, z, x_bj, Q2, n, p, W2;
    Int_t Iev=0;
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

    T->SetBranchAddress("EKLxe.x_bj",&x_bj);
    T->SetBranchAddress("EKLxe.Q2"  ,&Q2  );
    T->SetBranchAddress("EKLxe.W2"  ,&W2  );

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
        if((PID(cer, prl1, prl2, p, n, arm)==true)&&(ACC(ph, th, dp, arm)==true)&&(EC(z, arm)==true)&&(W2cut(W2)==true)){
          event_list << cer << "," << prl1 << "," << prl2 << "," << p << "," << ph << "," << th << "," << dp << "," << z << "," << x_bj << "," << Q2 << "," << W2 << endl;
        }
      }
      if(j==(events-1)){
        trig_scal = T2s;
      }
    }
    Double_t lt = trig_rec/trig_scal;
    avgI /= Iev;
    run_info << runvec[i] << "," << lt << "," << avgI << "," << charge << endl;

    delete T;
  }
}
