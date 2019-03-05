{
  TString folder = "p2newz";
  int bins = 33;
  double min = 0;
  double max = 0.99;
  for(int i = 0; i < 15; i++){
    if(i<=5){
      hydro_ana(i,folder,bins,min,max);
    }else if(i%2!=0){
      hydro_ana(i,folder,bins,min,max,1);
      hydro_ana(i,folder,bins,min,max,2);
    }
  }
  hydro_ana(15,folder,bins,min,max,1);
  hydro_ana(15,folder,bins,min,max,2);
  hydro_ana(15,folder,bins,min,max,3);
  hydro_anaR(16,folder,bins,min,max,1);
  hydro_anaR(16,folder,bins,min,max,2);
}
