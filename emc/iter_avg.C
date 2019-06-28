//Creates histograms to manage the average x in a bin
//Should be expanded to Q2 soon
void iter_avg(int kin, TString folder, int nbins, double low, double high, int iter=0){
  TString set="";
  if(kin>=7){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
    }else if(iter==3 && kin==15){
      set = "_3rd";
    }else{
      cout << "1st or 2nd iteration of that kin?" << endl;
      exit(0);
    }
  }

  yieldHistogram *He3full = new yieldHistogram(Form("%s/kin%d%s/He3/full.dat",folder.Data(),kin,set.Data()));
  yieldHistogram *D2full = new yieldHistogram(Form("%s/kin%d%s/D2/full.dat",folder.Data(),kin,set.Data()));

  TFile *f = new TFile(Form("%s/fullKin/kin%d%s.root",folder.Data(),kin,set.Data()),"UPDATE");

  vector<Double_t> He3x = He3full->getAvgx();
  vector<Double_t> D2x = D2full->getAvgx();
  vector<Double_t> He3c = He3full->getCounts();
  vector<Double_t> D2c = D2full->getCounts();

  //I really need to get better at names....
  TH1D *xHe3 = new TH1D("He3xavg","Average binned x value - He3",nbins,low,high);
  TH1D *xD2 = new TH1D("D2xavg","Average binned x value - D2",nbins,low,high);
  TH1D *xemc = new TH1D("emcxavg","Average binned x value - EMC Ratio",nbins,low,high);

  for(int i=0;i<nbins;i++){
    xHe3->SetBinContent(i+1,He3x[i]);
    xD2->SetBinContent(i+1,D2x[i]);
    if(He3c[i]!=0 && D2c[i]!=0){
      xemc->SetBinContent(i+1,(((He3x[i]*He3c[i])+(D2x[i]*D2c[i]))/(He3c[i]+D2c[i])));
    }
  }

  xHe3->Write();
  xD2->Write();
  xemc->Write();

  f->Close();
}
