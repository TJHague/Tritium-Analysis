{
  TString folder = "x_03_test";
  int bins = 33;
  double min = 0;
  double max = .99;
  for(int i = 0; i <= 5; i++){
    cout << i << endl;
//    emc(i,folder,bins,min,max);
    kin_clean(i,folder);
  }
  for(int i = 7; i <= 15; i+=2){
    cout << i << endl;
//    emc(i,folder,bins,min,max,1);
//    emc(i,folder,bins,min,max,2);
    if(i != 15){
      kin_clean(i,folder);
    }
  }
  //emc(15,folder,bins,min,max,3);
  kin_clean(15,folder);
  cout << "16" << endl;
  //emcR(16,folder,bins,min,max,1);
  //emcR(16,folder,bins,min,max,2);
  kin_clean(16,folder);

  merge_all(folder);
  cout << "done" << endl;
}
