void plot_iso(TString folder){
  TFile *f = new TFile(Form("%s/He3.root",folder.Data()));
  gStyle->SetErrorX(0);

  /*TH1D *i = (TH1D*) f->Get("emciso");
  TH1D *in = (TH1D*) f->Get("emcisonorm");
  TH1D *ib = (TH1D*) f->Get("emcisobodek");*/
  TH1D *iw = (TH1D*) f->Get("emciso");
  TH1D *kp = (TH1D*) f->Get("emcisoKP");

  /*i->SetMarkerStyle(24);
  i->SetMarkerColor(1);
  in->SetMarkerStyle(25);
  in->SetMarkerColor(2);
  ib->SetMarkerStyle(27);
  ib->SetMarkerColor(3);*/
  iw->SetMarkerStyle(24);
  iw->SetMarkerColor(1);
  kp->SetMarkerStyle(28);
  kp->SetMarkerColor(4);

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  /*legend->AddEntry(i,"No Normalization","p");
  legend->AddEntry(in,"D/P Normalization","p");
  legend->AddEntry(ib,"Bodek Normalization","p");*/
  legend->AddEntry(iw,"No Normalization","p");
  legend->AddEntry(kp,"Kulagin and Petti Prediction","l");

  iw->SetAxisRange(0.9,1.05,"Y");
  /*i->Draw();
  in->Draw("SAME");
  ib->Draw("SAME");*/
  iw->Draw();
  kp->Draw("LSAME");
  legend->Draw();
}
