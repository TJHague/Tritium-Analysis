void check_qlt(const char* dir){
  char* fulldir = gSystem->ExpandPathName(dir);
  void* dirp = gSystem->OpenDirectory(fulldir);

  const char* entry;
  TString file;

  while((entry = (char*)gSystem->GetDirEntry(dirp))){
    file = entry;
    if(file.EndsWith(".dat")&&file!="full.dat"){
      file = gSystem->ConcatFileName(fulldir,entry);
      yieldHistogram* yhdat = new yieldHistogram(file);
      if(yhdat->getCharge()<10000. || yhdat->getLivetime()<0.9 || yhdat->getAvgI()<5. || yhdat->getLivetime()>1.0){
        cout << file << endl << "Charge: " << yhdat->getCharge() << endl << "Livetime: " << yhdat->getLivetime() << endl << "Average Current: " << yhdat->getAvgI() << endl << endl;
      }

      delete yhdat;
    }
  }
}
