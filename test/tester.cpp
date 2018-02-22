#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::DataOptionalParameters;
using cbirdpp::DetailType;
using cbirdpp::Observation;
using cbirdpp::RankType;
using cbirdpp::Requester;
using cbirdpp::SortType;

#include "../include/nlohmann/json.hpp"
using nlohmann::json;

#include <gtest/gtest.h>

#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;

ostream& operator<<(ostream& out, Observation o)
{
  out << o.speciesCode << endl;
  out << o.comName << endl;
  out << o.sciName << endl;
  out << o.locId << endl;
  out << o.locName << endl;
  out << o.obsDt << endl;
  out << o.howMany << endl;
  out << o.lat << endl;
  out << o.lng << endl;
  out << std::boolalpha;
  out << o.obsValid << endl;
  out << o.obsReviewed << endl;
  out << o.locationPrivate << endl;
  return out;
}

int main()
{
  ifstream fin("resources/apikey"); 
  if(!fin) return -1;
  string apikey;
  fin >> apikey;
  fin.close(); 
  cout << apikey << endl;

  Requester requester(apikey);

  vector<Observation> result = requester.get_recent_observations_in_region("US-DE");

  for(auto r : result) {
    cout << r;
  }

  cout << "TESTING OPTIONAL PARAMS" << endl;

  DataOptionalParameters params;

  params.set_back(30);
  params.set_cat("domestic,intergrade,slash,species");
  params.set_maxResults(1);
  params.set_includeProvisional(true);
  params.set_hotspot(false);
  params.set_detail(DetailType::full);
  params.set_sort(SortType::species);
  params.set_dist(50);
  params.set_rank(RankType::create);

  result = requester.get_recent_observations_in_region("US-DE", params);

  for(auto r : result) {
    cout << r;
  }

  return 0;
}

/*  Google Test Main
int main()
{
  ::testing::InitGoogleTest(&argc, char **argv);
  return RUN_ALL_TESTS();
}
*/
