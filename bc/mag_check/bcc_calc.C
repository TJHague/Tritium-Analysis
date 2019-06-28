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
  /*TH1D *xavg = (TH1D*) f->Get("emcxavg");
  xavg->SetDirectory(0);*/
  TH1D *emc = (TH1D*) f->Get("emc");
  emc->SetDirectory(0);
  TH1D *bcc = new TH1D("bcc_val","Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  TH1D *emcbc = new TH1D("emcbc","He3 Bin Centered EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());

  int iter_low = 0;
  int iter_hi = 0;
  int iter_end = 0;
  for(int i=1; i<=emc->GetNbinsX(); i++){
    double c = emc->GetBinContent(i);
    if(c!=0){
      double low_x = emc->GetBinCenter(i)-0.015;
      double hi_x = emc->GetBinCenter(i)+0.015;
      double end_x = emc->GetBinCenter(i);
      while(KPx[iter_low+1]<low_x){
        iter_low++;
      }
      iter_end = iter_low;
      while(KPx[iter_end+1]<end_x){
        iter_end++;
      }
      iter_hi = iter_end;
      while(KPx[iter_hi+1]<hi_x){
        iter_hi++;
      }

      double lowKP = (((KPemc[iter_low+1]-KPemc[iter_low])/(KPx[iter_low+1]-KPx[iter_low]))*(low_x-KPx[iter_low])) + KPemc[iter_low];
      double endKP = (((KPemc[iter_end+1]-KPemc[iter_end])/(KPx[iter_end+1]-KPx[iter_end]))*(end_x-KPx[iter_end])) + KPemc[iter_end];
      double hiKP = (((KPemc[iter_hi+1]-KPemc[iter_hi])/(KPx[iter_hi+1]-KPx[iter_hi]))*(hi_x-KPx[iter_hi])) + KPemc[iter_hi];

      iter_low++;
      double expectation_val = 0;
      expectation_val += (lowKP*(KPx[iter_low]-low_x)) + (0.5*(KPemc[iter_low]-lowKP)*(KPx[iter_low]-low_x));
      iter_low++;
      while(iter_low<=iter_hi){
        expectation_val += (KPemc[iter_low-1]*(KPx[iter_low]-KPx[iter_low-1])) + (0.5*(KPemc[iter_low]-KPemc[iter_low-1])*(KPx[iter_low]-KPx[iter_low-1]));
        iter_low++;
      }
      expectation_val += (KPemc[iter_hi]*(hi_x-KPx[iter_hi])) + (0.5*(hiKP-KPemc[iter_hi])*(hi_x-KPx[iter_hi]));
      expectation_val /= 0.03;

      double bcc_val = endKP/expectation_val;
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
