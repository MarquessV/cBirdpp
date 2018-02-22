#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::Requester;
using cbirdpp::Observation;
//using cbirdpp::ObservationsInRegion;
//using cbirdpp::NotableObservationsInRegion;

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

  return 0;
}

/*  Google Test Main
int main()
{
  ::testing::InitGoogleTest(&argc, char **argv);
  return RUN_ALL_TESTS();
}
*/
