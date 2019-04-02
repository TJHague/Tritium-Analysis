void plot_together(int norm){
    gStyle->SetErrorX(0);
    gStyle->SetOptStat(0);
    int total_col=11;
    int col;
    TString plot = "emciso";
    TString normstring;
    if(norm==0){
        col=8; //no norm
        normstring = "No Normalizion";
    }else if(norm==1){
        col=9; //Dp norm
        plot += "norm";
        normstring = "Marathon Normalization";
    }else if(norm==2){
        col=10; //Whitlow Norm
        plot += "whitlow";
        normstring = "Whitlow Normalization";
    }else if(norm==3){
        col=11; //Bodek Norm
        plot += "bodek";
        normstring = "Bodek Normalization";
    }else{
        exit(0);
    }

    string tmp;
    ifstream f("p2_ACCfinal/EMC_Normalizations.csv");
    getline(f,tmp);

    TH1D *H3 = new TH1D("H3","Tritium",33,0,0.99);
    while(!f.eof()){
        double bin_c, bin_e, val, tmp;
        f >> bin_c >> tmp >> bin_e;
        for(int i=4;i<=11;i++){
            if(i==col){
                f >> val;
            }else{
                f >> tmp;
            }
        }
        int bin = int((bin_c+0.015)/0.03);
        H3->SetBinContent(bin,val);
        H3->SetBinError(bin,bin_e);
    }
    f.close();

    ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

    double x[100];
    double he3y[100];
    double h3y[100];

    for(int i=0;i<100;i++){
        double f2p,f2n,f2d,f2he3,f2h3;
        in >> x[i];
        in >> f2p >> f2p >> f2n >> f2d >> f2h3 >> f2he3;

        he3y[i] = f2he3/f2d;
        he3y[i] *= 0.5 * (1. + (f2n/f2p));
        he3y[i] /= ((1./3.) * (2. + (f2n/f2p)));

        h3y[i] = f2h3/f2d;
        h3y[i] *= 0.5 * (1. + (f2n/f2p));
        h3y[i] /= ((1./3.) * (1. + (2.*f2n/f2p)));
    }

    TH1D *he3kp = new TH1D("he3isoKP","He3 EMC Ratio KP",100,x);
    TH1D *h3kp = new TH1D("h3isoKP","H3 EMC Ratio KP",100,x);
    for(int i=0;i<100;i++){
        he3kp->SetBinContent(i+1,he3y[i]);
        h3kp->SetBinContent(i+1,h3y[i]);
    }
    in.close();

    TFile *root = new TFile("p2_ACCfinal/He3.root","UPDATE");
    TH1D *He3 = (TH1D*) root->Get(plot.Data());
    He3->SetDirectory(0);
    root->Close();

    He3->SetAxisRange(0.9,1.05,"Y");
    He3->SetMarkerStyle(24);
    He3->SetMarkerColor(1);
    H3->SetMarkerStyle(25);
    H3->SetMarkerColor(2);
    He3->SetTitle(Form("EMC %s",normstring.Data()));

    he3kp->SetLineColor(1);
    h3kp->SetLineColor(2);

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(He3,"Helium-3 EMC","p");
    legend->AddEntry(H3,"Tritium EMC","p");
    legend->AddEntry(he3kp,"Helium-3 EMC KP Prediction","l");
    legend->AddEntry(h3kp,"Tritium EMC KP Prediction","l");

    He3->Draw();
    H3->Draw("SAME");
    he3kp->Draw("LSAME");
    h3kp->Draw("LSAME");
    legend->Draw();
}
