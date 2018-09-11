//An alternative to using a TH1, should speed things up???

//#include "yieldHistogram.h"

yieldHistogram::yieldHistogram(TString t, Int_t n, Double_t min, Double_t max){
  //Initialize descriptor variables
  title   = t;
  bin_min = min;
  bin_max = max;
  nbins   = n;
  
  //Initialize bin content structures
  bin_info binfo;
  binfo.Q2sum   = 0;
  binfo.xsum    = 0;
  binfo.entries = 0;
  binfo.scale   = 1;
  binfo.scaled  = 0;
  for(Int_t i = 0; i < nbins; i++){
    bins.push_back(bin_info());
    bins[i]=binfo;
  }

  //Initialize underflow bin
  underflow.Q2sum   = 0;
  underflow.xsum    = 0;
  underflow.entries = 0;
  underflow.scale   = 1;
  underflow.scaled  = 0;
  
  //Initialize overflow bin
  overflow.Q2sum   = 0;
  overflow.xsum    = 0;
  overflow.entries = 0;
  overflow.scale   = 1;
  overflow.scaled  = 0;
}

//If for some reason I want to make a quick piece of code and don't want to bother naming the Histogram
yieldHistogram::yieldHistogram(Int_t n, Double_t min, Double_t max){
  yieldHistogram("", n, min, max);
}

//Load from a file
yieldHistogram::yieldHistogram(TString file){
  ifstream f(file.Data());
  char t[256];
  f.getline(t,256);
  title = t;
  
  f >> nbins;
  f >> bin_min;
  f >> bin_max;
  f >> charge;
  f >> livetime;
  f >> avgI;

  f >> underflow.entries;
  f >> underflow.scaled;
  f >> underflow.xsum;
  f >> underflow.Q2sum;
  f >> underflow.scale;

  f >> overflow.entries;
  f >> overflow.scaled;
  f >> overflow.xsum;
  f >> overflow.Q2sum;
  f >> overflow.scale;

  for(Int_t i = 0; i < nbins; i++){
    bins.push_back(bin_info());
    f >> bins[i].entries;
    f >> bins[i].scaled;
    f >> bins[i].xsum;
    f >> bins[i].Q2sum;
    f >> bins[i].scale;
  }

  f.close();
}

//Scale all bins by scaling factor s
void yieldHistogram::scale(Double_t s){
  for(Int_t i = 0; i < nbins; i++){
    bins[i].scale  *= s;
    bins[i].scaled *= s;
  }
}

//Scale a bin b by scaling factor s
void yieldHistogram::scaleBin(Int_t b, Double_t s){
  bins[b].scale  *= s;
  bins[b].scaled *= s;
}

//Add a count with physics variables x and Q2
Int_t yieldHistogram::addCount(Double_t x, Double_t Q2){
  Int_t n = TMath::FloorNint((x - bin_min)/((bin_max - bin_min)/(nbins)));
  if(n < 0){
    underflow.entries++;
    underflow.scaled += 1;
    underflow.xsum   += x;
    underflow.Q2sum  += Q2;
    return -1;
  }else if(n > nbins){
    overflow.entries++;
    overflow.scaled += 1;
    overflow.xsum   += x;
    overflow.Q2sum  += Q2;
    return 1;
  }
  bins[n].entries++;
  bins[n].scaled += 1;
  bins[n].xsum   += x;
  bins[n].Q2sum  += Q2;
  return 0;
}

//Returns a vector of the number average x for each bin
vector<Double_t> yieldHistogram::getAvgx(){
  vector<Double_t> avgx;
  for(Int_t i = 0; i < nbins; i++){
    if(bins[i].entries != 0){
      avgx.push_back(bins[i].xsum / (Double_t)bins[i].entries);
    }else{
      avgx.push_back(0);
    }
  }
  return avgx;
}

//Returns a vector of the number average Q2 for each bin
vector<Double_t> yieldHistogram::getAvgQ2(){
  vector<Double_t> avgQ2;
  for(Int_t i = 0; i < nbins; i++){
    if(bins[i].entries != 0){
      avgQ2.push_back(bins[i].Q2sum / (Double_t)bins[i].entries);
    }else{
      avgQ2.push_back(0);
    }
  }
  return avgQ2;
}

//Returns a vector of the scaled counts
vector <Double_t> yieldHistogram::getCounts(){
  vector<Double_t> counts;
  for(Int_t i = 0; i < nbins; i++){
    counts.push_back(bins[i].scaled);
  }
  return counts;
}

//Set Charge
void yieldHistogram::setCharge(Double_t Q){
  charge = Q;
}

//Get Charge
Double_t yieldHistogram::getCharge(){
  return charge;
}

void yieldHistogram::setLivetime(Double_t lt){
  livetime = lt;
}

Double_t yieldHistogram::getLivetime(){
  return livetime;
}

void yieldHistogram::setAvgI(Double_t i){
  avgI = i;
}

Double_t yieldHistogram::getAvgI(){
  return avgI;
}

//Put the data into a TH1D and return the pointer to it
TH1D* yieldHistogram::getTH1(TString name){
  TH1D *histo = new TH1D(name.Data(),title.Data(),nbins,bin_min,bin_max);
  histo->Sumw2();
  for(Int_t i = 0; i < nbins; i++){
    for(Int_t j = 0; j < bins[i].entries; j++){
//      histo->AddBinContent(i + 1);
      histo->Fill(bins[i].xsum/bins[i].entries);
    }
  }
  return histo;
}

//Save data to a text file
Int_t yieldHistogram::save(TString file){
  ofstream f(file.Data());
  f << setprecision(10);
  if(f.is_open()){
    f << title << endl;
    f << nbins << endl;
    f << bin_min << endl;
    f << bin_max << endl;
    f << charge  << endl;
    f << livetime << endl;
    f << avgI << endl;
    
    f << underflow.entries << " " << underflow.scaled << " " << underflow.xsum << " " << underflow.Q2sum << " " << underflow.scale << endl;
    f << overflow.entries  << " " << overflow.scaled  << " " << overflow.xsum  << " " << overflow.Q2sum  << " " << overflow.scale  << endl;

    for(Int_t i = 0; i < nbins ; i++){
      f << bins[i].entries << " " << bins[i].scaled   << " " << bins[i].xsum   << " " << bins[i].Q2sum   << " " << bins[i].scale << endl;
    }

    return 0;
  }
  f.close();
  return 1; //Something went wrong with the file
}

Int_t yieldHistogram::add(yieldHistogram *other){
  if((nbins == other->nbins) && (bin_min == other->bin_min) && (bin_max == other->bin_max)){
    charge += other->charge;
    for(Int_t i = 0; i < nbins; i++){
      if(bins[i].entries != 0){
        bins[i].scale = ((bins[i].entries * bins[i].scale) + (other->bins[i].entries * other->bins[i].scale)) / (bins[i].entries + other->bins[i].entries);
      }else{
        bins[i].scale = other->bins[i].scale;
      }
      bins[i].scaled  += other->bins[i].scaled;
      bins[i].xsum    += other->bins[i].xsum;
      bins[i].Q2sum   += other->bins[i].Q2sum;
      bins[i].entries += other->bins[i].entries;
    }
    return 0;
  }
  return -1;
}
