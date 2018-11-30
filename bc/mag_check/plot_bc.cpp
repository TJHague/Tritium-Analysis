#include <iostream>
#include <vector>

void plot_bc(int bins){
  vector<double> He3bin;
  vector<double> D2bin;
  vector<double> xbin;

  ifstream f("sigs.dat");
  while(!f.eof()){
    double tmp;
    f >> tmp;
    if(tmp>0.9){
      break;
    }
    xbin.push_back(tmp);

    for(int i=0;i<6;i++){
      f >> tmp;
    }

    f >> tmp;
    D2bin.push_back(tmp);

    f >> tmp;
    f >> tmp;
    He3bin.push_back(tmp);
  }
  f.close();

  vector<double> He3avg;
  vector<double> D2avg;

  for(int i=0; i+bins<xbin.size(); i++){
    He3avg.push_back(0);
    D2avg.push_back(0);
    for(int j=0;j<bins;j++){
      He3avg[i]+=He3bin[i+j];
      D2avg[i]+=D2bin[i+j];
    }
    He3avg[i]/=bins;
    D2avg[i]/=bins;
  }

  int diff = (bins-1)/2;

  double x[100], He3[100], D2[100], ratio[100];
  for(int i=0; i<He3avg.size(); i++){
    x[i]=xbin[i+diff];
    He3[i]=He3avg[i]/He3bin[i+diff];
    D2[i]=D2avg[i]/D2bin[i+diff];
    ratio[i]=He3[i]/D2[i];
    cout << x[i] << " " << He3[i] << " " << D2[i] << " " << ratio[i] << " " << endl;
  }

  gStyle->SetOptStat(0);
  auto legend = new TLegend(.1,0.7,0.48,0.9);
  TGraph *He3bcc = new TGraph(He3avg.size(),x,He3);
  TGraph *D2bcc = new TGraph(He3avg.size(),x,D2);
  TGraph *ratiobcc = new TGraph(He3avg.size(),x,ratio);

  He3bcc->SetMarkerStyle(24);
  D2bcc->SetMarkerStyle(25);
  ratiobcc->SetMarkerStyle(26);

  He3bcc->SetMarkerColor(1);
  D2bcc->SetMarkerColor(2);
  ratiobcc->SetMarkerColor(3);

  legend->AddEntry(He3bcc,"Helium-3 BCC","p");
  legend->AddEntry(D2bcc,"Deuterium BCC","p");
  legend->AddEntry(ratiobcc,"Ratio of BCCs","p");

  He3bcc->Draw("AP");
  D2bcc->Draw("PSAME");
  ratiobcc->Draw("PSAME");
  legend->Draw();

}
