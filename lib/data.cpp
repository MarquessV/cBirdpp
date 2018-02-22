#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::Observation;

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "../include/nlohmann/json.hpp"
using nlohmann::json;

#include <iostream>
using std::cout;
using std::endl;

#include <list>
using std::list;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <vector>
using std::vector;

const std::string OBSURL = "https://ebird.org/ws2.0/data/obs/";

using nlohmann::json;

namespace cbirdpp
{
  void from_json(const json& source, Observation& target)
  {
    target.speciesCode = source.at("speciesCode").get<std::string>();
    target.comName = source.at("comName").get<std::string>();
    target.sciName = source.at("sciName").get<std::string>();
    target.locId = source.at("locId").get<std::string>();
    target.locName = source.at("locName").get<std::string>();
    target.obsDt = source.at("obsDt").get<std::string>();
    if(source.find("howMany") == source.end()) {
      target.howMany = 0;
    } else {
      target.howMany = source.at("howMany").get<unsigned int>();
    }
    target.lat = source.at("lat").get<double>();
    target.lng = source.at("lng").get<double>();
    target.obsValid = source.at("obsValid").get<bool>();
    target.obsReviewed = source.at("obsReviewed").get<bool>();
    target.locationPrivate = source.at("locationPrivate").get<bool>();
  }


  Requester::Requester(const std::string& key)
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
    if(params.back()) args.push_back(params.format_back());
    if(params.cat()) args.push_back(params.format_cat());
    if(params.maxResults()) args.push_back(params.format_maxResults());
    if(params.includeProvisional()) args.push_back(params.format_includeProvisional());
    if(params.hotspot()) args.push_back(params.format_hotspot());
    
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
    
    //std::cout << request_url << std::endl;

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

    for(auto x : response) {
      results.push_back(x.get<Observation>());
    }

    return results;
  }
}
