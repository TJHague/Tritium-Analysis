//Takes KP data file and calculates cross sections

void cc_sig(){
  double E = 10.59;
  double R = 0.18;
  double Mp = 0.938272;
  double alpha = (1.6e-19*1.6e-19)/(4.*TMath::Pi());
  double ahc = 0.00144;

  double He3R = TMath::Sqrt(5.*1.88/3.);


  ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");
  ofstream out("sigeff.dat",ofstream::trunc);

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

    //calculate effective values for coulomb correction
    double He3q2eff = Q2 * TMath::Power((1. + (3.*2.*ahc/(2.*He3R*E))),2.);
    double He3xeff  = x  * TMath::Power((1. + (3.*2.*ahc/(2.*He3R*E))),2.);
    double Dq2eff = Q2 * TMath::Power((1. + (3.*2.*ahc/(2.*DR*E))),2.);
    double Dxeff  = x  * TMath::Power((1. + (3.*2.*ahc/(2.*DR*E))),2.);
    //double He3thetaeff = 2.*TMath::ASin(TMath::Sqrt(Q2eff/(4.*E*Ep)));

    double He3sigeff = TMath::Power(TMath::Tan(theta/2.),2.);
    He3sigeff *= (1. + He3Q2eff/((E-Ep)*(E-Ep)))/(He3xeff*Mp*(1.+R));
    He3sigeff += (1./(E-Ep));
    He3sigeff *= 4.*alpha*alpha*Ep*Ep/(He3Q2eff*He3Q2eff);
    He3sigeff *= TMath::Power(TMath::Cos(theta/2.),2.);
    He3sigeff *= F2He3;

    double Dsigeff = TMath::Power(TMath::Tan(theta/2.),2.);
    Dsigeff *= (1. + DQ2eff/((E-Ep)*(E-Ep)))/(Dxeff*Mp*(1.+R));
    Dsigeff += (1./(E-Ep));
    Dsigeff *= 4.*alpha*alpha*Ep*Ep/(DQ2eff*DQ2eff);
    Dsigeff *= TMath::Power(TMath::Cos(theta/2.),2.);
    Dsigeff *= F2D;

    out << x << "  " << Q2 << "  " << F2p << "  " << F2n << "  " << F2D << "  " << F2H3 << "  " << F2He3 << "  " << sigD << "  " << sigH3 << "  " << sigHe3 << " " << He3sigeff << " " << Dsigeff << " " << 0 << endl;
  }
  in.close();
  out.close();
}
