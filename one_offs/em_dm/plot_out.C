{
  gStyle->SetOptStat(0);
  gStyle->SetErrorX(0);
  TFile *f = new TFile("em_dm.root");

  auto legend = new TLegend(0.1,0.1,0.48,0.3);
  auto legend2 = new TLegend(0.1,0.1,0.48,0.3);
  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  int j=0;
  for(int i=0; i<=16; i++){
    if(i<=5 || i%2==1 || i==16){
      TH1D *tmp = (TH1D*) f->Get(Form("em_up_kin%d",i));
      TH1D *tmp2 = (TH1D*) f->Get(Form("em_down_kin%d",i));
      tmp->SetMarkerStyle(j%4+20);
      tmp->SetMarkerColor(j%4+1);
      tmp2->SetMarkerStyle(j%4+20);
      tmp2->SetMarkerColor(j%4+1);
      j++;
      c1->cd(0);
      if(i==0){
        tmp->Draw();
      }else{
        tmp->Draw("SAME");
      }
      c2->cd(0);
      if(i==0){
        tmp2->Draw();
      }else{
        tmp2->Draw("SAME");
      }
      legend->AddEntry(tmp,Form("Kinematic %d",i),"p");
      legend2->AddEntry(tmp2,Form("Kinematic %d",i),"p");
    }
  }
  c1->cd(0);
  legend->Draw();
  c2->cd(0);
  legend2->Draw();
}
