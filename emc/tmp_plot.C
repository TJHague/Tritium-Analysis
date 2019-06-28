void tmp_plot(TString folder){
  TFile *f = new TFile(Form("%s/He3.root",folder.Data()));
  gStyle->SetErrorX(0);

  TH1D *i = (TH1D*) f->Get("emciso");
  TH1D *in = (TH1D*) f->Get("emcisonorm");
  //TH1D *ib = (TH1D*) f->Get("emcisobodek");
  //TH1D *iw = (TH1D*) f->Get("emcisowhitlow");
  TH1D *kp = (TH1D*) f->Get("emcisoKP");
  TH1D *test = (TH1D*) in->Clone("test");
  test->SetDirectory(0);
  test->Scale(1.027);

  i->SetMarkerStyle(24);
  i->SetMarkerColor(1);
  in->SetMarkerStyle(25);
  in->SetMarkerColor(2);
  /*ib->SetMarkerStyle(27);
  ib->SetMarkerColor(3);
  iw->SetMarkerStyle(24);
  iw->SetMarkerColor(1);*/
  kp->SetMarkerStyle(28);
  kp->SetMarkerColor(4);
  test->SetMarkerStyle(27);
  test->SetMarkerColor(1);

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  //legend->AddEntry(i,"No Normalization","p");
  //legend->AddEntry(in,"D/P Normalization","p");
  legend->AddEntry(test,"D/P Normalization and 2.7% He3 Norm","p");
  //legend->AddEntry(ib,"Bodek Normalization","p");
  //legend->AddEntry(iw,"Whitlow Normalization","p");
  legend->AddEntry(kp,"Kulagin and Petti Prediction","l");

  test->SetAxisRange(0.9,1.1,"Y");
  //i->Draw();
  //in->Draw("SAME");
  test->Draw();
  //ib->Draw("SAME");
  //iw->Draw("SAME");
  kp->Draw("LSAME");
  legend->Draw();
}
