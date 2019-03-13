void plot_ECC_noedge(TString target, TString folder){
    int x = (target=="H1") ? 4 : 16;
    int y = (target=="H1") ? 4 : 11;
    gStyle->SetErrorX(0);
    TFile *f = new TFile(folder + "/" + target + ".root");
    TH1D* raw[12];
    TH1D* plots[12];
    int j = 0;
    for(int i=0; i<=x; i++){
        if(i<=5 || i%2==1 || i==16){
	        cout << i << " " << j << endl;
            raw[j] = (TH1D*) f->Get(Form("ecc_kin%d",i));
            raw[j]->SetDirectory(0);
            j += 1;
        }
    }
    f->Close();
    
    for(int i=0; i<=y; i++){
      plots[i] = new TH1D(raw[i]->GetName(),raw[i]->GetTitle(),33,0,0.99);
      bool first = true;
      for(int k=0; k<33; k++){
        double c = raw[i]->GetBinContent(k);
        if(c!=0){
          if(!first){
            double d = raw[i]->GetBinContent(k+1);
            if(d!=0){
              plots[i]->SetBinContent(k,c);
              plots[i]->SetBinError(k,raw[i]->GetBinError(k));
            }else{
              break;
            }
          }else{
            first = false;
          }
        }
      }
    }

    auto legend = new TLegend(0.1,0.7,0.48,0.9);

    plots[0]->SetMarkerColor(1);
    plots[0]->SetMarkerStyle(24);
    plots[0]->SetDirectory(0);
    plots[0]->SetTitle(Form("%s Endcap Contamination",target.Data()));
    plots[0]->SetAxisRange(0,0.05,"Y");
    plots[0]->Draw("P");
    legend->AddEntry(plots[0],"Kinematic 0","p");
    for(int i=1;i<=y;i++){
        plots[i]->SetMarkerColor(1+(int)(i%4));
        plots[i]->SetMarkerStyle(24+(int)(i%5));
        plots[i]->Draw("PSAME");
        int kin = (i<=5) ? i : ((i - 5) * 2) + 5;
        kin = (kin==17) ? 16 : kin;
        legend->AddEntry(plots[i],Form("Kinematic %d",kin),"p");
    }
    legend->Draw();
}
