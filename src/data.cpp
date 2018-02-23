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
    target.firstName = source.at("firstName").get<string>();
    target.hasComments = source.at("hasComments").get<bool>();
    target.hasRichMedia = source.at("hasRichMedia").get<bool>();
    target.lastName = source.at("lastName").get<string>();
    target.locID = source.at("locID").get<string>();
    target.obsId = source.at("obsId").get<string>();
    target.presenceNoted = source.at("presenceNoted").get<bool>();
    target.subId = source.at("subId").get<string>();
    target.subnational1Code = source.at("subnational1Code").get<string>();
    target.subnational1Name = source.at("subnational1Name").get<string>();
    target.subnational2Code = source.at("subnational2Code").get<string>();
    target.subnational2Name = source.at("subnational2Name").get<string>();
    target.userDisplayName = source.at("userDisplayName").get<string>();
  }
  Requester::Requester(const string& key)
  {
    api_key = key;
  }

  Observations Requester::get_recent_observations_in_region(const string& regionCode, const DataOptionalParameters& params) const
  {
    // Process optional arguments
    vector<string> args;
    if(params.back()) {args.emplace_back(params.format_back());}
    if(params.cat()) {args.emplace_back(params.format_cat());}
    if(params.maxResults()) {args.emplace_back(params.format_maxResults());}
    if(params.includeProvisional()) {args.emplace_back(params.format_includeProvisional());}
    if(params.hotspot()) {args.emplace_back(params.format_hotspot());}
    
    // Append optional arguments to request url
    string request_url = OBSURL + regionCode + "/recent" + generate_argument_string(args);
    
    // Get the response
    json response = request_json(request_url);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }

  json Requester::get_recent_notable_setup(const string& regionCode, const DataOptionalParameters& params/*=defaults*/, bool detailed/*=false*/) const
  {
    vector<string> args;
    if(params.back()) {args.emplace_back(params.format_back());}
    if(params.maxResults()) {args.emplace_back(params.format_maxResults());}
    if(detailed) {args.emplace_back("detail=full");}  // Don't trust the user to request detailed without setting the parameter.
    if(params.hotspot()) {args.emplace_back(params.format_hotspot());}

    string request_url = OBSURL + regionCode + "/recent/notable" + generate_argument_string(args);
    return request_json(request_url);
  }

  Observations Requester::get_recent_notable_observations_in_region(const string& regionCode, const DataOptionalParameters& params) const
  {
    json response = get_recent_notable_setup(regionCode, params);
    auto observs = json_to_object<Observations, Observation>(response);
    return observs;
  }
  
  DetailedObservations Requester::get_detailed_recent_notable_observations_in_region(const string& regionCode, const DataOptionalParameters& params) const
  {
    json response = get_recent_notable_setup(regionCode, params, true);
    auto observs = json_to_object<DetailedObservations, DetailedObservation>(response);
    return observs;
  }

}
