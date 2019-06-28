void gen_radinp(TString folder){
  ofstream heinp(Form("%s/He3.inp",folder.Data()));
  ofstream dinp(Form("%s/D2.inp",folder.Data()));
  heinp << "n   xbj   Q2" << endl;
  dinp << "n   xbj   Q2" << endl;

  int n = 0;
  for(int i=0;i<=5;i++){
    yieldHistogram *tmp1 = new yieldHistogram(Form("%s/kin%d/He3/full.dat",folder.Data(),i));
    yieldHistogram *tmp2 = new yieldHistogram(Form("%s/kin%d/D2/full.dat",folder.Data(),i));

    vector<Double_t> x = tmp1->getAvgx();
    vector<Double_t> q2 = tmp1->getAvgQ2();

    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        heinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }
    
    x = tmp2->getAvgx();
    q2 = tmp2->getAvgQ2();
    
    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        dinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }

    delete tmp1;
    delete tmp2;
  }
  for(int i=7;i<=15;i+=2){
    yieldHistogram *tmp1 = new yieldHistogram(Form("%s/kin%d_1st/He3/full.dat",folder.Data(),i));
    yieldHistogram *tmp2 = new yieldHistogram(Form("%s/kin%d_1st/D2/full.dat",folder.Data(),i));
    yieldHistogram *tmp3 = new yieldHistogram(Form("%s/kin%d_2nd/He3/full.dat",folder.Data(),i));
    yieldHistogram *tmp4 = new yieldHistogram(Form("%s/kin%d_2nd/D2/full.dat",folder.Data(),i));

    vector<Double_t> x = tmp1->getAvgx();
    vector<Double_t> q2 = tmp1->getAvgQ2();

    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        heinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }
    
    x = tmp2->getAvgx();
    q2 = tmp2->getAvgQ2();
    
    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        dinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }

    x = tmp3->getAvgx();
    q2 = tmp3->getAvgQ2();
    
    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        heinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }

    x = tmp4->getAvgx();
    q2 = tmp4->getAvgQ2();
    
    for(unsigned int j=0;j<x.size();j++){
      if(x[j]!=0){
        dinp << n << ", " << x[j] << ", " << q2[j] << endl;
        n++;
      }
    }


    delete tmp1;
    delete tmp2;
    delete tmp3;
    delete tmp4;
  }

  yieldHistogram *tmp1 = new yieldHistogram(Form("%s/kin15_3rd/He3/full.dat",folder.Data()));
  yieldHistogram *tmp2 = new yieldHistogram(Form("%s/kin15_3rd/D2/full.dat",folder.Data()));
  
  vector<Double_t> x = tmp1->getAvgx();
  vector<Double_t> q2 = tmp1->getAvgQ2();

  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      heinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }
  
  x = tmp2->getAvgx();
  q2 = tmp2->getAvgQ2();
  
  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      dinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }

  delete tmp1;
  delete tmp2;

  tmp1 = new yieldHistogram(Form("%s/kin16_1st/He3/full.dat",folder.Data()));
  tmp2 = new yieldHistogram(Form("%s/kin16_1st/D2/full.dat",folder.Data()));
  yieldHistogram *tmp3 = new yieldHistogram(Form("%s/kin16_2nd/He3/full.dat",folder.Data()));
  yieldHistogram *tmp4 = new yieldHistogram(Form("%s/kin16_2nd/D2/full.dat",folder.Data()));

  x = tmp1->getAvgx();
  q2 = tmp1->getAvgQ2();

  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      heinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }
  
  x = tmp2->getAvgx();
  q2 = tmp2->getAvgQ2();
  
  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      dinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }

  x = tmp3->getAvgx();
  q2 = tmp3->getAvgQ2();
  
  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      heinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }

  x = tmp4->getAvgx();
  q2 = tmp4->getAvgQ2();
  
  for(unsigned int j=0;j<x.size();j++){
    if(x[j]!=0){
      dinp << n << ", " << x[j] << ", " << q2[j] << endl;
      n++;
    }
  }


  delete tmp1;
  delete tmp2;
  delete tmp3;
  delete tmp4;

  heinp.close();
  dinp.close();
}
