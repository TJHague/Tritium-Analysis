//Takes KP data file and calculates cross sections

void gen_sig(){
  double E = 10.59;
  double R = 0.18;
  double Mp = 0.938272;
  double alpha = (1.6e-19*1.6e-19)/(4.*TMath::Pi());;
  ifstream in("F2_ratio/F2dis_os1tm1ht1mec1_Dav18_He3Salme");
  ofstream out("KPsig.csv",ofstream::trunc);

  out << "x,sigD,sigHe3,ratio" << endl;

  while(!in.eof()){
    double x, Q2, F2p, F2n, F2D, F2H3, F2He3;
    in >> x >> Q2 >> F2p >> F2n >> F2D >> F2H3 >> F2He3;
    double Ep = E-(Q2/(2.*Mp*x));
    double theta = 2.*TMath::ASin(TMath::Sqrt(Q2/(4.*E*Ep)));

    double sigD = TMath::Power(TMath::Tan(theta/2.),2.);
    sigD *= (1. + Q2/((E-Ep)*(E-Ep)))/(x*Mp*(1.+R));
    sigD += (1./(E-Ep));
    sigD *= 4.*alpha*alpha*Ep*Ep/(Q2*Q2);
    sigD *= TMath::Power(TMath::Cos(theta/2.),2.);

    double sigHe3 = sigD*F2He3;
    double sigH3 = sigD*F2H3;
    sigD *= F2D;

    out << x << "," << sigD << "," << sigHe3 << "," << sigHe3/sigD << endl;
  }
  in.close();
  out.close();
}
