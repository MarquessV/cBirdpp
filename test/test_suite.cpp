#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::Checklist;
using cbirdpp::Checklists;
using cbirdpp::DataOptionalParameters;
using cbirdpp::DataSortType;
using cbirdpp::DetailedObservations;
using cbirdpp::Observations;
using cbirdpp::RankType;
using cbirdpp::Requester;
using cbirdpp::SortType;
using cbirdpp::Top100;

#include <gtest/gtest.h>

#include <string>
#include <fstream>

#include <string>
using std::string;

#include <utility>
using std::pair;

#include <vector>
using std::vector;

std::ifstream fin("resources/apikey");
std::string APIKEY;

// One of each type of region code: Country code, locID, subnational1, subnational2
vector<string> region_codes = {"US", "L3938360", "US-CA"/*, "US-CA-075"*/};
vector<string> species_codes = {"calqua"};
vector<pair<double, double>> coords = { {37.881619, -121.914099}, {37.8, -121.9} };

// 2 variations: Both have all non-default, one uses max possible values in range, the other
// uses the minumum possible values.
vector<DataOptionalParameters> data_optional_params(2);

// Tests get_recent_observations_in_region with valid arguments
TEST(GetRecentObsInRegionTest, Valid)
{
  Requester requester(APIKEY);
  for(const string& region : region_codes) {
    Observations obs = requester.get_recent_observations_in_region(region);
  }
  for(const string& region : region_codes) {
    for(const DataOptionalParameters& p : data_optional_params) {
      Observations obs = requester.get_recent_observations_in_region(region, p);
    }
  }
}

TEST(GetRecentObsInRegionTest, Invalid)
{
  Requester requester(APIKEY);
  try {
    Observations obs = requester.get_recent_observations_in_region("GIBBBBBBERISH");
  } catch(const cbirdpp::RequestFailed& rf) {}
}

TEST(GetRecentNotableObsInRegionTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const string& region : region_codes) {
    Observations obs = requester.get_recent_notable_observations_in_region(region);
  }
  for(const string& region : region_codes) {
    for(const DataOptionalParameters& p : data_optional_params) {
      Observations obs = requester.get_recent_notable_observations_in_region(region, p);
    }
  }
}

TEST(GetDetailedRecentNotableObsInRegionTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const string& region : region_codes) {
    DetailedObservations obs = requester.get_detailed_recent_notable_observations_in_region(region);
  }
  for(const string& region : region_codes) {
    for(const DataOptionalParameters& p : data_optional_params) {
      DetailedObservations obs = requester.get_detailed_recent_notable_observations_in_region(region, p);
    }
  }
}

TEST(GetRecentObsOfSpeciesInRegionTest, SuccessTest)
{
  Requester requester(APIKEY);  
  for(const string& region : region_codes) {
    for(const string& species: species_codes) {
      Observations obs = requester.get_recent_observations_of_species_in_region(region, species);
    }
  }
  for(const string& region : region_codes) {
    for(const string& species : species_codes) {
      for(const DataOptionalParameters& p : data_optional_params) {
        Observations obs = requester.get_recent_observations_of_species_in_region(region, species, p);
      }
    }
  }
}

TEST(GetRecentNearbyObservationsTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const pair<double,double>& coord : coords) {
    Observations obs = requester.get_recent_nearby_observations(coord.first, coord.second);
  }
  for(const pair<double,double>& coord : coords) {
    for(const DataOptionalParameters& p : data_optional_params) {
      Observations obs = requester.get_recent_nearby_observations(coord.first, coord.second, p);
    }
  }
}

TEST(GetRecentNearbyNotableTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const pair<double,double>& coord : coords) {
    Observations obs = requester.get_recent_nearby_notable_observations(coord.first, coord.second);
  }
  for(const pair<double,double>& coord : coords) {
    for(const DataOptionalParameters& p : data_optional_params) {
      Observations obs = requester.get_recent_nearby_notable_observations(coord.first, coord.second, p);
    }
  }
}

TEST(GetDetailedRecentNearbyNotableTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const pair<double,double>& coord : coords) {
    DetailedObservations obs = requester.get_detailed_recent_nearby_notable_observations(coord.first, coord.second);
  }
  for(const pair<double,double>& coord : coords) {
    for(const DataOptionalParameters& p : data_optional_params) {
      DetailedObservations obs = requester.get_detailed_recent_nearby_notable_observations(coord.first, coord.second, p);
    }
  }
}

