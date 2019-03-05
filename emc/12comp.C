{
  TFile *f = new TFile("pass2_test/fullKin/kin1.root","UPDATE");
  TH1D *a = (TH1D*) gDirectory->Get("emc");
  a->SetDirectory(0);
  f->Close();
  delete f;
  
  f = new TFile("pass2_test/fullKin/kin4.root","UPDATE");
  TH1D *b = (TH1D*) gDirectory->Get("emc");
  b->SetDirectory(0);
  f->Close();
  delete f;
  
  f = new TFile("newx03/fullKin/kin1.root","UPDATE");
  TH1D *c = (TH1D*) gDirectory->Get("emc");
  c->SetDirectory(0);
  f->Close();
  delete f;
  
  f = new TFile("newx03/fullKin/kin4.root","UPDATE");
  TH1D *d = (TH1D*) gDirectory->Get("emc");
  d->SetDirectory(0);
  f->Close();
  delete f;

  f = new TFile("energy_loss/fullKin/kin1.root","UPDATE");
  TH1D *g = (TH1D*) gDirectory->Get("emc");
  g->SetDirectory(0);
  f->Close();
  delete f;

  f = new TFile("energy_loss/fullKin/kin4.root","UPDATE");
  TH1D *h = (TH1D*) gDirectory->Get("emc");
  h->SetDirectory(0);
  f->Close();
  delete f;

  a->SetMarkerStyle(24);
  b->SetMarkerStyle(24);
  a->SetMarkerColor(1);
  b->SetMarkerColor(2);
  c->SetMarkerStyle(25);
  d->SetMarkerStyle(25);
  c->SetMarkerColor(3);
  d->SetMarkerColor(4);
  g->SetMarkerStyle(26);
  h->SetMarkerStyle(26);
  g->SetMarkerColor(6);
  h->SetMarkerColor(7);

  a->SetAxisRange(0.9,1.3,"Y");
  a->SetAxisRange(0.,0.5,"X");
  a->SetTitle("Pass 1 to Pass 2 Comparison");
  gStyle->SetOptStat(0);
  gStyle->SetErrorX(0);

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry(c,"Pass 1 Kin 1","p");
  legend->AddEntry(d,"Pass 1 Kin 4","p");
  legend->AddEntry(a,"Pass 2 Kin 1","p");
  legend->AddEntry(b,"Pass 2 Kin 4","p");
  legend->AddEntry(g,"Pass 2 Kin 1 - Energy Loss","p");
  legend->AddEntry(h,"Pass 2 Kin 4 - Energy Loss","p");

  a->Draw();
  b->Draw("SAME");
  c->Draw("SAME");
  d->Draw("SAME");
  g->Draw("SAME");
  h->Draw("SAME");
  legend->Draw();
}
