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
    int temp;

    getline( coffeestream, rl ); // read one line  = event into string
    istringstream listentry(rl); // tokenize string

    // Limit to someone's ticks:
    listentry >> user;
    //if(user != "{4CF8B02A-DD0F-4B19-BC92-C3D9B23CCAB0}") continue; // Thomas
    if(user != "{225E0443-FCEF-4400-8D69-B1535A07B9DA}") continue; // Simon

    listentry >> temp; // year
    listentry >> temp; // month
    listentry >> temp; // day

    listentry >> timestamp;
    listentry >> ticks;

    coffee.push_back(std::make_pair(timestamp,ticks));
  }
  std::sort(coffee.begin(), coffee.end(), pairCompare);

  vector <double> cfrate;
  vector <double> time;
  vector <double> accum_coffee;

  int start_time_of_phd = coffee.front().first;

  for(int i = 0; i < coffee.size(); i++) {
    time.push_back((coffee.at(i).first - start_time_of_phd)/3600/24);
    if(i == 0) {
      accum_coffee.push_back(coffee.at(i).second);
      cfrate.push_back(0);
    } else {
      accum_coffee.push_back(accum_coffee.back()+coffee.at(i).second);
      cfrate.push_back(coffee.at(i).second/((coffee.at(i).first-coffee.at(i-1).first)/3600.0/24.0)*100.0);
    }
  }

  cout << "Found " << accum_coffee.back() << " coffees for this user." << endl;

  TCanvas *c1 = new TCanvas("c1","coffee",1800,600);
  TGraph *cff = new TGraph(cfrate.size(),&(time[0]),&(cfrate[0]));
  TGraph *allcff = new TGraph(accum_coffee.size(),&(time[0]),&(accum_coffee[0]));

  allcff->SetLineColor(2);
  allcff->SetMarkerColor(kBlue);
  allcff->SetMarkerStyle(22);
  allcff->SetMarkerSize(1);
  allcff->SetLineWidth(-1503);
  allcff->SetFillStyle(3005);
  allcff->SetFillColor(2);
  allcff->SetTitle("Caffeine Intoxication");
  allcff->Draw("APL");
  allcff->GetYaxis()->SetTitle("Coffee Consumption [cups]");
  allcff->GetXaxis()->SetTitle("PhD Duration [d]");

  cff->SetLineColor(kGray+1);
  cff->SetLineWidth(-802);
  cff->SetFillStyle(3004);
  cff->SetFillColor(kGray+1);
  cff->Draw("PL");

  c1->SetLogy();
  c1->SetTicky();
  c1->SetTickx();
  c1->SetTitle("Caffeine Intoxication");

  TLegend * leg = new TLegend(0.55,0.15,0.85,0.35);
  leg->AddEntry(allcff,"Accumulated Cups");
  leg->AddEntry(cff,"Daily Dosage #times 100");
  leg->SetLineColor(0);
  leg->Draw();
}

