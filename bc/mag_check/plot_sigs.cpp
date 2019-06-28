void plot_sigs(){
    vector<double> He3;
    vector<double> D2;
    vector<double> x;
    vector<double> ratio;

    ifstream f("sigs.dat");
    while(!f.eof()){
        double tmp;
        f >> tmp;
        if(tmp>0.9){
            break;
        }
        x.push_back(tmp);

        f >> tmp;
        double f2p;
        f >> f2p;
        double f2n;
        f >> f2n;
        f >> tmp;
        f >> tmp;
        f >> tmp;

        f >> tmp;
        D2.push_back(tmp);

        f >> tmp;
        f >> tmp;
        He3.push_back(tmp);

        double rat = He3.back()/D2.back();
        //rat *= 0.5 * (1. + (f2n/f2p));
        //rat /= ((1./3.) * (2. + (f2n/f2p)));
        ratio.push_back(rat);
    }
    f.close();

    TGraph *D2sig = new TGraph(D2.size(),x.data(),D2.data());
    D2sig->SetName("D2");
    D2sig->SetTitle("D2");
    D2sig->SetMarkerStyle(24);
    TGraph *He3sig = new TGraph(He3.size(),x.data(),He3.data());
    He3sig->SetName("He3");
    He3sig->SetTitle("He3");
    He3sig->SetMarkerStyle(24);
    TGraph *ratiosig = new TGraph(ratio.size(),x.data(),ratio.data());
    ratiosig->SetName("ratio");
    ratiosig->SetTitle("ratio");
    ratiosig->SetMarkerStyle(24);

    TCanvas *c1 = new TCanvas();
    D2sig->Draw("APL");

    TCanvas *c2 = new TCanvas();
    c2->cd(0);
    He3sig->Draw("APL");

    TCanvas *c3 = new TCanvas();
    c3->cd(0);
    ratiosig->Draw("APL");
}
