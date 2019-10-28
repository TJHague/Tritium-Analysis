void coulomb_cor(TString folder, TString inhist=""){
  const char* ff = "pol5";
  double n = 6;
  //Load in the natural log of structure functions from KP
  //Plot and fit with a 7th order polynomial
  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

  double KPx[100];
  double He3[100];
  double D2[100];

  double Ep = 3.1;
  double E = 10.6;

  for(int i=0;i<100;i++){
    double f2p,f2n,f2d,f2he3,q2;
    in >> KPx[i];
    in >> q2 >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

    double theta = 2. * TMath::ASin(TMath::Sqrt(q2/(4*E*Ep)));
    double sigma = TMath::Power(Ep*TMath::Cos(theta/2.)/q2,2.) * ((1./(E-Ep)) + ((1. + (q2/TMath::Power(E-Ep,2.))) * TMath::Power(TMath::Tan(theta/2.),2.) / (KPx[i]*PROTON_MASS*(1.+R_Whitlow(KPx[i],q2)))));

    He3[i] = TMath::Log(f2he3 * sigma);
    D2[i]  = TMath::Log(f2d * sigma);
  }
  in.close();

  TF1 *KPHe3 = new TF1("KPHe3",ff,0.18,.9);
  TGraph *He3hist = new TGraph(100,KPx,He3);//new TH1D("He3hist","KP hist He3",100,KPx,He3);
  TFitResultPtr He3fitres = He3hist->Fit("KPHe3","S");
  TMatrixDSym He3cov = He3fitres->GetCovarianceMatrix();
//  He3cov.Print();
  He3hist->SetMarkerStyle(8);
//  He3hist->Draw("AP");
//  KPHe3->Draw("SAME");

  TF1 *KPD2 = new TF1("KPD2",ff,0.18,.9);
  TGraph *D2hist = new TGraph(100,KPx,D2);//new TH1D("D2hist","KP hist D2",100,KPx,D2);
  TFitResultPtr D2fitres = D2hist->Fit("KPD2","S");
  TMatrixDSym D2cov = D2fitres->GetCovarianceMatrix();

  //Get the emc ratio from the ROOT file
  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  TH1D *emc = (TH1D*) f->Get(Form("emc%s",inhist.Data()));
  emc->SetDirectory(0);

  //Store correction value and corrected ratio
  TH1D *He3coul = new TH1D("He3coul_val","Helium-3 Coulomb Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  He3coul->Sumw2();
  TH1D *D2coul = new TH1D("D2coul_val","Dueterium Coulomb Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  D2coul->Sumw2();
  TH1D *coul = new TH1D("coul_val","Coulomb Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  coul->Sumw2();
  TH1D *emc_cc = (TH1D*) emc->Clone(Form("emc%s_cc",inhist.Data()));
  //TH1D *emc_cc = new TH1D("emc_cc","He3 Coulomb Corrected EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  emc_cc->Sumw2();

  //Calculate the Coulomb conversion from Q2 -> Q2_eff
  double He3_rmsR = 1.88;
  double D2_rmsR  = 2.127;

  double He3C2 = TMath::Power( 1. + (0.00432*2. / (2.*10.586*TMath::Sqrt(5.*He3_rmsR/3.))) ,2.);
  double D2C2  = TMath::Power( 1. + (0.00432*1. / (2.*10.586*TMath::Sqrt(5.*D2_rmsR/3.)))  ,2.);

  cout << "Bin Center | He3 CC | D2 CC | Ratio CC" << endl;
  for(int i=1; i<=emc->GetNbinsX(); i++){
    double c = emc->GetBinContent(i);
    if(c!=0){
      double bc = emc->GetBinCenter(i);

      TMatrixD h(1,n);
      TMatrixD v(n,1);

      //Calculate correction to He3 yield
      double val_at_x = TMath::Exp(KPHe3->Eval(bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(bc,j);
        v[j][0] = TMath::Power(bc,j);
      }
      double vx_err = val_at_x * TMath::Sqrt((h*He3cov*v)[0][0]);
      cout << val_at_x << " | " <<  vx_err << endl;

      double val_at_x_eff = TMath::Exp(KPHe3->Eval(He3C2 * bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(He3C2*bc,j);
        v[j][0] = TMath::Power(He3C2*bc,j);
      }
      double vxe_err = val_at_x_eff * TMath::Sqrt((h*He3cov*v)[0][0]);
      cout << val_at_x_eff << " | " << vxe_err << endl;
//      (h*He3cov*v).Print();
      double He3_CC = val_at_x / val_at_x_eff;
      double He3_CC_err = He3_CC * TMath::Sqrt((vx_err*vx_err/(val_at_x*val_at_x)) + (vxe_err*vxe_err/(val_at_x_eff*val_at_x_eff)));
      cout << He3_CC_err << endl;

      He3coul->SetBinContent(i,He3_CC);
      He3coul->SetBinError(i,He3_CC_err);
      
      val_at_x = TMath::Exp(KPD2->Eval(bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(bc,j);
        v[j][0] = TMath::Power(bc,j);
      }
      vx_err = val_at_x * TMath::Sqrt((h*He3cov*v)[0][0]);

      val_at_x_eff = TMath::Exp(KPD2->Eval(D2C2 * bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(D2C2*bc,j);
        v[j][0] = TMath::Power(D2C2*bc,j);
      }
      vxe_err = val_at_x_eff * TMath::Sqrt((h*He3cov*v)[0][0]);
      double D2_CC = val_at_x / val_at_x_eff;
      double D2_CC_err = D2_CC * TMath::Sqrt((vx_err*vx_err/(val_at_x*val_at_x)) + (vxe_err*vxe_err/(val_at_x_eff*val_at_x_eff)));
      cout << D2_CC_err << endl;
      
      D2coul->SetBinContent(i,D2_CC);
      D2coul->SetBinError(i,D2_CC_err);

      double CC = He3_CC / D2_CC;
      double CC_err = CC * TMath::Sqrt(TMath::Power(He3_CC_err/He3_CC,2) + TMath::Power(D2_CC_err/D2_CC,2));
      cout << CC_err << endl;
      c *= CC;

      cout << bc << " | " << He3_CC << " | " << D2_CC << " | " << CC << endl;
      coul->SetBinContent(i,CC);
      emc_cc->SetBinContent(i,c);
      emc_cc->SetBinError(i,TMath::Sqrt(TMath::Power(emc->GetBinError(i)*CC,2.) + TMath::Power(c*CC_err,2.)));
    }
  }
  coul->Write();
  emc_cc->Write();
  f->Close();
}
