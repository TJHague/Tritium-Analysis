{
  for(int i=0;i<=5;i++){
    cout << i << endl;
    kinYield(Form("x_03/kin%d/He3/",i));
    kinYield(Form("x_03/kin%d/D2/",i));
  }
  for(int i=7; i<=13; i+=2){
    cout << i << endl;
    kinYield(Form("x_03/kin%d_1st/He3/",i));
    kinYield(Form("x_03/kin%d_1st/D2/",i));
    kinYield(Form("x_03/kin%d_2nd/He3/",i));
    kinYield(Form("x_03/kin%d_2nd/D2/",i));
  }
  kinYield("x_03/kin15_1st/He3/");
  kinYield("x_03/kin15_2nd/He3/");
  kinYield("x_03/kin15_3rd/He3/");
  kinYield("x_03/kin15_1st/D2/");
  kinYield("x_03/kin15_2nd/D2/");
  kinYield("x_03/kin15_3rd/D2/");
  kinYield("x_03/kin16_1st/He3/");
  kinYield("x_03/kin16_2nd/He3/");
  kinYield("x_03/kin16_1st/D2/");
  kinYield("x_03/kin16_2nd/D2/");
}
