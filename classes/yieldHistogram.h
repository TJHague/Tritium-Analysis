struct bin_info{
  Int_t    entries; //So that new entries can be added after scaling and still give an accurate Q2 and x average
  Double_t scaled;
  Double_t xsum;
  Double_t Q2sum;
  Double_t scale;   //Scale new entries as they are added
};

class yieldHistogram{
  vector<bin_info> bins;
  Double_t bin_min, bin_max, charge, livetime, avgI;
  Int_t nbins;
  bin_info underflow, overflow;
  TString title;

  public:
  yieldHistogram(TString, Int_t, Double_t, Double_t);
  yieldHistogram(Int_t, Double_t, Double_t);
  yieldHistogram(TString);
  void scale(Double_t);
  void scaleBin(Int_t, Double_t);
  Int_t addCount(Double_t, Double_t);
  vector<Double_t> getAvgx();
  vector<Double_t> getAvgQ2();
  vector<Double_t> getCounts();
  void setCharge(Double_t);
  Double_t getCharge();
  void setLivetime(Double_t);
  Double_t getLivetime();
  void setAvgI(Double_t);
  Double_t getAvgI();
  TH1D* getTH1(TString);
  Int_t save(TString);
  Int_t add(yieldHistogram*);
};

