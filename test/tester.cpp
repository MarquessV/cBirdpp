#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::DataOptionalParameters;
using cbirdpp::DetailType;
using cbirdpp::DetailedObservation;
using cbirdpp::DetailedObservations;
using cbirdpp::Observation;
using cbirdpp::Observations;
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
#include <variant>
using std::get;
#include <vector>
using std::vector;

ostream& operator<<(ostream& out, const Observation& o)
{
  out << "speciesCode: " << o.speciesCode << endl
      << "comName: " << o.comName << endl
      << "sciName: " << o.sciName << endl
      << "locId: " << o.locId << endl
      << "locName: " << o.locName << endl
      << "obsDt: " << o.obsDt << endl
      << "howMany: " << o.howMany << endl
      << "lat: " << o.lat << endl
      << "lng: " << o.lng << endl
      << std::boolalpha
      << "obsValid: " << o.obsValid << endl
      << "obsReviewed: " << o.obsReviewed << endl
      << "locationPrivate: " << o.locationPrivate << endl;
  return out;
}

ostream& operator<<(ostream& out, const DetailedObservation& o)
{
  const Observation* downcast = &o;
  out << *downcast
      << o.checklistId << endl
      << o.countryCode << endl 
      << o.countryName << endl
      << o.firstName << endl
      << o.hasComments << endl
      << o.hasRichMedia << endl
      << o.lastName << endl
      << o.locID << endl
      << o.obsId << endl
      << o.presenceNoted << endl
      << o.subnational1Code << endl
      << o.subnational1Name << endl
      << o.subnational2Code << endl
      << o.subnational2Name << endl
      << o.userDisplayName << endl;
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

  Observations result = requester.get_recent_nearby_observations(37.8, -121.9);

  /*
  for(auto r : result) {
    cout << r;
  }
  */

  cout << "TESTING OPTIONAL PARAMS" << endl;

  DataOptionalParameters params;

  params.set_back(30);
  params.set_cat("domestic,intergrade,slash,species");
  params.set_maxResults(1);
  params.set_includeProvisional(true);
  params.set_hotspot(true);
  params.set_detail(DetailType::full);
  params.set_sort(SortType::species);
  params.set_dist(50);
  params.set_rank(RankType::create);

  result = requester.get_recent_nearby_observations(37.887815, -121.910193, params);

  for(auto r : result) {
    cout << r;
  }

  return 0;
}
