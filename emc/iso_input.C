{
  TString folder = "newx03";
  
  vector<double> he3avgx(33,0);
  vector<double> he3avgq2(33,0);
  vector<double> he3norm(33,0);
  
  vector<double> d2avgx(33,0);
  vector<double> d2avgq2(33,0);
  vector<double> d2norm(33,0);

  for(int i=0;i<=5;i++){
    yieldHistogram *tmp1 = new yieldHistogram(Form("%s/kin%d/He3/full.dat",folder.Data(),i));
    yieldHistogram *tmp2 = new yieldHistogram(Form("%s/kin%d/D2/full.dat",folder.Data(),i));

    vector<Double_t> x = tmp1->getAvgx();
    vector<Double_t> q2 = tmp1->getAvgQ2();
    vector<Double_t> c = tmp1->getCounts();

    bool first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0){
        if(!first){
          he3avgx[j] += x[j]/c[j];
          he3avgq2[j] += q2[j]/c[j];
          he3norm[j] += 1./c[j];
        }else{
          first = false;
        }
      }
    }
    
    x = tmp2->getAvgx();
    q2 = tmp2->getAvgQ2();
    c = tmp2->getCounts();
    
    first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0&&x[j+1]!=0){
        if(!first){
          d2avgx[j] += x[j]/c[j];
          d2avgq2[j] += q2[j]/c[j];
          d2norm[j] += 1./c[j];
        }else{
          first = false;
        }
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
    vector<Double_t> c = tmp1->getCounts();

    bool first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0&&x[j+1]!=0){
        if(!first){
          he3avgx[j] += x[j]/c[j];
          he3avgq2[j] += q2[j]/c[j];
          he3norm[j] += 1./c[j];
        }else{
          first = false;
        }
      }
    }
    
    x = tmp2->getAvgx();
    q2 = tmp2->getAvgQ2();
    c = tmp2->getCounts();
    
    first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0&&x[j+1]!=0){
        if(!first){
          d2avgx[j] += x[j]/c[j];
          d2avgq2[j] += q2[j]/c[j];
          d2norm[j] += 1./c[j];
        }else{
          first = false;
        }
      }
    }

    x = tmp3->getAvgx();
    q2 = tmp3->getAvgQ2();
    c = tmp3->getCounts();
    
    first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0&&x[j+1]!=0){
        if(!first){
          he3avgx[j] += x[j]/c[j];
          he3avgq2[j] += q2[j]/c[j];
          he3norm[j] += 1./c[j];
        }else{
          first = false;
        }
      }
    }

    x = tmp4->getAvgx();
    q2 = tmp4->getAvgQ2();
    c = tmp4->getCounts();
    
    first = true;
    for(int j=0;j<x.size();j++){
      if(x[j]!=0&&x[j+1]!=0){
        if(!first){
          d2avgx[j] += x[j]/c[j];
          d2avgq2[j] += q2[j]/c[j];
          d2norm[j] += 1./c[j];
        }else{
          first = false;
        }
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
  vector<Double_t> c = tmp1->getCounts();

  bool first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        he3avgx[j] += x[j]/c[j];
        he3avgq2[j] += q2[j]/c[j];
        he3norm[j] += 1./c[j];
      }else{
        first = false;
      }
    }
  }
  
  x = tmp2->getAvgx();
  q2 = tmp2->getAvgQ2();
  c = tmp2->getCounts();
  
  first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        d2avgx[j] += x[j]/c[j];
        d2avgq2[j] += q2[j]/c[j];
        d2norm[j] += 1./c[j];
      }else{
        first = false;
      }
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
  c = tmp1->getCounts();

  first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        he3avgx[j] += x[j]/c[j];
        he3avgq2[j] += q2[j]/c[j];
        he3norm[j] += 1./c[j];
      }else{
        first = false;
      }
    }
  }
  
  x = tmp2->getAvgx();
  q2 = tmp2->getAvgQ2();
  c = tmp2->getCounts();
  
  first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        d2avgx[j] += x[j]/c[j];
        d2avgq2[j] += q2[j]/c[j];
        d2norm[j] += 1./c[j];
      }else{
        first = false;
      }
    }
  }

  x = tmp3->getAvgx();
  q2 = tmp3->getAvgQ2();
  c = tmp3->getCounts();
  
  first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        he3avgx[j] += x[j]/c[j];
        he3avgq2[j] += q2[j]/c[j];
        he3norm[j] += 1./c[j];
      }else{
        first = false;
      }
    }
  }

  x = tmp4->getAvgx();
  q2 = tmp4->getAvgQ2();
  c = tmp4->getCounts();
  
  first = true;
  for(int j=0;j<x.size();j++){
    if(x[j]!=0&&x[j+1]!=0){
      if(!first){
        d2avgx[j] += x[j]/c[j];
        d2avgq2[j] += q2[j]/c[j];
        d2norm[j] += 1./c[j];
      }else{
        first = false;
      }
    }
  }

  for(int j=0;j<he3avgx.size();j++){
    if(he3avgx[j]!=0.){
      he3avgx[j]/=he3norm[j];
      he3avgq2[j]/=he3norm[j];
    }
  }
  for(int j=0;j<d2avgx.size();j++){
    if(d2avgx[j]!=0.){
      d2avgx[j]/=d2norm[j];
      d2avgq2[j]/=d2norm[j];
    }
  }

  /*for(int j=0;j<he3avgx.size();j++){
    if(he3avgx[j]!=0.){
      cout << he3avgx[j] << " " << he3avgq2[j] << " " << d2avgx[j] << " " << d2avgq2[j] << endl;
    }
  }*/

  TFile *f = new TFile(Form("%s/He3.root",folder.Data()));
  TH1D *ratio = (TH1D*) gDirectory->Get("emc");

  ofstream out("x_for_iso.dat");
  out << "Bin Center | He3/D2 | x avg (He3) | Q2 avg (He3) | x avg (D2) | Q2 avg (D2)" << endl;
  for(int j=1;j<=ratio->GetNbinsX();j++){
    out << ratio->GetBinCenter(j) << "        " << ratio->GetBinContent(j) << "  " << he3avgx[j-1] << "      " << he3avgq2[j-1] << "        " << d2avgx[j-1] << "     " << d2avgq2[j-1] << endl;
  }
  delete f;

  delete tmp1;
  delete tmp2;
  delete tmp3;
  delete tmp4;
}
