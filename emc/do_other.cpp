{
  TString folder = "p2_allerr";
  int bins = 33;
  double min = 0;
  double max = .99;

  /*for(int i = 0; i <= 5; i++){
    cout << i << endl;
    process_other(i,folder,bins,min,max);
    kinYield(Form("%s/kin%d/H3/",folder.Data(),i));
    if(kin<=4){
      kinYield(Form("%s/kin%d/H1/",folder.Data(),i));
    }
  }*/
  for(int i = 7; i <= 15; i+=2){
    cout << i << endl;
    process_other(i,folder,bins,min,max,1);
    process_other(i,folder,bins,min,max,2);
    kinYield(Form("%s/kin%d_1st/H3/",folder.Data(),i));
    kinYield(Form("%s/kin%d_2nd/H3/",folder.Data(),i));
  }
  process_other(15,folder,bins,min,max,3);
  kinYield(Form("%s/kin15_3rd/H3/",folder.Data()));
  cout << "16" << endl;
  process_otherR(16,folder,bins,min,max,1);
  process_otherR(16,folder,bins,min,max,2);
  kinYield(Form("%s/kin16_1st/H3/",folder.Data()));
  kinYield(Form("%s/kin16_2nd/H3/",folder.Data()));
  cout << "done" << endl;
}
