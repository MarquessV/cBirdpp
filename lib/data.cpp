#include "../include/cbirdpp/cbirdpp.h"

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
    cURLpp::Cleanup cleaner;
    cURLpp::Easy request_handle;

    const std::string request_url = OBSURL;

    std::cout << request_url << std::endl;
    list<string> header = {"X-eBirdApiToken: " + api_key};
    request_handle.setOpt(cURLpp::Options::Url(request_url));
    request_handle.setOpt(cURLpp::Options::Header(true));
    request_handle.setOpt(cURLpp::Options::HttpHeader(header));
    std::ostringstream os("");
    cURLpp::Options::WriteStream ws(&os);
    request_handle.setOpt(ws);
    request_handle.perform();

    std::cout << os.str() << std::endl;

    nlohmann::json response = json::parse(os.str().substr(os.str().find('[')));

    std::vector<cbirdpp::Observation> results;

    for(auto x : response) {
      std::cout << x << std::endl << std::endl;
      results.push_back(x.get<Observation>());
    }

    return results;
  }

}
