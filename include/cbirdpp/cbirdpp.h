#ifndef CBIRDPP_CBIRDPP_H
#define CBIRDPP_CBIRDPP_H

#include "DataOptionalParameters.h"
#include "Observation.h"

#include "../nlohmann/json.hpp"

#include <string>
#include <vector>

namespace cbirdpp
{

  extern DataOptionalParameters DATA_DEFAULT_PARAMS;

  /*
   * The primary interface for making requests to the eBird 2.0 API.
   * Requires an API key to be provided during construction. 
   */
  class Requester
  {
    private:
      std::string api_key;

      /*
       * Generates the list of arguments in the format required for the request URL
       * @param args a vector of arguments to be added to the request
       */
      std::string generate_argument_string(const std::vector<std::string>& args) const;

      /*
       * Generates the arguments for "nearby" API requests which requires both latitude and longitude
       * @param double lat, the latitude
       * @param double lng, the longitude
       */
       std::vector<std::string> generate_nearby_arguments(double lat, double lng) const;

      /*
       * Takes a request URL and attempts to make the request
       * @param request_url the url of the request to be made
       */
      nlohmann::json request_json(const std::string& request_url) const;

      /*
       * Takes some source JSON and converts it to a container of the given base type.
       * Assumes from_json(const json& source, T& target) has been defined somewhere
       * in the cbirdpp namespace.
       * @param source the json to be converted
       */ 
      template <typename Container, typename Base>
      Container json_to_object(const nlohmann::json& source) const
      {
        Container result;
        for(const auto& entry : source) {
          result.push_back(entry.get<Base>());   
        }
        return result;
      }

      /*
       * Performs common setup between get recent notable simple and detailed requests
       */
      nlohmann::json get_recent_notable_setup(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS, bool detailed=false) const;

      /*
       * Performs common setup between get recent nearby notable simple and detailed requests
       */
      nlohmann::json get_recent_nearby_notable_setup(double lat, double lng, const DataOptionalParameters& params, bool detailed=false) const;


    public:
      /*
       * The only available constructor, takes an api key as a string.
       * @param key the api key the requester will use to formulate requests
       */
      Requester(const std::string& key);

      /*
       * Performs the "Recent observations in a region" API request.
       * The only required parameter is a string, the region code as either a eBird locId or subnational2 code.
       * @param regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       * @param back the number of days back to look for observations. Range: [1-30]. Default: 14.
       * @param cat the taxonomic category or categories to limit the results to. Multiple categories may be comma-separated. 
       *    Range: [domestic|form|hybrid|intergrade|issf|slash|species|spuh]. Default: all
       * @param maxResults the maximum number of results to return. Range: [1-10000]. Default: 0 (all)
       * @param includeProvisional set to true if you want flagged records that have not been reviewed yet. Default: false.
       * @param hotspot set to true if results should be limited to sightings at birding hotspots. Default: false.
       * @return vector of the observations received from the request.
       */
      Observations get_recent_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

      /*
       * Performs the "Recent notable observations in a region" API request.
       * The only required parameter is a string, the region code as either a eBird locId or subnational2 code.
       * @param regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       * @param params a DataOptionalParameters object with the desired optional parameters set.
       * For reference, these are the optional parameters and their ranges:
       *  regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       *  back the number of days back to look for observations. Range: [1-30]. Default: 14.
       *  maxResults the maximum number of results to return. Range: [1-10000]. Default: 0 (all)
       *  detail (ignored, simple by default)
       *  hotspot set to true if results should be limited to sightings at birding hotspots. Default: false.
       * @return Observations a container of the observations received from the request.
       */
      Observations get_recent_notable_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

      /*
       * Performs the "Recent notable observations in a region" API request with the detailed format.
       * The only required parameter is a string, the region code as either a eBird locId or subnational2 code.
       * @param regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       * @param params DataOptionalParameters object with the desired optional parameters set.
       *  regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       *  back the number of days back to look for observations. Range: [1-30]. Default: 14.
       *  param maxResults the maximum number of results to return. Range: [1-10000]. Default: 0 (all)
       *  detail (ignored, detailed by default)
       *  hotspot set to true if results should be limited to sightings at birding hotspots. Default: false.
       * @return DetailedObservations a container of the observations received from the request.
       */
      DetailedObservations get_detailed_recent_notable_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

      /*
       * Performs the "Recent observations of a species in a region" API request.
       * There are two required parameters. The regionCode as a string, and the speciesCode as a string.
       * @param regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       * @param speciesCode any species code in the current eBird taxonomy.
       * @param params a DataOptionalParameters object with the desired optional parameters set.
       *  The optional parameters for this request are: back, maxResults, includeProvisional, hotspot
       *  @return Observations a container of the observations received from the request.
       */
      Observations get_recent_observations_of_species_in_region(const std::string& regionCode, const std::string& speciesCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;
       
      /*
       * Performs the "Recent nearby observations" API request.
       * There are two required parameters. The latitude and longitude as doubles. This method simply converts the doubles to strings
       * and calls the string argument version of this function.
       * @param lat double, the latitude of the target area, [-90.0, 90.0], will be extended to two digits of precision.
       * @param lng double, the longitude of the target area, [-180.0, 180.0], will be extended to two digits of precision.
       * @param params a DataOptionalParameters object with the desired optional parameter set.
       *  The optional parameters for this request are: dist, back, cat, maxResults, includeProvisional, hotspot, sort
       * @return Observations a container of the observations received from the request.
       */
      Observations get_recent_nearby_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

            /*
       * Performs the "Recent nearby notable observations" API request.
       * There are two requred parameters. The latitude and longitude as doubles, this method simply converts the doubles to strings and 
       * calls the string argument version of this function.
       * @param lat double, the latitude of the target area, [-90.0, 90.0], will be extended to two digits of precision.
       * @param lng double, the longitude of the target area, [-180.0, 180.0], will be extended to two digits of precision.
       * @param params a DataOptionalParameters object with the desired optional parameter set.
       *   The optional parameters for this request are: dist, back, maxResults, hotspot
       * @return Observations a container of the observations received from the request.
       */
      Observations get_recent_nearby_notable_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

      /*
       * Performs the "Recent nearby notable observations" API request with the detailed format
       * There are two requred parameters. The latitude and longitude as doubles (or strings).
       * @param lat double, the latitude of the target area, [-90.0, 90.0], will be extended to two digits of precision.
       * @param lng double, the longitude of the target area, [-180.0, 180.0], will be extended to two digits of precision.
       * @param params a DataOptionalParameters object with the desired optional parameter set.
       *   The optional parameters for this request are: dist, back, maxResults, hotspot
       * @return Observations a container of the observations received from the request.
       */
      DetailedObservations get_detailed_recent_nearby_notable_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;
  };
  
  class RequestFailed: public std::exception
  {
    public:
      RequestFailed() = default;

      virtual const char* what() const throw()
      {
        return "Didn't receive JSON response from the API. Either there is a problem with the service, or an argument isnt't valid";
      }
  };
}

#endif
