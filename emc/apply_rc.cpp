void apply_rc(TString folder, TString inf="fullKin", TString outf="radKin"){
  /*TString set = "";
  if(kin>=7){
    if(iter==1){
      set = "_1st";
    }else if(iter==2){
      set = "_2nd";
    }else if(iter==3 && kin==15){
      set = "_3rd";
    }
  }*/
  //Have to remove first line from stream
  string tmp;
  ifstream He3rc("/work/halla/triton/tjhague/radcor/T2_externals/OUT/He3emc_xs.out");
  getline(He3rc,tmp);
  ifstream D2rc("/work/halla/triton/tjhague/radcor/T2_externals/OUT/D2emc_xs.out");
  getline(D2rc,tmp);
  cout <<"test" << endl;
  for(int i=0;i<=5;i++){
    cout <<"1" <<endl;
    TFile *a = new TFile(Form("%s/%s/kin%d.root",folder.Data(),inf.Data(),i),"UPDATE");
    cout <<"2" <<endl;
    TH1D *He3clean = (TH1D*) gDirectory->Get("He3full");
    He3clean->SetDirectory(0);
    cout <<"3" <<endl;
    TH1D *He3 = (TH1D*) He3clean->Clone("He3rad");
    cout <<"3" <<endl;
    He3->SetDirectory(0);
    cout <<"4" <<endl;
    TH1D *D2clean = (TH1D*) gDirectory->Get("D2full");
    D2clean->SetDirectory(0);
    TH1D *D2 = (TH1D*) D2clean->Clone("D2rad");
    cout <<"5" <<endl;
    D2->SetDirectory(0);
//    TH1D *emc = (TH1D*) gDirectory->Get("emc");
    delete a;
    TH1D *emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
    emc->Sumw2();

    cout << "He3" << endl;
    for(int j=1; j<=He3->GetNbinsX(); j++){
      if(He3->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          He3rc >> born;
        }
        He3rc >> rad;
        cout << born/rad << endl;
        He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
      }
    }
    cout << "D2" << endl;
    for(int j=1; j<=D2->GetNbinsX(); j++){
      if(D2->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          D2rc >> born;
        }
        D2rc >> rad;
        cout << born/rad << endl;
        D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
      }
    }
    emc->Add(He3);
    emc->Divide(D2);

    a = new TFile(Form("%s/%s/kin%d.root",folder.Data(),outf.Data(),i),"RECREATE");
    He3->Write();
    D2->Write();
    emc->Write();
    delete a;
    delete emc;
  }
 for(int i=7;i<15;i+=2){
    TFile *a = new TFile(Form("%s/%s/kin%d_1st.root",folder.Data(),inf.Data(),i),"UPDATE");
    TH1D *He3clean = (TH1D*) gDirectory->Get("He3full");
    TH1D *He3 = (TH1D*) He3clean->Clone("He3rad");
    He3->SetDirectory(0);
    TH1D *D2clean = (TH1D*) gDirectory->Get("D2full");
    TH1D *D2 = (TH1D*) D2clean->Clone("D2rad");
    D2->SetDirectory(0);
//    TH1D *emc = (TH1D*) gDirectory->Get("emc");
    delete a;
    TH1D *emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());

    for(int j=1; j<=He3->GetNbinsX(); j++){
      if(He3->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          He3rc >> born;
        }
        He3rc >> rad;
        He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
      }
    }
    for(int j=1; j<=D2->GetNbinsX(); j++){
      if(D2->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          D2rc >> born;
        }
        D2rc >> rad;
        D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
      }
    }
//    
    emc->Sumw2();
    emc->Add(He3);
    emc->Divide(D2);

    a = new TFile(Form("%s/%s/kin%d_1st.root",folder.Data(),outf.Data(),i),"RECREATE");
    He3->Write();
    D2->Write();
    emc->Write();
    delete a;
    delete emc;

    a = new TFile(Form("%s/%s/kin%d_2nd.root",folder.Data(),inf.Data(),i),"UPDATE");
    He3clean = (TH1D*) gDirectory->Get("He3full");
    He3 = (TH1D*) He3clean->Clone("He3rad");
    He3->SetDirectory(0);
    D2clean = (TH1D*) gDirectory->Get("D2full");
    D2 = (TH1D*) D2clean->Clone("D2rad");
    D2->SetDirectory(0);
//    emc = (TH1D*) gDirectory->Get("emc");
    delete a;
    emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
    emc->Sumw2();
    for(int j=1; j<=He3->GetNbinsX(); j++){
      if(He3->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          He3rc >> born;
        }
        He3rc >> rad;
        He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
      }
    }
    for(int j=1; j<=D2->GetNbinsX(); j++){
      if(D2->GetBinContent(j)!=0){
        double born, rad;
        for(int k=0;k<5;k++){
          D2rc >> born;
        }
        D2rc >> rad;
        D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
      }
    }
//    
    emc->Add(He3);
    emc->Divide(D2);

    a = new TFile(Form("%s/%s/kin%d_2nd.root",folder.Data(),outf.Data(),i),"RECREATE");
    He3->Write();
    D2->Write();
    emc->Write();
    delete a;
    delete emc;
    cout << "maybe?" << endl;
  }
  cout << "end of loops" << endl;
  TFile *a = new TFile(Form("%s/%s/kin15_1st.root",folder.Data(),inf.Data()),"UPDATE");
  TH1D *He3clean = (TH1D*) gDirectory->Get("He3full");
  TH1D *He3 = (TH1D*) He3clean->Clone("He3rad");
  He3->SetDirectory(0);
  TH1D *D2clean = (TH1D*) gDirectory->Get("D2full");
  TH1D *D2 = (TH1D*) D2clean->Clone("D2rad");
  D2->SetDirectory(0);
