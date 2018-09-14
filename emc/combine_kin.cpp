#include "TSystem.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1.h"
#include <iostream>

void combine_kin(){
  TH1D* emc[17];
  bool plotted = false;
  //emc = (TH1D*) gDirectory->Get("emc");
  //emc->Draw("P");

  auto legend = new TLegend(0.1,0.1,0.48,0.3);
  legend->SetHeader("Helium-3 EMC Ratio","C");
  //legend->AddEntry(emc,"Kin 1","p");
  //legend->Draw();

//  f->Close();

  for(Int_t i = 0; i <= 5; i++){
    if( !gSystem->AccessPathName(Form("fullKin/kin%d.root",i))){
      TFile *f = new TFile(Form("fullKin/kin%d.root",i));
      emc[i] = (TH1D*) gDirectory->Get("emc");
      emc[i]->SetDirectory(0);
      emc[i]->SetMarkerStyle(21); //Box
      emc[i]->SetMarkerColor(i+1);
      emc[i]->SetMarkerSize(1);

      if(plotted){
        emc[i]->Draw("SAME");
      }else{
        emc[i]->GetYaxis()->SetRangeUser(0,1.5);
        emc[i]->SetTitle("Helium-3 EMC Ratio");
        emc[i]->Draw("P");
        plotted = true;
      }

      legend->AddEntry(emc[i],Form("Kin %d",i),"p");

      f->Close();
    }
  }

  for(Int_t i = 7; i < 17; i=i+2){
    if( !gSystem->AccessPathName(Form("fullKin/kin%d_1st.root",i))){
      TFile *f = new TFile(Form("fullKin/kin%d_1st.root",i));
      emc[i] = (TH1D*) gDirectory->Get("emc");
      emc[i]->SetDirectory(0);
      if(i<9){
        emc[i]->SetMarkerStyle(21); //Box
        emc[i]->SetMarkerColor(i+1);
      }else{
        emc[i]->SetMarkerStyle(22); //Triangle
        emc[i]->SetMarkerColor(i-8);
      }
      emc[i]->SetMarkerSize(1);

      if(plotted){
        emc[i]->Draw("SAME");
      }else{
        emc[i]->GetYaxis()->SetRangeUser(0,1.5);
        emc[i]->SetTitle("Helium-3 EMC Ratio");
        emc[i]->Draw("P");
        plotted = true;
      }

      legend->AddEntry(emc[i],Form("Kin %d",i),"p");

      f->Close();
    }
    if( !gSystem->AccessPathName(Form("fullKin/kin%d_2nd.root",i))){
      TFile *f = new TFile(Form("fullKin/kin%d_2nd.root",i));
      emc[i+1] = (TH1D*) gDirectory->Get("emc");
      emc[i+1]->SetDirectory(0);
      if((i+1)<9){
        emc[i+1]->SetMarkerStyle(21); //Box
        emc[i+1]->SetMarkerColor(i+2);
      }else{
        emc[i+1]->SetMarkerStyle(22); //Triangle
        emc[i+1]->SetMarkerColor(i-7);
      }
      emc[i+1]->SetMarkerSize(1);

      if(plotted){
        emc[i+1]->Draw("SAME");
      }else{
        emc[i+1]->GetYaxis()->SetRangeUser(0,1.5);
        emc[i+1]->SetTitle("Helium-3 EMC Ratio");
        emc[i+1]->Draw("P");
        plotted = true;
      }

      legend->AddEntry(emc[i+1],Form("Kin %d",i),"p");

      f->Close();
    }
  }

  if( !gSystem->AccessPathName("fullKin/kin16.root")){
    TFile *f = new TFile("fullKin/kin16.root");
    emc[16] = (TH1D*) gDirectory->Get("emc");
    emc[16]->SetDirectory(0);
    emc[16]->SetMarkerStyle(22); //Box
    emc[16]->SetMarkerColor(9);
    emc[16]->SetMarkerSize(1);

    if(plotted){
      emc[16]->Draw("SAME");
    }else{
      emc[16]->GetYaxis()->SetRangeUser(0,1.5);
      emc[16]->SetTitle("Helium-3 EMC Ratio");
      emc[16]->Draw("P");
      plotted = true;
    }

    legend->AddEntry(emc[16],"Kin 16","p");

    f->Close();
  }
  legend->Draw();
}
