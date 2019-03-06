void plot_ECC(TString target){
    int x = (target=="H1") ? 4 : 15;
    int y = (target=="H1") ? 4 : 10;
    gStyle->SetErrorX(0);
    TFile *f = new TFile(target + ".root");
    TH1D* plots[11];
    int j = 0;
    for(int i=0; i<=x; i++){
        if(i<=5 || i%2==1){
	        cout << i << " " << j << endl;
            plots[j] = (TH1D*) f->Get(Form("ecc_kin%d",i));
            j += 1;
        }
    }
    auto legend = new TLegend(0.1,0.7,0.48,0.9);

    plots[0]->SetMarkerColor(1);
    plots[0]->SetMarkerStyle(24);
    plots[0]->SetDirectory(0);
    plots[0]->SetTitle(Form("%s Endcap Contamination",target.Data()));
    plots[0]->SetAxisRange(0,0.05,"Y");
    plots[0]->Draw();
    legend->AddEntry(plots[0],"Kinematic 0","p");
    for(int i=1;i<=y;i++){
        plots[i]->SetMarkerColor(1+(int)(i%4));
        plots[i]->SetMarkerStyle(24+(int)(i%5));
        plots[i]->Draw("SAME");
        int kin = (i<=5) ? i : ((i - 5) * 2) + 5;
        legend->AddEntry(plots[i],Form("Kinematic %d",kin),"p");
    }
    legend->Draw();
}
