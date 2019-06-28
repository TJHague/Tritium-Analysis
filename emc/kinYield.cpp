void kinYield(const char* dir){
  char* fulldir = gSystem->ExpandPathName(dir);
  void* dirp = gSystem->OpenDirectory(fulldir);

  const char* entry;
  TString file;

  yieldHistogram *full;
  bool first = true;
  while((entry = (char*)gSystem->GetDirEntry(dirp))){
    file = entry;
    if(file.EndsWith(".dat")&&file!="full.dat"){
      file = gSystem->ConcatFileName(fulldir,entry);
      if(first){
        first = false;
        full = new yieldHistogram(file);
      }else{
        yieldHistogram *tmp = new yieldHistogram(file);
        full->add(tmp);
        delete tmp;
      }
    }
  }
  TString out = dir;
  out += "full.dat";
  if(full){
    full->save(out);
    delete full;
  }
}
