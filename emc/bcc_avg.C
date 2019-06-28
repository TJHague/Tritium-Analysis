void bcc_calc(TString folder){
  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

  double KPx[100];
  double KPemc[100];

  for(int i=0;i<100;i++){
    double f2p,f2n,f2d,f2he3;
    in >> KPx[i];
    in >> f2p >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

    KPemc[i] = f2he3/f2d;
  }
  in.close();

  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  TH1D *xavg = (TH1D*) f->Get("emcxavg");
  xavg->SetDirectory(0);
  TH1D *emc = (TH1D*) f->Get("emc");
  emc->SetDirectory(0);
  TH1D *bcc = new TH1D("bcc_val","Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  TH1D *emcbc = new TH1D("emcbc","He3 Bin Centered EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());

  int iter_start = 0;
  int iter_end = 0;
  for(int i=1; i<=emc->GetNbinsX(); i++){
    double c = emc->GetBinContent(i);
    if(c!=0){
      double start_x = xavg->GetBinContent(i);
      double end_x = xavg->GetBinCenter(i);
      while(KPx[iter_start+1]<start_x){
        iter_start++;
      }
      if(end_x>start_x){
        iter_end=iter_start;
      }
      while(KPx[iter_end+1]<end_x){
        iter_end++;
      }

      double startKP = (((KPemc[iter_start+1]-KPemc[iter_start])/(KPx[iter_start+1]-KPx[iter_start]))*(start_x-KPx[iter_start])) + KPemc[iter_start];
      double endKP = (((KPemc[iter_end+1]-KPemc[iter_end])/(KPx[iter_end+1]-KPx[iter_end]))*(end_x-KPx[iter_end])) + KPemc[iter_end];

      double bcc_val = endKP/startKP;
      c *= bcc_val;

      bcc->SetBinContent(i,bcc_val);
      emcbc->SetBinContent(i,c);
      emcbc->SetBinError(i,emc->GetBinError(i)*bcc_val);
    }
  }
  bcc->Write();
  emcbc->Write();
  f->Close();
}
