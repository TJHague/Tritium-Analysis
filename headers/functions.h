double bivargaus(double *x, double *par){
  double z = (x[0] - par[1])*(x[0] - par[1])/(par[2] * par[2]);
  z += (x[1] - par[3])*(x[1] - par[3])/(par[4] * par[4]);
  z -= 2.*par[5]*(x[0] - par[1])*(x[1] - par[3])/(par[2] * par[4]);
  double result = 1./(2. * TMath::Pi() * par[2] * par[4] * TMath::Sqrt(1. - (par[5] * par[5])));
  result *= TMath::Exp(-1. * z / (2. * (1. - (par[5] * par[5]))));
  result *= par[0];
  return result;
}
