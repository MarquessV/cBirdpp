#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::Observation;
using cbirdpp::DetailedObservation;

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "../include/nlohmann/json.hpp"
using nlohmann::json;

#include <iostream> //NOLINT
using std::cout; //NOLINT
using std::endl; //NOLINT

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;
using std::stringstream;

#include <vector>
using std::vector;

const string OBSURL = "https://ebird.org/ws2.0/data/obs/";

using nlohmann::json;

namespace cbirdpp
{

  void from_json(const json& source, Observation& target)
  {
    target.speciesCode = source.at("speciesCode").get<string>();
    target.comName = source.at("comName").get<string>();
    target.sciName = source.at("sciName").get<string>();
    target.locId = source.at("locId").get<string>();
    target.locName = source.at("locName").get<string>();
    target.obsDt = source.at("obsDt").get<string>();
    if(source.find("howMany") == source.end()) {
      target.howMany = 0;   // Represents an 'x' input for the observation
    } else {
      target.howMany = source.at("howMany").get<unsigned int>();
    }
    target.lat = source.at("lat").get<double>();
    target.lng = source.at("lng").get<double>();
    target.obsValid = source.at("obsValid").get<bool>();
    target.obsReviewed = source.at("obsReviewed").get<bool>();
    target.locationPrivate = source.at("locationPrivate").get<bool>();
  }

  void from_json(const json& source, DetailedObservation& target)
  {
    Observation *downcast = &target;
    from_json(source, *downcast);
    target.checklistId = source.at("checklistId").get<string>();
    target.countryCode = source.at("countryCode").get<string>();
    target.countryName = source.at("countryName").get<string>();
    if(source.find("firstName") == source.end()) {
      target.firstName = "N/A";
    } else {
      target.firstName = source.at("firstName").get<string>();
    }
    target.hasComments = source.at("hasComments").get<bool>();
    target.hasRichMedia = source.at("hasRichMedia").get<bool>();
    if(source.find("firstName") == source.end()) {
      target.lastName = "N/A";
    } else {
      target.lastName = source.at("lastName").get<string>();
    }
    target.hasComments = source.at("hasComments").get<bool>();
    target.locID = source.at("locID").get<string>();
    target.obsId = source.at("obsId").get<string>();
    target.presenceNoted = source.at("presenceNoted").get<bool>();
    target.subId = source.at("subId").get<string>();
    target.subnational1Code = source.at("subnational1Code").get<string>();
    target.subnational1Name = source.at("subnational1Name").get<string>();
    target.subnational2Code = source.at("subnational2Code").get<string>();
    target.subnational2Name = source.at("subnational2Name").get<string>();
    if(source.find("userDisplayName") == source.end()) {
      target.userDisplayName = "N/A";
    } else {
      target.userDisplayName = source.at("userDisplayName").get<string>();
    }
  }

  Observations Requester::get_recent_observations_in_region(const string& regionCode, const DataOptionalParameters& params/*=defaults*/) const
  {
    vector<string> args = process_args({DataParams::back, DataParams::cat, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot}, params);
    string request_url = OBSURL + regionCode + "/recent" + generate_argument_string(args);
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  json Requester::get_recent_notable_setup(const string& regionCode, const DataOptionalParameters& params, bool detailed/*=false*/) const
  {
    vector<string> args = process_args({DataParams::back, DataParams::maxResults, DataParams::hotspot}, params, detailed);
    string request_url = OBSURL + regionCode + "/recent/notable" + generate_argument_string(args);
    return request_json(request_url);
  }

  Observations Requester::get_recent_notable_observations_in_region(const string& regionCode, const DataOptionalParameters& params) const
  {
    json response = get_recent_notable_setup(regionCode, params);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }
  
  DetailedObservations Requester::get_detailed_recent_notable_observations_in_region(const string& regionCode, const DataOptionalParameters& params/*=defaults*/) const
  {
    json response = get_recent_notable_setup(regionCode, params, true);
    auto observs = json_to_object<DetailedObservations, DetailedObservation>(response);
    return observs;
  }

  Observations Requester::get_recent_observations_of_species_in_region(const std::string& regionCode, const std::string& speciesCode, const DataOptionalParameters& params/*defaults*/) const
  {
    vector<string> args = process_args({DataParams::back, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot}, params);
    string request_url = OBSURL + regionCode + "/recent/" + speciesCode + generate_argument_string(args);
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  Observations Requester::get_recent_nearby_observations(const double lat, const double lng, const DataOptionalParameters& params) const
  {
    vector<string> args = process_args({DataParams::dist, DataParams::back, DataParams::cat, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot, DataParams::sort}, params, lat, lng);
    string request_url = OBSURL + "geo/recent" + generate_argument_string(args);
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  json Requester::get_recent_nearby_notable_setup(const double lat, const double lng, const DataOptionalParameters& params, bool detailed/*=false*/) const
  {
    vector<string> args = process_args({DataParams::dist, DataParams::back, DataParams::maxResults, DataParams::hotspot}, params, lat, lng, detailed);
    string request_url = OBSURL + "geo/recent/notable" + generate_argument_string(args);
    return request_json(request_url);
  }

  Observations Requester::get_recent_nearby_notable_observations(const double lat, const double lng, const DataOptionalParameters& params/*=defaults*/) const
  {
    json response = get_recent_nearby_notable_setup(lat, lng, params);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  DetailedObservations Requester::get_detailed_recent_nearby_notable_observations(const double lat, const double lng, const DataOptionalParameters& params/*=defaults*/) const
  {
    json response = get_recent_nearby_notable_setup(lat, lng, params, true);
    auto observs = json_to_object<DetailedObservations, DetailedObservation>(response);
    return observs;
  }

  Observations Requester::get_recent_nearby_observations_of_species(const string& speciesCode, double lat, double lng, const DataOptionalParameters& params/*=defaults*/) const
  {
    vector<string> args = process_args({DataParams::dist, DataParams::back, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot}, params, lat, lng);
    string request_url = OBSURL + "geo/recent/" + speciesCode + generate_argument_string(args);
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  Observations Requester::get_nearest_observations_of_species(const string& speciesCode, const double lat, const double lng, const DataOptionalParameters& params/*=defaults*/) const
  {
    vector<string> args = process_args({DataParams::dist, DataParams::back, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot}, params, lat, lng);
    string request_url = OBSURL + "geo/recent/" + speciesCode + generate_argument_string(args);
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }


  json Requester::get_historic_observations_on_date_setup(const string& regionCode, int year, int month, int day, const DataOptionalParameters& params, bool detailed) const
  {
    vector<string> args = process_args({DataParams::rank, DataParams::cat, DataParams::maxResults, DataParams::includeProvisional, DataParams::hotspot}, params, detailed);
    string request_url = OBSURL + regionCode + "/historic/" + generate_date(year, month, day) + generate_argument_string(args);
    return request_json(request_url);
  }

  Observations Requester::get_historic_observations_on_date(const string& regionCode, int year, int month, int day, const DataOptionalParameters& params/*=defaults*/) const
  {
    json response = get_historic_observations_on_date_setup(regionCode, year, month, day, params);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  DetailedObservations Requester::get_detailed_historic_observations_on_date(const string& regionCode, int year, int month, int day, const DataOptionalParameters& params/*=defaults*/) const
  {
    json response = get_historic_observations_on_date_setup(regionCode, year, month, day, params, true);
    auto observs = json_to_object<DetailedObservations, DetailedObservation>(response);
    return observs;
  }

}
