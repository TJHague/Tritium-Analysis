void check_runlist(TString runlist){

  TString list = gGet_InputFile_Var(runlist,2);

  vector<Int_t> runvec = gGet_RunNoChain(list);

  const TString mysql_connection = "mysql://halladb/triton-work";
  const TString mysql_user       = "triton-user";
  const TString mysql_password   = "3He3Hdata";

  for(int i=0; i<runvec.size(); i++){
    TSQLServer* Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
    TString     query    = Form("select * from MARATHONrunlist where run_number=", runnum);
    TSQLResult* result   = Server->Query(query.Data());
    Server->Close();// Always remember to CLOSE the connection!

    Int_t       nrows    = result->GetRowCount(); 
    if(nrows==0){
      cout<< "Error: run "<<runnum<<" does not exist in the runlist. Please add it.";
    }
  }
}
