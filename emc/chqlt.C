{
  TString folder = "p1_test";
  for(int i=0; i<=5; i++){
    check_qlt(Form("%s/kin%d/He3/",folder.Data(),i));
    check_qlt(Form("%s/kin%d/D2/",folder.Data(),i));
  }
  for(int i=0; i<=15; i+=2){
    check_qlt(Form("%s/kin%d_1st/He3/",folder.Data(),i));
    check_qlt(Form("%s/kin%d_1st/D2/",folder.Data(),i));
    check_qlt(Form("%s/kin%d_2nd/He3/",folder.Data(),i));
    check_qlt(Form("%s/kin%d_2nd/D2/",folder.Data(),i));
  }
  check_qlt(Form("%s/kin15_3rd/He3/",folder.Data()));
  check_qlt(Form("%s/kin15_3rd/D2/",folder.Data()));
  check_qlt(Form("%s/kin16_1st/He3/",folder.Data()));
  check_qlt(Form("%s/kin16_1st/D2/",folder.Data()));
  check_qlt(Form("%s/kin16_2nd/He3/",folder.Data()));
  check_qlt(Form("%s/kin16_2nd/D2/",folder.Data()));
}
