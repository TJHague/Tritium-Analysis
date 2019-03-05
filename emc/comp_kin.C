{
  TCanvas *c[19];
  TH1D *p1[19];
  TH1D *p2[19];
  TLegend *l[19];
  /*TFile *f = new TFile("pass2_EL/D2.root");
    TFile *g = new TFile("pass2_noEL/D2.root");
    gStyle->SetErrorX(0);

    TH1D *i = (TH1D*) f->Get("D2isowhitlow");
    TH1D *j = (TH1D*) g->Get("D2isowhitlow");

    i->SetMarkerStyle(24);
    i->SetMarkerColor(1);
    j->SetMarkerStyle(25);
    j->SetMarkerColor(2);

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(i,"Whitlow Norm - Pass 2 Energy Loss","p");
    legend->AddEntry(j,"Whitlow Norm - Pass 2 no Energy Loss","p");

    i->SetAxisRange(0.9,1.05,"Y");
    i->Draw();
    j->Draw("SAME");
    legend->Draw();*/

  gStyle->SetOptStat(0);
  for(int i=0; i<=5; i++){
    c[i] = new TCanvas();
    TFile *f = new TFile(Form("newx03/fullKin/kin%d.root",i));
    TFile *g = new TFile(Form("pass2_noEL/fullKin/kin%d.root",i));

    p1[i] = (TH1D*) f->Get("D2");
    p2[i] = (TH1D*) g->Get("D2");
    p1[i]->SetDirectory(0);
    p2[i]->SetDirectory(0);

    p1[i]->SetMarkerStyle(24);
    p1[i]->SetMarkerColor(1);
    p2[i]->SetMarkerStyle(25);
    p2[i]->SetMarkerColor(2);

    f->Close();
    g->Close();

    l[i] = new TLegend(0.1,0.7,0.48,0.9);
    l[i]->AddEntry(p1[i],Form("Pass 1 Kin %d",i),"p");
    l[i]->AddEntry(p2[i],Form("Pass 2 Kin %d",i),"p");

    c[i]->cd();
    //p1[i]->SetAxisRange(0.,1.3,"Y");
    p1[i]->Draw();
    p2[i]->Draw("SAME");
    l[i]->Draw();
  }
  for(int i=7; i<=15; i+=2){
    c[i-1] = new TCanvas();
    TFile *f = new TFile(Form("newx03/fullKin/kin%d_1st.root",i));
    TFile *g = new TFile(Form("pass2_noEL/fullKin/kin%d_1st.root",i));

    p1[i-1] = (TH1D*) f->Get("D2");
    p2[i-1] = (TH1D*) g->Get("D2");
    p1[i-1]->SetDirectory(0);
    p2[i-1]->SetDirectory(0);

    p1[i-1]->SetMarkerStyle(24);
    p1[i-1]->SetMarkerColor(1);
    p2[i-1]->SetMarkerStyle(25);
    p2[i-1]->SetMarkerColor(2);

    f->Close();
    g->Close();

    l[i-1] = new TLegend(0.1,0.7,0.48,0.9);
    l[i-1]->AddEntry(p1[i-1],Form("Pass 1 Kin %d 1st",i),"p");
    l[i-1]->AddEntry(p2[i-1],Form("Pass 2 Kin %d 1st",i),"p");

    c[i-1]->cd();
    //p1[i-1]->SetAxisRange(0.,1.3,"Y");
    p1[i-1]->Draw();
    p2[i-1]->Draw("SAME");
    l[i-1]->Draw();


    c[i] = new TCanvas();
    f = new TFile(Form("newx03/fullKin/kin%d_2nd.root",i));
    g = new TFile(Form("pass2_noEL/fullKin/kin%d_2nd.root",i));

    p1[i] = (TH1D*) f->Get("D2");
    p2[i] = (TH1D*) g->Get("D2");
    p1[i]->SetDirectory(0);
    p2[i]->SetDirectory(0);

    p1[i]->SetMarkerStyle(24);
    p1[i]->SetMarkerColor(1);
    p2[i]->SetMarkerStyle(25);
    p2[i]->SetMarkerColor(2);

    f->Close();
    g->Close();

    l[i] = new TLegend(0.1,0.7,0.48,0.9);
    l[i]->AddEntry(p1[i],Form("Pass 1 Kin %d 2nd",i),"p");
    l[i]->AddEntry(p2[i],Form("Pass 2 Kin %d 2nd",i),"p");

    c[i]->cd();
    //p1[i]->SetAxisRange(0.,1.3,"Y");
    p1[i]->Draw();
    p2[i]->Draw("SAME");
    l[i]->Draw();
  }

  c[16] = new TCanvas();
  TFile *f = new TFile("newx03/fullKin/kin15_3rd.root");
  TFile *g = new TFile("pass2_noEL/fullKin/kin15_3rd.root");

  p1[16] = (TH1D*) f->Get("D2");
  p2[16] = (TH1D*) g->Get("D2");
  p1[16]->SetDirectory(0);
  p2[16]->SetDirectory(0);

  p1[16]->SetMarkerStyle(24);
  p1[16]->SetMarkerColor(1);
  p2[16]->SetMarkerStyle(25);
  p2[16]->SetMarkerColor(2);

  f->Close();
  g->Close();

  l[16] = new TLegend(0.1,0.7,0.48,0.9);
  l[16]->AddEntry(p1[16],"Pass 1 Kin 15 3rd","p");
  l[16]->AddEntry(p2[16],"Pass 2 Kin 15 3rd","p");

  c[16]->cd();
  //p1[16]->SetAxisRange(0.,1.3,"Y");
  p1[16]->Draw();
  p2[16]->Draw("SAME");
  l[16]->Draw();

  c[17] = new TCanvas();
  f = new TFile("newx03/fullKin/kin16_1st.root");
  g = new TFile("pass2_noEL/fullKin/kin16_1st.root");

  p1[17] = (TH1D*) f->Get("D2");
  p2[17] = (TH1D*) g->Get("D2");
  p1[17]->SetDirectory(0);
  p2[17]->SetDirectory(0);

  p1[17]->SetMarkerStyle(24);
  p1[17]->SetMarkerColor(1);
  p2[17]->SetMarkerStyle(25);
  p2[17]->SetMarkerColor(2);

  f->Close();
  g->Close();

  l[17] = new TLegend(0.1,0.7,0.48,0.9);
  l[17]->AddEntry(p1[17],"Pass 1 Kin 16 1st","p");
  l[17]->AddEntry(p2[17],"Pass 2 Kin 16 1st","p");

  c[17]->cd();
  //p1[17]->SetAxisRange(0.,1.3,"Y");
  p1[17]->Draw();
  p2[17]->Draw("SAME");
  l[17]->Draw();

  c[18] = new TCanvas();
  f = new TFile("newx03/fullKin/kin16_2nd.root");
  g = new TFile("pass2_noEL/fullKin/kin16_2nd.root");

  p1[18] = (TH1D*) f->Get("D2");
  p2[18] = (TH1D*) g->Get("D2");
  p1[18]->SetDirectory(0);
  p2[18]->SetDirectory(0);

  p1[18]->SetMarkerStyle(24);
  p1[18]->SetMarkerColor(1);
  p2[18]->SetMarkerStyle(25);
  p2[18]->SetMarkerColor(2);

  f->Close();
  g->Close();

  l[18] = new TLegend(0.1,0.7,0.48,0.9);
  l[18]->AddEntry(p1[18],"Pass 1 Kin 16 2nd","p");
  l[18]->AddEntry(p2[18],"Pass 2 Kin 16 2nd","p");

  c[18]->cd();
  //p1[18]->SetAxisRange(0.,1.3,"Y");
  p1[18]->Draw();
  p2[18]->Draw("SAME");
  l[18]->Draw();

}
