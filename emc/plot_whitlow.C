{
  TFile *f = new TFile("newx03/He3.root");
  gStyle->SetErrorX(0);

  TH1D *e = (TH1D*) f->Get("emc");
  //TH1D *ei = (TH1D*) f->Get("emcisonorm");
  TH1D *w = (TH1D*) f->Get("whitlow_emc");

  e->SetMarkerStyle(24);
  e->SetMarkerColor(1);
  //ei->SetMarkerStyle(25);
  //ei->SetMarkerColor(2);
  w->SetMarkerStyle(25);
  w->SetMarkerColor(2);

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry(e,"Marathon He3 EMC Ratio","p");
  //legend->AddEntry(in,"2.8% Normalization","p");
  legend->AddEntry(w,"Whitlow He3 EMC Ratio","p");

  //i->SetAxisRange(0.9,1.05,"Y");
  e->Draw();
  w->Draw("PSAME");
  legend->Draw();
}