TEST(GetRecentNearbyObservationsOfSpeciesTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const pair<double,double>& coord : coords) {
    for(const string& species : species_codes) {
      Observations obs = requester.get_recent_nearby_observations_of_species(species, coord.first, coord.second);
    }
  }
  for(const pair<double,double>& coord : coords) {
    for(const string& species : species_codes) {
      for(const DataOptionalParameters& p : data_optional_params) {
        Observations obs = requester.get_recent_nearby_observations_of_species(species, coord.first, coord.second, p);
      }
    }
  }
}

TEST(GetNearestObservationOfSpeciesTest, SuccessTest)
{
  Requester requester(APIKEY);
  for(const pair<double,double>& coord : coords) {
    for(const string& species : species_codes) {
      Observations obs = requester.get_nearest_observations_of_species(species, coord.first, coord.second);
    }
  }
  for(const pair<double,double>& coord : coords) {
    for(const string& species : species_codes) {
      for(const DataOptionalParameters& p : data_optional_params) {
        Observations obs = requester.get_nearest_observations_of_species(species, coord.first, coord.second, p);
      }
    }
  }
}

TEST(GetHistoricObservationsOnDateTest, SuccessTest)
{
  Requester requester(APIKEY); 
  for(const string& region : region_codes) {
    Observations obs = requester.get_historic_observations_on_date(region, 2014, 2, 14);
  }
  for(const string& region : region_codes) {
    for(const DataOptionalParameters& p : data_optional_params) {
      Observations obs = requester.get_historic_observations_on_date(region, 2014, 2, 14, p);
    }
  }
}

TEST(GetDetailedHistoricObservationsOnDateTest, SuccessTest)
{
  Requester requester(APIKEY); 
  for(const string& region : region_codes) {
    DetailedObservations obs = requester.get_detailed_historic_observations_on_date(region, 2014, 2, 14);
  }
  for(const string& region : region_codes) {
    for(const DataOptionalParameters& p : data_optional_params) {
      DetailedObservations obs = requester.get_detailed_historic_observations_on_date(region, 2014, 2, 14, p);
    }
  }
}

TEST(GetTop100Test, SuccessTest)
{
  Requester requester(APIKEY); 
  for(const string& region : region_codes) {
    Top100 top = requester.get_top_100(region, 2018, 1, 1);
    top = requester.get_top_100(region, 2018, 1, 1, true);
    top = requester.get_top_100(region, 2018, 1, 1, true, 5);
    top = requester.get_top_100(region, 2018, 1, 1, false, 5);
  }
}

TEST(GetChecklistFeedOnDateTest, SuccessTest)
{
  Requester requester(APIKEY); 
  for(const string& region : region_codes) {
    Checklists checklist = requester.get_checklist_feed_on_date(region, 2018, 1, 1);
    checklist = requester.get_checklist_feed_on_date(region, 2018, 1, 1, SortType::creation_dt);
    checklist = requester.get_checklist_feed_on_date(region, 2018, 1, 1, SortType::creation_dt, 5);
    checklist = requester.get_checklist_feed_on_date(region, 2018, 1, 1, 5);
  }
}

TEST(GetRecentChecklistsFeed, SuccessTest)
{

  Requester requester(APIKEY); 
  for(const string& region : region_codes) {
    Checklists checklist = requester.get_recent_checklists_feed(region);
    checklist = requester.get_recent_checklists_feed(region, 15);
  }
}

int main(int argc, char **argv)
{
  if(!fin) {return -1;}
  fin >> APIKEY;
  fin.close();

  data_optional_params[0].set_back(30);
  data_optional_params[0].set_cat("domestic,form,intergrade,species");
  data_optional_params[0].set_maxResults(10000);
  data_optional_params[0].set_includeProvisional(true);
  data_optional_params[0].set_hotspot(true);
  data_optional_params[0].set_sort(DataSortType::species);
  data_optional_params[0].set_dist(50);
  data_optional_params[0].set_rank(RankType::create);

  data_optional_params[1].set_back(1);
  data_optional_params[1].set_cat("species");
  data_optional_params[1].set_maxResults(1);
  data_optional_params[1].set_includeProvisional(true);
  data_optional_params[1].set_hotspot(true);
  data_optional_params[1].set_sort(DataSortType::species);
  data_optional_params[1].set_dist(50);
  data_optional_params[1].set_rank(RankType::create);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
