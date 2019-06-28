{
  TString folder = "p2_allerr";
  int bins = 33;
  double min = 0;
  double max = .99;

  for(int i = 0; i <= 5; i++){
    cout << i << endl;
    //emc(i,folder,bins,min,max);
    /*kinYield(Form("%s/kin%d/He3/",folder.Data(),i));
    kinYield(Form("%s/kin%d/D2/",folder.Data(),i));*/
    ind_cor(i,folder,bins,min,max);
    iter_avg(i,folder,bins,min,max);
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3");
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3lt");
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3boiling");
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3pos");
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3ecc");
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3nocor");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2lt");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2boiling");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2pos");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2ecc");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2nocor");
  }
  for(int i = 7; i <= 15; i+=2){
    cout << i << endl;
    //emc(i,folder,bins,min,max,1);
    //emc(i,folder,bins,min,max,2);
    /*kinYield(Form("%s/kin%d_1st/He3/",folder.Data(),i));
    kinYield(Form("%s/kin%d_2nd/He3/",folder.Data(),i));
    kinYield(Form("%s/kin%d_1st/D2/",folder.Data(),i));
    kinYield(Form("%s/kin%d_2nd/D2/",folder.Data(),i));*/
    ind_cor(i,folder,bins,min,max,1);
    ind_cor(i,folder,bins,min,max,2);
    iter_avg(i,folder,bins,min,max,1);
    iter_avg(i,folder,bins,min,max,2);
    if(i != 15){
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3");
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3lt");
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3boiling");
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3pos");
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3ecc");
      kin_clean_new(i,folder,"fullKin","cleanedKin","He3nocor");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2lt");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2boiling");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2pos");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2ecc");
      kin_clean_new(i,folder,"fullKin","cleanedKin","D2nocor");
    }
  }
  //emc(15,folder,bins,min,max,3);
  /*kinYield(Form("%s/kin15_3rd/He3/",folder.Data()));
  kinYield(Form("%s/kin15_3rd/D2/",folder.Data()));*/
  ind_cor(15,folder,bins,min,max,3);
  iter_avg(15,folder,bins,min,max,3);
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3");
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3lt");
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3boiling");
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3pos");
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3ecc");
  kin_clean_new(15,folder,"fullKin","cleanedKin","He3nocor");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2lt");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2boiling");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2pos");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2ecc");
  kin_clean_new(15,folder,"fullKin","cleanedKin","D2nocor");
  cout << "16" << endl;
  //emcR(16,folder,bins,min,max,1);
  //emcR(16,folder,bins,min,max,2);
  /*kinYield(Form("%s/kin16_1st/He3/",folder.Data()));
  kinYield(Form("%s/kin16_2nd/He3/",folder.Data()));
  kinYield(Form("%s/kin16_1st/D2/",folder.Data()));
  kinYield(Form("%s/kin16_2nd/D2/",folder.Data()));*/
  ind_cor(16,folder,bins,min,max,1);
  ind_cor(16,folder,bins,min,max,2);
  iter_avg(16,folder,bins,min,max,1);
  iter_avg(16,folder,bins,min,max,2);
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3");
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3lt");
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3boiling");
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3pos");
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3ecc");
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3nocor");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2lt");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2boiling");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2pos");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2ecc");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2nocor");

  //gen_radinp(folder);

  cout << "done with part 1" << endl << "calculate radiative corrections now" << endl;

  apply_rc_new(folder);
  apply_rc_new(folder,"fullKin","","");

  for(int i=0;i<=5;i++){
    cout << i << endl;
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3rad");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2rad");
    kin_clean_new(i,folder,"fullKin","cleanedKin","emc");
    clean_avg(i,folder);
  }
  for(int i=7;i<=15;i+=2){
    cout << i << endl;
    kin_clean_new(i,folder,"fullKin","cleanedKin","He3rad");
    kin_clean_new(i,folder,"fullKin","cleanedKin","D2rad");
    kin_clean_new(i,folder,"fullKin","cleanedKin","emc");
    clean_avg(i,folder);
  }
  cout << 16 << endl;
  kin_clean_new(16,folder,"fullKin","cleanedKin","He3rad");
  kin_clean_new(16,folder,"fullKin","cleanedKin","D2rad");
  kin_clean_new(16,folder,"fullKin","cleanedKin","emc");
  clean_avg(16,folder);

  TString out = "He3";

  merge_all_new(folder,"He3",out);
  merge_all_new(folder,"He3lt",out,true);
  merge_all_new(folder,"He3boiling",out,true);
  merge_all_new(folder,"He3pos",out,true);
  merge_all_new(folder,"He3ecc",out,true);
  merge_all_new(folder,"He3nocor",out,true);
  merge_all_new(folder,"He3rad",out,true);
  merge_all_new(folder,"D2",out);
  merge_all_new(folder,"D2lt",out,true);
  merge_all_new(folder,"D2boiling",out,true);
  merge_all_new(folder,"D2pos",out,true);
  merge_all_new(folder,"D2ecc",out,true);
  merge_all_new(folder,"D2nocor",out,true);
  merge_all_new(folder,"D2rad",out,true);
  merge_all_new(folder,"emc",out);
  merge_avg(folder,out);

  iso_input(folder);
  iso_apply(folder);
  //iso_new(folder,bins);
  //cout << "uhhh" << endl;
  KP_iso(folder);

  cout << "done" << endl;
}
