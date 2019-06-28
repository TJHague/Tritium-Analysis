void bcc_ind(TString folder){
  double E = 10.59;
  double R = 0.18;
  double Mp = 0.938272;
  double alpha = (1.6e-19*1.6e-19)/(4.*TMath::Pi());;

  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

  double KPx[100];
  double KPHe3[100];
  double KPD2[100];

  for(int i=0;i<100;i++){
    double x, Q2, F2p, F2n, F2D, F2H3, F2He3;
    in >> x >> Q2 >> F2p >> F2n >> F2D >> F2H3 >> F2He3;
    double Ep = E-(Q2/(2.*Mp*x));
    double theta = 2.*TMath::ASin(TMath::Sqrt(Q2/(4.*E*Ep)));

    double sigD = TMath::Power(TMath::Tan(theta/2.),2.);
    sigD *= (1. + Q2/((E-Ep)*(E-Ep)))/(x*Mp*(1.+R));
    sigD += (1./(E-Ep));
    sigD *= 4.*alpha*alpha*Ep*Ep/(Q2*Q2);
    sigD *= TMath::Power(TMath::Cos(theta/2.),2.);

    double sigHe3 = sigD*F2He3;
    double sigH3 = sigD*F2H3;
    sigD *= F2D;

    KPx[i]=x;
    KPHe3[i]=sigHe3;
    KPD2[i]=sigD;
    
    /*double Q2,f2p,f2n,f2d,f2he3;
    double Ep = E-(Q2/(2.*Mp*KPx[i]));
    double theta = 2.*TMath::ASin(TMath::Sqrt(Q2/(4.*E*Ep)));

    in >> KPx[i];
    in >> Q2 >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

    double sig = TMath::Power(TMath::Tan(theta/2.),2.);
    sig *= (1. + Q2/((E-Ep)*(E-Ep)))/(KPx[i]*Mp*(1.+R));
    sig += (1./(E-Ep));
    sig *= 4.*Ep*Ep/(Q2*Q2);
    sig *= TMath::Power(TMath::Cos(theta/2.),2.);

    KPHe3[i] = sig*f2he3;
    KPD2[i]  = sig*f2d;
    cout << KPHe3[i] << " " << KPD2[i] << endl;*/
  }
  in.close();

  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  /*TH1D *xavg = (TH1D*) f->Get("emcxavg");
  xavg->SetDirectory(0);*/
  TH1D *emc = (TH1D*) f->Get("emc");
  emc->SetDirectory(0);
  TH1D *He3bcc = new TH1D("He3bcc_val","He3 Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  TH1D *D2bcc = new TH1D("D2bcc_val","D2 Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  TH1D *emcbcc = new TH1D("emcbcc_val","EMC Ratio Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());

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

      double He3lowKP = (((KPHe3[iter_low+1]-KPHe3[iter_low])/(KPx[iter_low+1]-KPx[iter_low]))*(low_x-KPx[iter_low])) + KPHe3[iter_low];
      double He3endKP = (((KPHe3[iter_end+1]-KPHe3[iter_end])/(KPx[iter_end+1]-KPx[iter_end]))*(end_x-KPx[iter_end])) + KPHe3[iter_end];
      double He3hiKP = (((KPHe3[iter_hi+1]-KPHe3[iter_hi])/(KPx[iter_hi+1]-KPx[iter_hi]))*(hi_x-KPx[iter_hi])) + KPHe3[iter_hi];
      //cout << He3lowKP << " " << He3endKP << " " << He3hiKP << endl;

      double D2lowKP = (((KPD2[iter_low+1]-KPD2[iter_low])/(KPx[iter_low+1]-KPx[iter_low]))*(low_x-KPx[iter_low])) + KPD2[iter_low];
      double D2endKP = (((KPD2[iter_end+1]-KPD2[iter_end])/(KPx[iter_end+1]-KPx[iter_end]))*(end_x-KPx[iter_end])) + KPD2[iter_end];
      double D2hiKP = (((KPD2[iter_hi+1]-KPD2[iter_hi])/(KPx[iter_hi+1]-KPx[iter_hi]))*(hi_x-KPx[iter_hi])) + KPD2[iter_hi];
      //cout << D2lowKP << " " << D2endKP << " " << D2hiKP << endl;

      iter_low++;
      double He3expectation_val = 0;
      double D2expectation_val = 0;
      He3expectation_val += (He3lowKP*(KPx[iter_low]-low_x)) + (0.5*(KPHe3[iter_low]-He3lowKP)*(KPx[iter_low]-low_x));
      D2expectation_val += (D2lowKP*(KPx[iter_low]-low_x)) + (0.5*(KPD2[iter_low]-D2lowKP)*(KPx[iter_low]-low_x));
      iter_low++;
      while(iter_low<=iter_hi){
        He3expectation_val += (KPHe3[iter_low-1]*(KPx[iter_low]-KPx[iter_low-1])) + (0.5*(KPHe3[iter_low]-KPHe3[iter_low-1])*(KPx[iter_low]-KPx[iter_low-1]));
        D2expectation_val += (KPD2[iter_low-1]*(KPx[iter_low]-KPx[iter_low-1])) + (0.5*(KPD2[iter_low]-KPD2[iter_low-1])*(KPx[iter_low]-KPx[iter_low-1]));
        iter_low++;
      }
      He3expectation_val += (KPHe3[iter_hi]*(hi_x-KPx[iter_hi])) + (0.5*(He3hiKP-KPHe3[iter_hi])*(hi_x-KPx[iter_hi]));
      D2expectation_val += (KPD2[iter_hi]*(hi_x-KPx[iter_hi])) + (0.5*(D2hiKP-KPD2[iter_hi])*(hi_x-KPx[iter_hi]));
      He3expectation_val /= 0.03;
      D2expectation_val /= 0.03;

      double He3bcc_val = He3endKP/He3expectation_val;
      double D2bcc_val = D2endKP/D2expectation_val;

      He3bcc->SetBinContent(i,He3bcc_val);
      D2bcc->SetBinContent(i,D2bcc_val);
      emcbcc->SetBinContent(i,He3bcc_val/D2bcc_val);
      cout << i << " " << end_x << " " << He3expectation_val << " " << He3bcc_val << " " << D2expectation_val << " " << D2bcc_val << " " << He3bcc_val/D2bcc_val << endl;
    }
  }
  He3bcc->Write();
  D2bcc->Write();
  emcbcc->Write();
  f->Close();
}