//  TH1D *emc = (TH1D*) gDirectory->Get("emc");
  delete a;
  TH1D *emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
  emc->Sumw2();

  for(int j=1; j<=He3->GetNbinsX(); j++){
    if(He3->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        He3rc >> born;
      }
      He3rc >> rad;
      He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
    }
  }
  for(int j=1; j<=D2->GetNbinsX(); j++){
    if(D2->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        D2rc >> born;
      }
      D2rc >> rad;
      D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
    }
  }
  //
  emc->Add(He3);
  emc->Divide(D2);

  a = new TFile(Form("%s/%s/kin15_1st.root",folder.Data(),outf.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  emc->Write();
  delete a;
  delete emc;

  a = new TFile(Form("%s/%s/kin15_2nd.root",folder.Data(),inf.Data()),"UPDATE");
  He3clean = (TH1D*) gDirectory->Get("He3full");
  He3 = (TH1D*) He3clean->Clone("He3rad");
  He3->SetDirectory(0);
  D2clean = (TH1D*) gDirectory->Get("D2full");
  D2 = (TH1D*) D2clean->Clone("D2rad");
  D2->SetDirectory(0);
//  emc = (TH1D*) gDirectory->Get("emc");
  delete a;
  emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
  emc->Sumw2();

  for(int j=1; j<=He3->GetNbinsX(); j++){
    if(He3->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        He3rc >> born;
      }
      He3rc >> rad;
      He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
    }
  }
  for(int j=1; j<=D2->GetNbinsX(); j++){
    if(D2->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        D2rc >> born;
      }
      D2rc >> rad;
      D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
    }
  }
//  
  emc->Add(He3);
  emc->Divide(D2);
  
  a = new TFile(Form("%s/%s/kin15_2nd.root",folder.Data(),outf.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  emc->Write();
  delete a;
  delete emc;

  a = new TFile(Form("%s/%s/kin15_3rd.root",folder.Data(),inf.Data()),"UPDATE");
  He3clean = (TH1D*) gDirectory->Get("He3full");
  He3 = (TH1D*) He3clean->Clone("He3rad");
  He3->SetDirectory(0);
  D2clean = (TH1D*) gDirectory->Get("D2full");
  D2 = (TH1D*) D2clean->Clone("D2rad");
  D2->SetDirectory(0);
//  emc = (TH1D*) gDirectory->Get("emc");
  delete a;
  emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
  emc->Sumw2();

  for(int j=1; j<=He3->GetNbinsX(); j++){
    if(He3->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        He3rc >> born;
      }
      He3rc >> rad;
      He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
    }
  }
  for(int j=1; j<=D2->GetNbinsX(); j++){
    if(D2->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        D2rc >> born;
      }
      D2rc >> rad;
      D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
    }
  }
  
  emc->Add(He3);
  emc->Divide(D2);

  a = new TFile(Form("%s/%s/kin15_3rd.root",folder.Data(),outf.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  emc->Write();
  delete a;
  delete emc;

  a = new TFile(Form("%s/%s/kin16_1st.root",folder.Data(),inf.Data()),"UPDATE");
  He3clean = (TH1D*) gDirectory->Get("He3full");
  He3 = (TH1D*) He3clean->Clone("He3rad");
  He3->SetDirectory(0);
  D2clean = (TH1D*) gDirectory->Get("D2full");
  D2 = (TH1D*) D2clean->Clone("D2rad");
  D2->SetDirectory(0);
//  emc = (TH1D*) gDirectory->Get("emc");
  delete a;
  emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
  emc->Sumw2();

  for(int j=1; j<=He3->GetNbinsX(); j++){
    if(He3->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        He3rc >> born;
      }
      He3rc >> rad;
      He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
    }
  }
  for(int j=1; j<=D2->GetNbinsX(); j++){
    if(D2->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        D2rc >> born;
      }
      D2rc >> rad;
      D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
    }
  }
  
  emc->Add(He3);
  emc->Divide(D2);

  a = new TFile(Form("%s/%s/kin16_1st.root",folder.Data(),outf.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  emc->Write();
  delete a;
  delete emc;

  a = new TFile(Form("%s/%s/kin16_2nd.root",folder.Data(),inf.Data()),"UPDATE");
  He3clean = (TH1D*) gDirectory->Get("He3full");
  He3 = (TH1D*) He3clean->Clone("He3rad");
  He3->SetDirectory(0);
  D2clean = (TH1D*) gDirectory->Get("D2full");
  D2 = (TH1D*) D2clean->Clone("D2rad");
  D2->SetDirectory(0);
//  emc = (TH1D*) gDirectory->Get("emc");
  delete a;
  emc = new TH1D("emcrad","He-3 EMC Ratio",He3->GetNbinsX(),He3->GetXaxis()->GetXmin(),He3->GetXaxis()->GetXmax());
  emc->Sumw2();

  for(int j=1; j<=He3->GetNbinsX(); j++){
    if(He3->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        He3rc >> born;
      }
      He3rc >> rad;
      He3->SetBinContent(j,He3->GetBinContent(j)*born/rad);
    }
  }
  for(int j=1; j<=D2->GetNbinsX(); j++){
    if(D2->GetBinContent(j)!=0){
      double born, rad;
      for(int k=0;k<5;k++){
        D2rc >> born;
      }
      D2rc >> rad;
      D2->SetBinContent(j,D2->GetBinContent(j)*born/rad);
    }
  }
  a = new TFile(Form("%s/%s/kin16_2nd.root",folder.Data(),outf.Data()),"RECREATE");
  He3->Write();
  D2->Write();
  
  emc->Add(He3);
  emc->Divide(D2);
  emc->Write();
  delete a;
  delete emc;

  He3rc.close();
  D2rc.close();
}
