#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TLegend.h"

using namespace std;

bool pairCompare(const std::pair<int, int>& firstElem, const std::pair<int, int>& secondElem) {
  return firstElem.first < secondElem.first;
}

void plot() {

  cout << "try to open coffee file";
  ifstream coffeestream( "CMSCoffeeTicks.txt" );
  if( !coffeestream ) {
    cout << ": failed" << endl;
    return;
  }
  cout << ": succeed" << endl;

  vector <std::pair<int,int> > coffee;

  while( coffeestream.good() && ! coffeestream.eof() ) {

    // Read file by lines:
    string rl;

    string user;
    int timestamp;
    int ticks;

    getline( coffeestream, rl ); // read one line  = event into string
    istringstream listentry(rl); // tokenize string

    // Limit to Thomas' ticks:
    listentry >> user;
    //if(user != "{4CF8B02A-DD0F-4B19-BC92-C3D9B23CCAB0}") continue; // Thomas
    if(user != "{225E0443-FCEF-4400-8D69-B1535A07B9DA}") continue; // Simon
    
    listentry >> timestamp;
    listentry >> ticks;

    coffee.push_back(std::make_pair(timestamp,ticks));
  }
  std::sort(coffee.begin(), coffee.end(), pairCompare);

  vector <double> cffee;
  vector <double> time;
  vector <double> accum_coffee;

  int start_time_of_phd = coffee.front().first;
  
  for(int i = 0; i < coffee.size(); i++) {
    time.push_back((coffee.at(i).first - start_time_of_phd)/3600/24);
    cffee.push_back(coffee.at(i).second*10);
    if(i == 0) accum_coffee.push_back(coffee.at(i).second);
    else accum_coffee.push_back(accum_coffee.back()+coffee.at(i).second);
  }
  
  cout << "Found " << accum_coffee.back() << " coffees for this user." << endl;

  TCanvas *c1 = new TCanvas("c1","coffee",1800,600);
  TGraph *cff = new TGraph(cffee.size(),&(time[0]),&(cffee[0]));
  TGraph *allcff = new TGraph(accum_coffee.size(),&(time[0]),&(accum_coffee[0]));

  allcff->SetLineColor(2);
  allcff->SetMarkerColor(kBlue);
  allcff->SetMarkerStyle(22);
  allcff->SetMarkerSize(1);
  allcff->SetLineWidth(-1503);
  allcff->SetFillStyle(3005);
  allcff->SetFillColor(2);
  allcff->SetTitle("");
  allcff->Draw("APL");

  allcff->GetYaxis()->SetTitle("consume [coffee]");
  allcff->GetXaxis()->SetTitle("PhD [hours]");
  
  cff->SetLineColor(kGray+1);
  cff->SetLineWidth(-802);
  cff->SetFillStyle(3004);
  cff->SetFillColor(kGray+1);
  cff->Draw("PL");


  allcff->Draw("PL");
  
  c1->SetLogy();
  c1->SetTicky();
  c1->SetTickx();

  TLegend * leg = new TLegend(0.55,0.15,0.85,0.35);
  leg->AddEntry(allcff,"Accumulated Cups");
  leg->AddEntry(cff,"Cups per unit time #times 10");
  leg->SetLineColor(0);
  leg->Draw();
}

