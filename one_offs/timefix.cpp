#include "../headers/rootalias.h"

string getTime(int run){
  TChain *T = LoadRun(run);

  THaRun *run_data = 0;
  TDatime date;
  string stamp = "";
  if(T){
    run_data = GetRunData(T);
  }
  if(run_data){
    date = run_data->GetDate();
    stamp += "--------[ ";
    stamp += to_string(date.GetYear());
    stamp += "-";
    stamp += to_string(date.GetMonth());
    stamp += "-";
    stamp += to_string(date.GetDay());
    stamp += " ";
    stamp += to_string(date.GetHour());
    stamp += ":";
    stamp += to_string(date.GetMinute());
    stamp += ":";
    stamp += to_string(date.GetSecond()-1);
    stamp += " ]\n";
  }
  delete run_data;
  delete T;
  return stamp;
}

void timefix(){
  ifstream old("db_run.dat");
  ofstream fix("new_db_run.dat");

  while(!old.eof()){
    string line = "";
    getline(old, line);
    if(line[0]=='-'){
      string next = "";
      getline(old, next);
      string run_num = next.substr(28,5);
      int run = stoi(run_num);
      string stamp = getTime(run);
      if(stamp[0]=='-'){
        fix << stamp;
      }else{
        fix << line << '\n';
      }
      fix << next << '\n';
    }else{
      fix << line << '\n';
    }
  }
  old.close();
  fix.close();
  exit(0);
}
