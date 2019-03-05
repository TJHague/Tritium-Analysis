{
  TFile *f = new TFile("p2_test/He3.root");
  TFile *g = new TFile("p2newz/He3.root");
  gStyle->SetErrorX(0);

  TH1D *i = (TH1D*) f->Get("emciso");
  TH1D *j = (TH1D*) g->Get("emciso");

  i->SetMarkerStyle(24);
  i->SetMarkerColor(1);
  j->SetMarkerStyle(25);
  j->SetMarkerColor(2);

  auto legend = new TLegend(0.1,0.1,0.48,0.3);
  legend->AddEntry(i,"Pass 2 No Energy Loss","p");
  legend->AddEntry(j,"Pass 2 Energy Loss","p");

  i->SetAxisRange(0.9,1.05,"Y");
  i->Draw();
  j->Draw("SAME");
  legend->Draw();
}
