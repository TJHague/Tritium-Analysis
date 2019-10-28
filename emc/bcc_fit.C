void bcc_fit(TString folder, TString inhist=""){
  const char* ff = "pol5";
  double n = 6;

  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

  double KPx[100];
  double He3[100];
  double D2[100];
  double test[100];

  double Ep = 3.1;
  double E = 10.6;

  for(int i=0;i<100;i++){
    double f2p,f2n,f2d,f2he3,q2;
    in >> KPx[i];
    in >> q2 >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

    double theta = 2. * TMath::ASin(TMath::Sqrt(q2/(4*E*Ep)));
    double sigma = TMath::Power(Ep*TMath::Cos(theta/2.)/q2,2.) * ((1./(E-Ep)) + ((1. + (q2/TMath::Power(E-Ep,2.))) * TMath::Power(TMath::Tan(theta/2.),2.) / (KPx[i]*PROTON_MASS*(1.+R_Whitlow(KPx[i],q2)))));

    test[i] = f2he3*sigma;
    He3[i] = TMath::Log(f2he3 * sigma);
    D2[i]  = TMath::Log(f2d * sigma);
  }
  in.close();

  TGraph *testhist = new TGraph(100,KPx,test);
  testhist->SetMarkerStyle(8);
  //testhist->Draw("AP");

  TF1 *KPHe3 = new TF1("KPHe3",ff,0.18,.9);
  TGraph *He3hist = new TGraph(100,KPx,He3);//new TH1D("He3hist","KP hist He3",100,KPx,He3);
  TFitResultPtr He3fitres = He3hist->Fit("KPHe3","S");
  TMatrixDSym He3cov = He3fitres->GetCovarianceMatrix();
//  He3cov.Print();
  He3hist->SetMarkerStyle(8);
//  He3hist->Draw("AP");
//  KPHe3->Draw("SAME");


  TFile *f = new TFile(Form("%s/He3.root",folder.Data()),"UPDATE");
  /*TH1D *xavg = (TH1D*) f->Get("emcxavg");
  xavg->SetDirectory(0);*/
  TH1D *emc = (TH1D*) f->Get(Form("emc%s",inhist.Data()));
  emc->SetDirectory(0);
  TH1D *bccHe3 = new TH1D("He3bcc_val","Helium-3 Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  bccHe3->Sumw2();
  TH1D *bccD2  = new TH1D("D2bcc_val","Deuterium Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  bccD2->Sumw2();
  TH1D *bcc = new TH1D("bcc_val","Bin Centering Correction Values",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  bcc->Sumw2();
  TH1D *emcbc = (TH1D*) emc->Clone(Form("emc%s_bc",inhist.Data()));
  //TH1D *emcbc = new TH1D("emcbc","He3 Bin Centered EMC Ratio",emc->GetNbinsX(),emc->GetXaxis()->GetXmin(),emc->GetXaxis()->GetXmax());
  emcbc->Sumw2();

  for(int i=1; i<=emc->GetNbinsX(); i++){
    double c = emc->GetBinContent(i);
    if(c!=0){
      double bc = emc->GetBinCenter(i);
      TMatrixD h(1,n);
      TMatrixD v(n,1);

      //Calculate He3 bcc
      double He3cen = TMath::Exp(KPHe3->Eval(bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(bc,j);
        v[j][0] = TMath::Power(bc,j);
      }
      double He3cen_err = He3cen * TMath::Sqrt((h*He3cov*v)[0][0]);

      double He3exp = TMath::Exp(KPHe3->Integral(bc-0.015,bc+0.015)/0.03);
      double He3exp_err = He3exp * KPHe3->IntegralError(bc-0.015,bc+0.015)/0.03;

      double He3bcc = He3cen/He3exp;
      double He3bcc_err = He3bcc * TMath::Sqrt(TMath::Power(He3cen_err/He3cen, 2.) + TMath::Power(He3exp_err/He3exp, 2.));
      bccHe3->SetBinContent(i,He3bcc);
      bccHe3->SetBinError(i,He3bcc_err);
    }
  }

  TF1 *KPD2 = new TF1("KPD2",ff,0.18,.9);
  TGraph *D2hist = new TGraph(100,KPx,D2);//new TH1D("D2hist","KP hist D2",100,KPx,D2);
  TFitResultPtr D2fitres = D2hist->Fit("KPD2","S");
  TMatrixDSym D2cov = D2fitres->GetCovarianceMatrix();

  for(int i=1; i<=emc->GetNbinsX(); i++){
    double c = emc->GetBinContent(i);
    if(c!=0){
      double bc = emc->GetBinCenter(i);
      TMatrixD h(1,n);
      TMatrixD v(n,1);

      //Calculate D2 bcc
      double D2cen = TMath::Exp(KPD2->Eval(bc));
      for(int j=0; j<=n-1; j++){
        h[0][j] = TMath::Power(bc,j);
        v[j][0] = TMath::Power(bc,j);
      }
      double D2cen_err = D2cen * TMath::Sqrt((h*D2cov*v)[0][0]);

      double D2exp = TMath::Exp(KPD2->Integral(bc-0.015,bc+0.015)/0.03);
      double D2exp_err = D2exp * KPD2->IntegralError(bc-0.015,bc+0.015)/0.03;

      double D2bcc = D2cen/D2exp;
      double D2bcc_err = D2bcc * TMath::Sqrt(TMath::Power(D2cen_err/D2cen, 2.) + TMath::Power(D2exp_err/D2exp, 2.));
      bccD2->SetBinContent(i,D2bcc);
      bccD2->SetBinError(i,D2bcc_err);
      
      double He3bcc = bccHe3->GetBinContent(i);
      double He3bcc_err = bccHe3->GetBinError(i);

      double bcc_val = He3bcc/D2bcc;
      double bcc_err = bcc_val * TMath::Sqrt(TMath::Power(He3bcc_err/He3bcc, 2.) + TMath::Power(D2bcc_err/D2bcc, 2.));

      bcc->SetBinContent(i,bcc_val);
      bcc->SetBinError(i,bcc_err);

      emcbc->SetBinContent(i,c*bcc_val);
      emcbc->SetBinError(i,TMath::Sqrt(TMath::Power(emc->GetBinError(i)*bcc_val,2.) + TMath::Power(c*bcc_err,2.)));

      cout << bc << " | " << He3bcc << " +- " << He3bcc_err << " | " << D2bcc << " +- " << D2bcc_err << " | " << bcc_val << " +- " << bcc_err << endl;
    }
  }
  bccHe3->Write();
  bccD2->Write();
  bcc->Write();
  emcbc->Write();
  f->Close();
}
