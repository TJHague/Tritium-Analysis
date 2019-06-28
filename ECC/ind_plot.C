#include <stdio.h>
#include <stdlib.h>
void ind_plot(TString target1, TString target2, TString folder){
  int x = (target1=="H1" || target2=="H1") ? 4 : 16;
  int y = (target1=="H1" || target2=="H1") ? 4 : 18;
  gStyle->SetErrorX(0);

  //Get the histograms
  TFile *f1 = new TFile(folder + "/" + target1 + ".root");
  TH1D* raw1[19];
  TH1D* raw2[19];
  TH1D* plots1[19];
  TH1D* plots2[19];
  TH1D* ratio[19];
  int j = 0;
  for(int i=0; i<=x; i++){
    if(i<=5){
      cout << i << " " << j << endl;
      raw1[j] = (TH1D*) f1->Get(Form("ecc_kin%d",i))->Clone(Form("%s_kin%d",target1.Data(),i));
      raw1[j]->SetDirectory(0);
      j += 1;
    }else if(i%2==1 || i==16){
      cout << i << " " << j << endl;
      raw1[j] = (TH1D*) f1->Get(Form("ecc_kin%d_1st",i))->Clone(Form("%s_kin%d_1st",target1.Data(),i));
      raw1[j]->SetDirectory(0);
      j += 1;
      raw1[j] = (TH1D*) f1->Get(Form("ecc_kin%d_2nd",i))->Clone(Form("%s_kin%d_2nd",target1.Data(),i));
      raw1[j]->SetDirectory(0);
      j += 1;
    }
    if(i==15){
      raw1[j] = (TH1D*) f1->Get(Form("ecc_kin%d_3rd",i))->Clone(Form("%s_kin%d_3rd",target1.Data(),i));
      raw1[j]->SetDirectory(0);
      j += 1;
    }
  }
  f1->Close();
  TFile *f2 = new TFile(folder + "/" + target2 + ".root");
  j=0;
  for(int i=0; i<=x; i++){
    if(i<=5){
      cout << i << " " << j << endl;
      raw2[j] = (TH1D*) f2->Get(Form("ecc_kin%d",i))->Clone(Form("%s_kin%d",target2.Data(),i));
      raw2[j]->SetDirectory(0);
      j += 1;
    }else if(i%2==1 || i==16){
      cout << i << " " << j << endl;
      raw2[j] = (TH1D*) f2->Get(Form("ecc_kin%d_1st",i))->Clone(Form("%s_kin%d_1st",target2.Data(),i));
      raw2[j]->SetDirectory(0);
      j += 1;
      raw2[j] = (TH1D*) f2->Get(Form("ecc_kin%d_2nd",i))->Clone(Form("%s_kin%d_2nd",target2.Data(),i));
      raw2[j]->SetDirectory(0);
      j += 1;
    }
    if(i==15){
      raw2[j] = (TH1D*) f2->Get(Form("ecc_kin%d_3rd",i))->Clone(Form("%s_kin%d_3rd",target2.Data(),i));
      raw2[j]->SetDirectory(0);
      j += 1;
    }

  }
  f2->Close();

  //Remove edge bins and take ratio
  for(int i=0; i<=y; i++){
    plots1[i] = new TH1D(raw1[i]->GetName(),raw1[i]->GetTitle(),33,0,0.99);
    plots2[i] = new TH1D(raw2[i]->GetName(),raw2[i]->GetTitle(),33,0,0.99);
    bool first = true;
    for(int k=0; k<33; k++){
      double c = raw1[i]->GetBinContent(k);
      if(c!=0){
        if(!first){
          double d = raw1[i]->GetBinContent(k+1);
          if(d!=0){
            plots1[i]->SetBinContent(k,c);
            plots1[i]->SetBinError(k,raw1[i]->GetBinError(k));
          }else{
            break;
          }
        }else{
          first = false;
        }
      }
    }
    first = true;
    for(int k=0; k<33; k++){
      double c = raw2[i]->GetBinContent(k);
      if(c!=0){
        if(!first){
          double d = raw2[i]->GetBinContent(k+1);
          if(d!=0){
            plots2[i]->SetBinContent(k,c);
            plots2[i]->SetBinError(k,raw2[i]->GetBinError(k));
          }else{
            break;
          }
        }else{
          first = false;
        }
      }
    }
    TString kin = (i<=5) ? Form("%d",i) : ((i%2==0) ? Form("%d",i+1) + TString("_1st") : Form("%d",i) + TString("_2nd"));
    if(i==16){
      kin = "15_3rd";
    }else if(i==17){
      kin = "16_1st";
    }else if(i==18){
      kin = "16_2nd";
    }
    ratio[i] = new TH1D(Form("ratio_kin%s",kin.Data()),Form("%s/%s Endcap Contamination",target1.Data(),target2.Data()),33,0,0.99);
    for(int k=0; k<33; k++){
      double e1 = plots1[i]->GetBinContent(k);
      double e2 = plots2[i]->GetBinContent(k);
      if(e1!=0 && e2!=0){
        cout << e1 << " " << e2 << " " << ((1.-e1)/(1.-e2)) << endl;
        ratio[i]->SetBinContent(k,((1.-e1)/(1.-e2)));
        double de1 = plots1[i]->GetBinError(k);
        double de2 = plots2[i]->GetBinError(k);
        double error = de1*de1/((1.-e2)*(1.-e2));
        error += de2*de2*(1.-e1)*(1.-e1)/((1.-e2)*(1.-e2)*(1.-e2)*(1.-e2));
        error = TMath::Sqrt(error);
        ratio[i]->SetBinError(k,error);
      }
    }
  }

  TCanvas *c1 = new TCanvas();
  c1->cd(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  ratio[0]->SetMarkerColor(1);
  ratio[0]->SetMarkerStyle(24);
  ratio[0]->SetDirectory(0);
  //ratio[0]->SetTitle(Form("%s/%s Endcap Contamination",target1.Data(),target2.Data()));
  ratio[0]->SetAxisRange(0.97,1.03,"Y");
  ratio[0]->Draw("P");
  legend->AddEntry(ratio[0],"Kinematic 0","p");
  for(int i=1;i<=y;i++){
    ratio[i]->SetMarkerColor(1+(int)(i%4));
    ratio[i]->SetMarkerStyle(24+(int)(i%5));
    ratio[i]->Draw("PSAME");
    TString kin = (i<=5) ? Form("%d",i) : ((i%2==0) ? Form("%d",i+1) + TString("_1st") : Form("%d",i) + TString("_2nd"));
    if(i==16){
      kin = "15_3rd";
    }else if(i==17){
      kin = "16_1st";
    }else if(i==18){
      kin = "16_2nd";
    }
    legend->AddEntry(ratio[i],Form("Kinematic %s",kin.Data()),"p");
  }
  legend->Draw();

  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  auto legend1 = new TLegend(0.1,0.7,0.48,0.9);

  plots1[0]->SetMarkerColor(1);
  plots1[0]->SetMarkerStyle(24);
  plots1[0]->SetDirectory(0);
  plots1[0]->SetTitle(Form("%s Endcap Contamination",target1.Data()));
  plots1[0]->SetAxisRange(0,0.05,"Y");
  plots1[0]->Draw("P");
  legend1->AddEntry(plots1[0],"Kinematic 0","p");
  for(int i=1;i<=y;i++){
    plots1[i]->SetMarkerColor(1+(int)(i%4));
    plots1[i]->SetMarkerStyle(24+(int)(i%5));
    plots1[i]->Draw("PSAME");
    TString kin = (i<=5) ? Form("%d",i) : ((i%2==0) ? Form("%d",i+1) + TString("_1st") : Form("%d",i) + TString("_2nd"));
    if(i==16){
      kin = "15_3rd";
    }else if(i==17){
      kin = "16_1st";
    }else if(i==18){
      kin = "16_2nd";
    }
    legend1->AddEntry(plots1[i],Form("Kinematic %s",kin.Data()),"p");
  }
  legend1->Draw();

  TCanvas *c3 = new TCanvas();
  c3->cd(0);
  auto legend2 = new TLegend(0.1,0.7,0.48,0.9);

  plots2[0]->SetMarkerColor(1);
  plots2[0]->SetMarkerStyle(24);
  plots2[0]->SetDirectory(0);
  plots2[0]->SetTitle(Form("%s Endcap Contamination",target2.Data()));
  plots2[0]->SetAxisRange(0,0.05,"Y");
  plots2[0]->Draw("P");
  legend2->AddEntry(plots2[0],"Kinematic 0","p");
  for(int i=1;i<=y;i++){
    plots2[i]->SetMarkerColor(1+(int)(i%4));
    plots2[i]->SetMarkerStyle(24+(int)(i%5));
    plots2[i]->Draw("PSAME");
    TString kin = (i<=5) ? Form("%d",i) : ((i%2==0) ? Form("%d",i+1) + TString("_1st") : Form("%d",i) + TString("_2nd"));
    if(i==16){
      kin = "15_3rd";
    }else if(i==17){
      kin = "16_1st";
    }else if(i==18){
      kin = "16_2nd";
    }
    legend2->AddEntry(plots2[i],Form("Kinematic %s",kin.Data()),"p");
  }
  legend2->Draw();

}
