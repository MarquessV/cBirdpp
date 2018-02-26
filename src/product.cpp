#include "../include/cbirdpp/cbirdpp.h"
using cbirdpp::Top100;
using cbirdpp::Checklist;
using cbirdpp::RegionalStats;

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
using std::to_string;


#include <sstream>
using std::ostringstream;
using std::stringstream;

const string PRODURL = "https://ebird.org/ws2.0/product/";

using nlohmann::json;

namespace cbirdpp
{

  void from_json(const json& source, Top100Base& target)
  {
    if(source.find("profileHandle") == source.end()) {
      target.profileHandle = "N/A"; 
    } else {
      target.profileHandle = source.at("profileHandle").get<string>();
    }
    target.userDisplayName = source.at("userDisplayName").get<string>();
    target.numSpecies = source.at("numSpecies").get<unsigned int>();
    target.numCompleteChecklists = source.at("numCompleteChecklists").get<unsigned int>();
    target.rowNum = source.at("rowNum").get<unsigned int>();
    target.userId = source.at("userId").get<string>();
  }

  void from_json(const json& source, Checklist& target)
  {
    target.locId = source.at("locId").get<string>();
    target.subID = source.at("subID").get<string>();
    target.userDisplayName = source.at("userDisplayName").get<string>();
    target.numSpecies = source.at("numSpecies").get<unsigned int>();
    target.obsDt = source.at("obsDt").get<string>();
    target.countryCode = source.at("countryCode").get<string>();
    target.countryName = source.at("countryName").get<string>();
    target.subnational1Name = source.at("subnational1Name").get<string>();
    target.subnational1Code = source.at("subnational1Code").get<string>();
    target.subnational2Name = source.at("subnational2Name").get<string>();
    target.subnational2Code = source.at("subnational2Code").get<string>();
    target.isHotspot = source.at("isHotspot").get<bool>();
    target.hierarchicalName = source.at("hierarchicalName").get<string>();
  }

  void from_json(const json& source, RegionalStats& target)
  {
    target.numChecklists = source.at("numChecklists").get<unsigned int>();
    target.numContributors = source.at("numContributors").get<unsigned int>();
    target.numSpecies = source.at("numSpecies").get<unsigned int>();
  }

  Top100 Requester::get_top_100(const string& regionCode, int year, int month, int day, bool checklistSort/*=false*/, unsigned int maxResults/*=100*/) const
  {
    string request_url = PRODURL + "top100/" + regionCode + "/" + generate_date(year, month, day);
    if(checklistSort || maxResults != 100) {
      request_url += "?";
      if(checklistSort) {
        request_url += "checklistSort=true";
        if(maxResults != 100) {
          request_url += "&maxResults=" + to_string(maxResults);
        }
      } else {
        request_url += "&maxResults=" + to_string(maxResults);
      }
    }

    json response = request_json(request_url);
    auto result = json_to_object<Top100, Top100Base>(response);
    return result;
  }

  Top100 Requester::get_top_100(const string& regionCode, int year, int month, int day, unsigned int maxResults) const
  {
    return get_top_100(regionCode, year, month, day, false, maxResults);
  }

}
