{
  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

  double x[100];
  double y[100];

  for(int i=0;i<100;i++){
    double f2p,f2n,f2d,f2he3;
    in >> x[i];
    in >> f2p >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

    y[i] = f2he3/f2d;
    y[i] *= 0.5 * (1. + (f2n/f2p));
    y[i] /= ((1./3.) * (2. + (f2n/f2p)));
  }

  TH1D *iso = new TH1D("emcisoKP","He3 EMC Ratio KP",100,x);
  for(int i=0;i<100;i++){
    iso->SetBinContent(i+1,y[i]);
  }
  
  TFile *f = new TFile("p2newz/He3.root","UPDATE");
  iso->Write();

  in.close();
  delete iso;
  delete f;
}
