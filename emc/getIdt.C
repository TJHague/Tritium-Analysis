{
  TString folder = "p2_allerr";
  ofstream out("per_kin.csv");
  out << "kin,He3I,He3dt,H2I,H2dt" << endl;
  for(int i=0;i<=5;i++){
    yieldHistogram *He3 = new yieldHistogram(Form("%s/kin%d/He3/full.dat",folder.Data(),i));
    yieldHistogram *D2 = new yieldHistogram(Form("%s/kin%d/D2/full.dat",folder.Data(),i));
    out << i << "," << He3->getAvgI() << "," << 1.-He3->getLivetime() << "," << D2->getAvgI() << "," << 1.-D2->getLivetime() << endl;
    delete He3;
    delete D2;
  }
  for(int i=7;i<=13;i+=2){
    yieldHistogram *He3 = new yieldHistogram(Form("%s/kin%d_1st/He3/full.dat",folder.Data(),i));
    yieldHistogram *He32 = new yieldHistogram(Form("%s/kin%d_2nd/He3/full.dat",folder.Data(),i));
    He3->add(He32);
    yieldHistogram *D2 = new yieldHistogram(Form("%s/kin%d_1st/D2/full.dat",folder.Data(),i));
    yieldHistogram *D22 = new yieldHistogram(Form("%s/kin%d_2nd/D2/full.dat",folder.Data(),i));
    D2->add(D22);
    out << i << "," << He3->getAvgI() << "," << 1.-He3->getLivetime() << "," << D2->getAvgI() << "," << 1.-D2->getLivetime() << endl;
    delete He3;
    delete He32;
    delete D2;
    delete D22;
  }
  int i=15;
  yieldHistogram *He3 = new yieldHistogram(Form("%s/kin%d_1st/He3/full.dat",folder.Data(),i));
  yieldHistogram *He32 = new yieldHistogram(Form("%s/kin%d_2nd/He3/full.dat",folder.Data(),i));
  yieldHistogram *He33 = new yieldHistogram(Form("%s/kin%d_3rd/He3/full.dat",folder.Data(),i));
  He3->add(He32);
  He3->add(He33);
  yieldHistogram *D2 = new yieldHistogram(Form("%s/kin%d_1st/D2/full.dat",folder.Data(),i));
  yieldHistogram *D22 = new yieldHistogram(Form("%s/kin%d_2nd/D2/full.dat",folder.Data(),i));
  yieldHistogram *D23 = new yieldHistogram(Form("%s/kin%d_3rd/D2/full.dat",folder.Data(),i));
  D2->add(D22);
  D2->add(D23);
  out << i << "," << He3->getAvgI() << "," << 1.-He3->getLivetime() << "," << D2->getAvgI() << "," << 1.-D2->getLivetime() << endl;

  delete He3;
  delete He32;
  delete He33;
  delete D2;
  delete D22;
  delete D23;

  i=16;
  He3 = new yieldHistogram(Form("%s/kin%d_1st/He3/full.dat",folder.Data(),i));
  He32 = new yieldHistogram(Form("%s/kin%d_2nd/He3/full.dat",folder.Data(),i));
  He3->add(He32);
  D2 = new yieldHistogram(Form("%s/kin%d_1st/D2/full.dat",folder.Data(),i));
  D22 = new yieldHistogram(Form("%s/kin%d_2nd/D2/full.dat",folder.Data(),i));
  D2->add(D22);
  out << i << "," << He3->getAvgI() << "," << 1.-He3->getLivetime() << "," << D2->getAvgI() << "," << 1.-D2->getLivetime() << endl;
  
  delete He3;
  delete He32;
  delete D2;
  delete D22;

  out.close();
}
