#include "TSystem.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1.h"
#include <iostream>

void combine_kin(){
  TH1D* emc[12];
  bool plotted = false;

  auto legend = new TLegend(0.1,0.1,0.48,0.3);
  legend->SetHeader("Helium-3 EMC Ratio","C");

  int j = 0; //array location
  for(Int_t i = 0; i <= 5; i++){
    if( !gSystem->AccessPathName(Form("cleanedKin/kin%d.root",i))){
      TFile *f = new TFile(Form("cleanedKin/kin%d.root",i));
      emc[j] = (TH1D*) gDirectory->Get("emc");
      emc[j]->SetDirectory(0);
      emc[j]->SetMarkerStyle(21); //Box
      emc[j]->SetMarkerColor(i+1);
      emc[j]->SetMarkerSize(1);

      if(plotted){
        emc[j]->Draw("SAME");
      }else{
        emc[j]->GetYaxis()->SetRangeUser(0.6,1.4);
        emc[j]->SetTitle("Helium-3 EMC Ratio");
        emc[j]->Draw("P");
        plotted = true;
      }

      legend->AddEntry(emc[j],Form("Kin %d",i),"p");

      f->Close();
      j++;
    }
  }

  for(Int_t i = 7; i <= 15; i=i+2){
    if( !gSystem->AccessPathName(Form("cleanedKin/kin%d.root",i))){
      TFile *f = new TFile(Form("cleanedKin/kin%d.root",i));
      emc[j] = (TH1D*) gDirectory->Get("emc");
      emc[j]->SetDirectory(0);
      if(j<9){
        emc[j]->SetMarkerStyle(21); //Box
        emc[j]->SetMarkerColor(j+1);
      }else{
        emc[j]->SetMarkerStyle(22); //Triangle
        emc[j]->SetMarkerColor(j-8);
      }
      emc[j]->SetMarkerSize(1);

      if(plotted){
        emc[j]->Draw("SAME");
      }else{
        emc[j]->GetYaxis()->SetRangeUser(0.6,1.4);
        emc[j]->SetTitle("Helium-3 EMC Ratio");
        emc[j]->Draw("P");
        plotted = true;
      }

      legend->AddEntry(emc[j],Form("Kin %d",i),"p");

      f->Close();
      j++;
    }
  }

  if( !gSystem->AccessPathName("cleanedKin/kin16.root")){
    TFile *f = new TFile("cleanedKin/kin16.root");
    emc[j] = (TH1D*) gDirectory->Get("emc");
    emc[j]->SetDirectory(0);
    if(j<9){
      emc[j]->SetMarkerStyle(21); //Box
      emc[j]->SetMarkerColor(j+1);
    }else{
      emc[j]->SetMarkerStyle(22); //Triangle
      emc[j]->SetMarkerColor(j-8);
    }
    emc[j]->SetMarkerSize(1);

    if(plotted){
      emc[j]->Draw("SAME");
    }else{
      emc[j]->GetYaxis()->SetRangeUser(0.6,1.4);
      emc[j]->SetTitle("Helium-3 EMC Ratio");
      emc[j]->Draw("P");
      plotted = true;
    }

    legend->AddEntry(emc[j],"Kin 16","p");

    f->Close();
  }

  legend->Draw();
}
