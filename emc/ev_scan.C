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
  vector<Int_t> vec = gGet_RunNoChain(runlist);
  int size = vec.size();
  Int_t runvec[200]; 
  for(int i=0;i<size;i++){
    runvec[i] = vec[i];
    cout << vec[i] << endl;
    cout << runvec[i] << endl;
  }

  /*ofstream run_info(Form("%s/run_info.csv",folder.Data()),ofstream::app);
    run_info << "Run" << "," << "Livetime" << "," << "Average Current" << "," << "Charge" << endl;*/
  /****************************************************************************
   * Here is where it needs to turn into a loop over each run.
   * Probably start with Helium-3 then go to Deuterium
   ****************************************************************************/

  ofstream list(Form("%s/list.dat",folder.Data()),ofstream::app);
  ofstream missing(Form("%s/missing.dat",folder.Data()),ofstream::app);
  //Helium-3 Yield
  for(Int_t i=0; i<size; i++){
    cout << vec[i] << endl;
    TChain* T = LoadRun(vec[i],"T");
    list << vec[i] << endl;
    if(gSystem->AccessPathName(Form("%s/%d.csv",folder.Data(),vec[i]))){
      missing << vec[i] << endl;

      ofstream event_list(Form("%s/%d.csv",folder.Data(),vec[i]),ofstream::trunc);
      event_list << "Cherenkov,Preshower,Shower,Momentum,nTracks,Phi,Theta,dp,Z,x,Q2,W2,x no EL,Q2 no EL,W2 no EL,dnew,dnew_rate,clock,T2,T2scalar" << endl;
      //run_info << runvec[i] << "," << endl;

      Double_t T2, T2s, avgI=0, cer, prl1, prl2, x_bj, Q2, n, W2, xnel, Q2nel, W2nel, clock, dnew, dnew_rate;
      Double_t z[100]={0}, ph[100]={0}, th[100]={0}, dp[100]={0}, p[100]={0};

      if(kin!=16){
        //Int_t Iev=0;
        T->SetBranchAddress("V1495ClockCount",&clock);
        T->SetBranchAddress("evLeftdnew",&dnew);
        T->SetBranchAddress("evLeftdnew_r",&dnew_rate);
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

        T->SetBranchAddress("EKLx.x_bj",&xnel );
        T->SetBranchAddress("EKLx.Q2"  ,&Q2nel);
        T->SetBranchAddress("EKLx.W2"  ,&W2nel);
      }else{
        //Int_t Iev=0;
        T->SetBranchAddress("V1495ClockCount",&clock);
        T->SetBranchAddress("evRightdnew",&dnew);
        T->SetBranchAddress("evRightdnew_r",&dnew_rate);
        T->SetBranchAddress("DR.bit5",&T2);
        T->SetBranchAddress("evRightT5",&T2s);

        //PID variables
        T->SetBranchAddress("R.cer.asum_c",&cer);
        T->SetBranchAddress("R.ps.e",&prl1);
        T->SetBranchAddress("R.sh.e",&prl2);
        T->SetBranchAddress("R.tr.p",p);
        T->SetBranchAddress("R.tr.n",&n);

        //Acceptance variables
        T->SetBranchAddress("R.tr.tg_ph",ph);
        T->SetBranchAddress("R.tr.tg_th",th);
        T->SetBranchAddress("R.tr.tg_dp",dp);
        T->SetBranchAddress("rpr.z",z);

        T->SetBranchAddress("EKRxe.x_bj",&x_bj);
        T->SetBranchAddress("EKRxe.Q2"  ,&Q2  );
        T->SetBranchAddress("EKRxe.W2"  ,&W2  );

        T->SetBranchAddress("EKRx.x_bj",&xnel );
        T->SetBranchAddress("EKRx.Q2"  ,&Q2nel);
        T->SetBranchAddress("EKRx.W2"  ,&W2nel);
      }

      Int_t events = T->GetEntries();
      //Double_t trig_rec  = 0;
      //Double_t trig_scal = 0;
      //Double_t charge    = 0;

      cout << "start of loop" << endl;
      for(Int_t j=0; j<events; j++){
        /*      if(j%10000==0){
                cout << j << " working..." << endl;
                event_list.close();
                event_list.open(Form("%s/%d.csv",folder.Data(),runvec[i]),ofstream::trunc);
                }*/

        T->GetEntry(j);

        event_list << double(cer);
        event_list << ",";
        event_list << double(prl1);
        event_list << ",";
        event_list << double(prl2);
        event_list << ",";
        event_list << double(p[0]);
        event_list << ",";
        event_list << double(n);
        event_list << ",";
        event_list << double(ph[0]);
        event_list << ",";
        event_list << double(th[0]);
        event_list << ",";
        event_list << double(dp[0]);
        event_list << ",";
        event_list << double(z[0]);
        event_list << ",";
        event_list << double(x_bj);
        event_list << ",";
        event_list << double(Q2);
        event_list << ",";
        event_list << double(W2);
        event_list << ",";
        event_list << double(xnel);
        event_list << ",";
        event_list << double(Q2nel);
        event_list << ",";
        event_list << double(W2nel);
        event_list << ",";
        event_list << double(clock);
        event_list << ",";
        event_list << double(dnew);
        event_list << ",";
        event_list << double(dnew_rate);
        event_list << ",";
        event_list << double(T2);
        event_list << ",";
        event_list << double(T2s);
        event_list << endl;
      }
      //cout << "perhaps???" << endl;

      event_list.close();
      //cout << "past close" << endl;

      delete T;
      //cout << "past delete" << endl;
    }
  }
  list.close();
  missing.close();
  //run_info.close();
}
