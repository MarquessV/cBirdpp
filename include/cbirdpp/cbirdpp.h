#ifndef CBIRDPP_CBIRDPP_H
#define CBIRDPP_CBIRDPP_H

#include "Observation.h"
#include "DataOptionalParameters.h"

#include<any>
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
      std::vector<Observation> get_recent_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS);

      /*
       * Performs the "Recent notable observations in a region" API request.
       * The only required parameter is a string, the region code as either a eBird locId or subnational2 code.
       * @param regionCode the eBird locId or subnational2 code of the desired region to get observations from.
       * @param back the number of days back to look for observations. Range: [1-30]. Default: 14.
       * @param maxResults the maximum number of results to return. Range: [1-10000]. Default: 0 (all)
       * @param detail set to all to retrieve all sighting fields or simple to receive a subset of the fields. Range: [simple|full]. Default: "simple".
       * @param hotspot set to true if results should be limited to sightings at birding hotspots. Default: false.
       * @return vector of the observations received from the request.
       */
      DetailedObservations get_recent_notable_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS);
       
  };
}

#endif
