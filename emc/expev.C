{
  TString folder = "p2_ev";
  TString targs[4] = {"He3","D2","H3","H1"}; 

  for(int i=0; i<4; i++){
    if(i!=0){for(int j=0; j<=5; j++){
      ev_scan(j,targs[i],folder);
    }
    for(int j=7; j<=15; j++){
      ev_scan(j,targs[i],folder,1);
      ev_scan(j,targs[i],folder,2);
    }
    ev_scan(15,targs[i],folder,3);}
    ev_scan(16,targs[i],folder,1);
    ev_scan(16,targs[i],folder,2); 
  }
}
