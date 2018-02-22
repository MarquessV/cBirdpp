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

#include <list>
using std::list;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <variant>
using std::variant;
using std::get;

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

  vector<Observation> Requester::get_recent_observations_in_region(const string& regionCode, const DataOptionalParameters& params)
  {
    // Setup cURLpp handler
    cURLpp::Cleanup cleaner;
    cURLpp::Easy request_handle;

    // Create request url
    string request_url = OBSURL + regionCode + "/recent";

    // Process optional arguments
    vector<string> args;
    if(params.back()) {args.push_back(params.format_back());}
    if(params.cat()) {args.push_back(params.format_cat());}
    if(params.maxResults()) {args.push_back(params.format_maxResults());}
    if(params.includeProvisional()) {args.push_back(params.format_includeProvisional());}
    if(params.hotspot()) {args.push_back(params.format_hotspot());}
    
    // Append optional arguments to request url
    if(!args.empty()) {
      request_url += "?";
      for(const string& arg : args) {
        request_url += arg;
        if(arg != args.back()) {
          request_url += "&";
        }
      }
    }
    
    // Set cURLpp options
    request_handle.setOpt(cURLpp::Options::Url(request_url));
    request_handle.setOpt(cURLpp::Options::Header(true));
    request_handle.setOpt(cURLpp::Options::HttpHeader(list<string>({"X-eBirdApiToken: " + api_key})));
    // Create output stream for the results and perform the request
    ostringstream os("");
    cURLpp::Options::WriteStream ws(&os);
    request_handle.setOpt(ws);
    request_handle.perform();

    // Parse the response
    json response = json::parse(os.str().substr(os.str().find('[')));

    // Create a vector of the results and return it.
    vector<Observation> results;

    for(const auto& x : response) {
      results.push_back(x.get<Observation>());
    }

    return results;
  }

  DetailedObservations Requester::get_recent_notable_observations_in_region(const string& regionCode, const DataOptionalParameters& params)
  {
    // Setup cURLpp handler
    cURLpp::Cleanup cleaner;
    cURLpp::Easy request_handle;

    // Create request url
    string request_url = OBSURL + regionCode + "/recent/notable";

    // Process optional arguments
    vector<string> args;
    bool detailed = false;
    if(params.back()) {args.push_back(params.format_back());}
    if(params.maxResults()) {args.push_back(params.format_maxResults());}
    if(params.detail()) {
      args.push_back(params.format_detail());
      detailed = true;
    }
    if(params.hotspot()) {args.push_back(params.format_hotspot());}
    
    // Append optional arguments to request url
    if(!args.empty()) {
      request_url += "?";
      for(const string& arg : args) {
        request_url += arg;
        if(arg != args.back()) {
          request_url += "&";
        }
      }
    }
    
    cout << request_url << endl;

    // Set cURLpp options
    request_handle.setOpt(cURLpp::Options::Url(request_url));
    request_handle.setOpt(cURLpp::Options::Header(true));
    request_handle.setOpt(cURLpp::Options::HttpHeader(list<string>({"X-eBirdApiToken: " + api_key})));
    // Create output stream for the results and perform the request
    ostringstream os("");
    cURLpp::Options::WriteStream ws(&os);
    request_handle.setOpt(ws);
    request_handle.perform();

    // Parse the response
    json response = json::parse(os.str().substr(os.str().find('[')));

    //cout << response.dump(4) << endl;

    DetailedObservations results;
    if(detailed) {
      for(const auto& entry : response) {
        results.push_back(entry.get<DetailedObservation>()); 
      }
    } else {
      vector<Observation> results;
      for(const auto& x : response) {
        results.push_back(x.get<Observation>());
      }
    }
    return results;
  }

}
