/*
 *Main file for EMC analysis of tritium data. This file will do the He3/D as
 *that is my calculation for my thesis.
 *
 * Tyler Hague - August 2018
 */

#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"

void emc(){
  //Load runs 
  TChain* R_t = (TChain*) gGetTree(gGet_RunNoChain(R_list),"T");
  vector<Int_t> runvec = gGet_RunNoChain(list);
}
