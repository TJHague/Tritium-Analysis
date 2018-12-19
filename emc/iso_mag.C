void iso_mag(){
  TH1D *iso = new TH1D("iso","Isoscalar Correction Magnitude Plot",33,0,.99);
  TH1D *isonorm = new TH1D("isonorm","Isoscalar Correction Magnitude Plot",33,0,.99);
  TH1D *isobodek = new TH1D("isobodek","Isoscalar Correction Magnitude Plot",33,0,.99);

  for(int i = 1; i <= 33; i++){
    iso->SetBinContent(i,He3iso(iso->GetBinCenter(i),0));
    isonorm->SetBinContent(i,He3iso(iso->GetBinCenter(i),1));
    isobodek->SetBinContent(i,He3iso(iso->GetBinCenter(i),2));
  }

  iso->SetMarkerStyle(24);
  isonorm->SetMarkerStyle(25);
  isobodek->SetMarkerStyle(26);

  iso->SetMarkerColor(1);
  isonorm->SetMarkerColor(2);
  isobodek->SetMarkerColor(3);

  auto legend = new TLegend(0.52,0.7,0.9,0.9);
  legend->AddEntry(iso,"No Normalization","p");
  legend->AddEntry(isonorm,"2.8% Normalization","p");
  legend->AddEntry(isobodek,"Bodek Normalization","p");

  iso->SetAxisRange(0.8,1.1,"Y");
  gStyle->SetErrorX(0);
  gStyle->SetOptStat(0);
  iso->Draw("P");
  isonorm->Draw("PSAME");
  isobodek->Draw("PSAME");
  legend->Draw();
}
