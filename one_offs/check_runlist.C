#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"

void check_runlist(TString runlist){

  TString list_tar = gGet_InputFile_Var(runlist,0);
  TString list = gGet_InputFile_Var(runlist,2);
  
  TString target;
  if(list_tar=="H3"){
    target="Tritium";
  }else if(list_tar=="He3"){
    target="Helium-3";
  }else if(list_tar=="EM"){
    target="Empty Cell";
  }else if(list_tar=="D2"){
    target="Deuterium";
  }else if(list_tar=="H1"){
    target="Hydrogen";
  }else if(list_tar=="Carbon"){
    target="Carbon";
  }else if(list_tar=="Carbon_Hole"){
    target="Carbon Hole";
  }else if(list_tar=="DM"){
    target="25 cm Dummy";
  }else if(list_tar=="Optics"){
    target="Optics";
  }else if(list_tar=="Raster_Target"){
    target="Raster Target";
  }else if(list_tar=="Titanium"){
    target="Titanium";
  }else if(list_tar=="HOME"){
    target="Home";
  }else{
    target="blank";
  }

  vector<Int_t> runvec = gGet_RunNoChain(list);

  const TString mysql_connection = "mysql://halladb/triton-work";
  const TString mysql_user       = "triton-user";
  const TString mysql_password   = "3He3Hdata";

  for(int i=0; i<runvec.size(); i++){
    TSQLServer* Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
    TString     query    = Form("select * from MARATHONrunlist where run_number=%d", runvec[i]);
    TSQLResult* result   = Server->Query(query.Data());
    Server->Close();// Always remember to CLOSE the connection!

    Int_t       nrows    = result->GetRowCount(); 
    if(nrows==0){
      cout<< "Error: run "<< runvec[i] <<" does not exist in the runlist. Please add it." << endl;
      if(target!="blank"){
        gSystem->Exec(Form("python ../headers/log2db.py %d %s", runvec[i], target.Data()));
      }
    }
  }
  exit(0);
}
