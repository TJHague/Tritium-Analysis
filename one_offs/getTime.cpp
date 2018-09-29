#include "../headers/rootalias.h"

void getTime(int run){
  TChain *T = LoadRun(run);

  THaRun *run = 0;
  TDatime *date = 0;
  if(T){
    run = GetRunData(T);
  }
  if(run){
    date = run->GetDate();
  }
  if(date){
    cout << "--------[ " << date->GetYear() << "-" << date->GetMonth() << "-" << date->GetDay() << " " << date->GetHour() << ":" << date->GetMinute() << ":" << date->GetSecond()-1 << " ]" << endl;
    exit(0);
  }
  cout << "0" << endl;
  exit(0);

}
