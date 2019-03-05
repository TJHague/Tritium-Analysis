void whitlow_test(){
  string tmp;
  ifstream in("/work/halla/triton/tjhague/Tritium-Analysis/emc/x_for_iso.dat");
  getline(in,tmp);

  ofstream out("whit_test.txt");
  out << "He3x | He3Q2 | D2x | D2Q2 | He3f2p | He3f2d | f2he3 | D2f2 | f2d2" << endl;

  for(int i=1; i<=33; i++){
    double blah, He3x, He3Q2, D2x, D2Q2;
    in >> blah >> blah >> He3x >> He3Q2 >> D2x >> D2Q2;
    if(He3x!=0){
      double He3p = f2(He3x,He3Q2,false);
      double He3d = f2(He3x,He3Q2,true);
      double He3 = He3d + He3p;
      He3 /= 3.;
      double D2f = f2(D2x,D2Q2,true);
      double D2 = D2f/2.;
      out << He3x << " | " << He3Q2 << " | " << D2x << " | " << D2Q2 << " | " << He3p << " | " << He3d << " | " << He3 << " | " << D2f << " | " << D2 << endl;
    }
  }
  in.close();
  out.close();
}
