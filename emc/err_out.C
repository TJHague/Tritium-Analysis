{
  TString folder = "p2_allerr";
  int bins = 33;
  double min = 0;
  double max = .99;

  for(int i = 0; i <= 5; i++){
    cout << i << endl;
    check_errors(i,folder,bins,min,max);
  }
  for(int i = 7; i <= 15; i+=2){
    cout << i << endl;
    check_errors(i,folder,bins,min,max,1);
    check_errors(i,folder,bins,min,max,2);
  }
  check_errors(15,folder,bins,min,max,3);
  cout << "16" << endl;
  check_errors(16,folder,bins,min,max,1);
  check_errors(16,folder,bins,min,max,2);
}
