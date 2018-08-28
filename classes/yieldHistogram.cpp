//An alternative to using a TH1, should speed things up???

yieldHistogram::yieldHistogram(TString t, Int_t n, Double_t min, Double_t max){
  //Initialize descriptor variables
  title   = t;
  bin_min = min;
  bin_max = max;
  nbins   = n;
  
  //Initialize bin content structures
  bins.resize(nbins);
  for(Int_t i = 0; i < nbins; i++){
    bins[i].Q2sum   = 0;
    bins[i].xsum    = 0;
    bins[i].entries = 0;
    bins[i].scale   = 1;
  }

  //Initialize underflow bin
  underflow.Q2sum   = 0;
  underflow.xsum    = 0;
  underflow.entries = 0;
  underflow.scale   = 1;
  
  //Initialize overflow bin
  overflow.Q2sum   = 0;
  overflow.xsum    = 0;
  overflow.entries = 0;
  overflow.scale   = 1;
}

//If for some reason I want to make a quick piece of code and don't want to bother naming the Histogram
yieldHistogram::yieldHistogram(Int_t n, Double_t min, Double_t max){
  yieldHistogram("", n, min, max);
}

//Load from a file
yieldHistogram::yieldHistogram(TString file){
  ifstream f(file.Data();
  char t[256];
  f.getline(t,256);
  title = f;
  
  f >> nbins;
  f >> bin_min;
  f >> bin_max;

  f >> underflow.entries;
  f >> underflow.xsum;
  f >> underflow.Q2sum;
  f >> underflow.scale;

  f >> overflow.entries;
  f >> overflow.xsum;
  f >> overflow.Q2sum;
  f >> overflow.scale;

  for(Int_t i = 0; i < nbins; i++){
    f >> bins[i].entries;
    f >> bins[i].xsum;
    f >> bins[i].Q2sum;
    f >> bins[i].scale;
  }
}

//Scale all bins by scaling factor s
void yieldHistogram::scale(Double_t s){
  for(Int_t i = 0; i < nbins; i++){
    bins[i].scale *= s;
  }
}

//Scale a bin b by scaling factor s
void scaleHistogram::scaleBin(Int_t b, Double_t s){
  bins[b].scale *= s;
}

//Add a count with physics variables x and Q2
Int_t yieldHistogram::addCount(Double_t x, Double_t Q2){
  Int_t n = TMath::FloorNint((x - bin_min)/((bin_max - bin_min)/(nbins)));
  if(n < 0){
    underflow.entries++;
    underflow.xsum  += x;
    underflow.Q2sum += Q2;
    return -1;
  }else if(n > nbins){
    overflow.entries++;
    overflow.xsum  += x;
    overflow.Q2sum += Q2;
    return 1;
  }
  bins[n].entries++;
  bins[n].xsum  += x;
  bins[n].Q2sum += Q2;
  return 0;
}

//Put the data into a TH1D and return the pointer to it
TH1D* yieldHistogram::getTH1(){
  TH1D *histo = new TH1D("yieldHistogram",title.Data(),nbins,bin_min,bin_max);
  for(Int_t i = 0; i < nbins; i++){
    histo->UpdateBinContent(i + 1, bins[i].entries * bins[i].scale);
  }
  return histo;
}

//Save data to a text file
Int_t save(TString file){
  ofstream f(file.Data());
  if(f.is_open()){
    f << title << endl;
    f << nbins << endl;
    f << bin_min << endl;
    f << bin_max << endl;
    
    f << underflow.entries << " " << underflow.xsum << " " << underflow.Q2sum << " " << underflow.scale << endl;
    f << overflow.entries  << " " << overflow.xsum  << " " << overflow.Q2sum  << " " << overflow.scale  << endl;

    for(Int_t i = 0; i < nbins ; i++){
      f << bins[i].entries << " " << bins[i].xsum << " " << bins[i].Q2sum << " " << bins[i].scale << endl;
    }

    return 0;
  }
  return 1; //Something went wrong with the file
}
