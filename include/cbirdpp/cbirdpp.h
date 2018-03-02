#ifndef CBIRDPP_CBIRDPP_H
#define CBIRDPP_CBIRDPP_H

#include "Checklist.h"
#include "DataOptionalParameters.h"
#include "Observation.h"
#include "RegionalStats.h"
#include "Top100.h"

#include "../nlohmann/json.hpp"

#include <string>
#include <vector>

namespace cbirdpp {

enum SortType {obs_dt, creation_dt};

extern DataOptionalParameters DATA_DEFAULT_PARAMS;

/** \class Requester
 *  \brief The primary interface for making requests to the eBird API
 *
 *  This is the primary class in cBirdpp. The Requester is constructed using the users eBird 2.0 API key, and has public
 *  methods for making and returning all twenty eBird 2.0 API requests. In general, each request has a method with the
 *  same name using snake_case, and without 'a'. For example, the method for get recent observations in a region is 
 *  called get_recent_observations_in_region. Each method takes its required params as arguments and optionally, any
 *  optional request parameters. In the case of "data/obs" type request, there is a class called DataOptionalParameters
 *  that holds the settings for those kinds of requests. For other type of requests, methods are overloaded so that every 
 * permutation of optional parameters is possible.
 */
class Requester {
  private:
    std::string api_key;

    /// Processes DataOptionalParams into a vector of string arguments. 
    /** This version of the function takes all possible mandatory arguments as well as
     *  a DataOptionalParameters object of the optional arguments. All other overloaded
     *  versions of process_arg call some permutation of this method.
     *  @param optional_params an initializer list of the optional parameters that need to be checked for non-default values.
     *  @param params the DataOptionalParameters object to check for non-default values.
     *  @param lat a double representing the latitude for various "nearby" requests.
     *  @param lng a double representing the longitude for various "nearby" requests.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @param nearby_args a bool that should be set to true if latitude longitude are relevant for this request.
     *  @return a vector of strings in the format "param=arg" for the required and non-default optional parameters.
     */
    std::vector<std::string> process_args(const std::initializer_list<DataParams>& optional_params, const DataOptionalParameters& params, double lat, double lng, bool detailed, bool nearby_args) const;

    /// Processes DataOptionalParams into a vector of string arguments. 
    /** This version of the function takes only the optional parameters available in the DataOptionalParams object.
     *  @param optional_params an initializer list of the optional parameters that need to be checked for non-default values.
     *  @param params the DataOptionalParameters object to check for non-default values.
     *  @return a vector of strings in the format "param=arg" for the required and non-default optional parameters.
     */
    std::vector<std::string> process_args(const std::initializer_list<DataParams>& optional_params, const DataOptionalParameters& params) const;

    /// Processes DataOptionalParams into a vector of string arguments. 
    /** This version of the function takes the optional parameters available in the DataOptionalParams object and whether 
     *  or not the request should return the detailed format.
     *  @param optional_params an initializer list of the optional parameters that need to be checked for non-default values.
     *  @param params the DataOptionalParameters object to check for non-default values.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @return a vector of strings in the format "param=arg" for the required and non-default optional parameters.
     */
    std::vector<std::string> process_args(const std::initializer_list<DataParams>& optional_params, const DataOptionalParameters& params, bool detailed) const;

    /// Processes DataOptionalParams into a vector of string arguments. 
    /** This version of the function takes the optional parameters available in the DataOptionalParams object, latitude
     *  longitude, and optionally, whether or nothe the request should return the detailed format.
     *  @param optional_params an initializer list of the optional parameters that need to be checked for non-default values.
     *  @param params the DataOptionalParameters object to check for non-default values.
     *  @param lat a double representing the latitude for various "nearby" requests.
     *  @param lng a double representing the longitude for various "nearby" requests.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @return a vector of strings in the format "param=arg" for the required and non-default optional parameters.
     */
    std::vector<std::string> process_args(const std::initializer_list<DataParams>& optional_params, const DataOptionalParameters& params, double lat, double lng, bool detailed=false) const;

    /// Generates a list of arguments in the format required for the request URL.
    /** The format is ?param0=arg0&param1=arg1&...&paramn=argn
     *  @param args a vector of arguments to be added to the request
     *  @return a string that can be appended to a request URL defining all arguments given to the method.
     */
    std::string generate_argument_string(const std::vector<std::string>& args) const;

    /// Generates the latitude and longitude arguments for the various "nearby" requests.
    /** 
     *  @param lat a double representing the latitude for "nearby" requests.
     *  @param lng a double representing the longitude for "nearby" requests.
     *  @return a vector of two strings with the format "lat={lat}" and "lng={lng}".
     */ 
     std::vector<std::string> generate_nearby_arguments(double lat, double lng) const;

    /// Generates date string formatted for the API request url.
    /** The format is y/m/d
     *  @param year, the year
     *  @param month, the month
     *  @param day, the day
     *  @return a string representing the date in the format required by the request URL.
     */
     std::string generate_date(int year, int month, int day) const;

    /// Takes a request URL and returns the result as a JSON object.
    /** This method assumes a JSON object will be returned and throws an exception if one is not.
     *  @param request_url the url of the request to be made.
     *  @return a json object of the result.
     */
    nlohmann::json request_json(const std::string& request_url) const;

     /// Takes some source JSON and converts it to a container of the given base type.
     /** This method requires that from_json(const json& source, T& target has been defined in the cBirdpp namespace.
      *  In addition, it is assumed Container is a sub class of std::vector and uses the push_back() method. If it isn't
      *  a wrapper around a vector, then push_back(Base& b) must be defined on the Container class.
      *  @param source the json to be converted
      *  @return A collection of the results as type Base, held in a Container
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
    
    /// Performs the common setup between the get recent notable observations in a region requests.
    /** There is a simple and detailed variation of the get recent notable observations request. Both require basically
     *  the same setup up to processing the resulting JSON. This method performs that setup.
     *  @param regionCode any eBird locId or subnational2 code, or ISO/eBird subnational1 or country code.
     *  @param params an optional DataOptionalParameters object with any desired optional parameters set.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @return the json of the resulting get recent notable observations in a region request.
     */
    nlohmann::json get_recent_notable_setup(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS, bool detailed=false) const;

    
    /// Performs the common setup between the get recent nearby notable observations in a region requests.
    /** There is a simple and detailed variation of the get recent nearby notable observations request. Both require basically
     *  the same setup up to processing the resulting JSON. This method performs that setup.
     *  @param lat a double representing the latitude for "nearby" requests.
     *  @param lng a double representing the longitude for "nearby" requests.
     *  @param params an optional DataOptionalParameters object with any desired optional parameters set.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @return the json of the resulting get recent notable observations in a region request.
     */
    nlohmann::json get_recent_nearby_notable_setup(double lat, double lng, const DataOptionalParameters& params, bool detailed=false) const;

    /// Performs the common setup between the get historic observations on a date request.
    /** There is a simple and detailed variation of the get historic observations on a date request. Both require basically
     *  the same setup up to processing the resulting JSON. This method performs that setup.
     *  @param regionCode any eBird locId or subnational2 code, or ISO/eBird subnational1 or country code.
     *  @param year, the year
     *  @param month, the month
     *  @param day, the day
     *  @param params an optional DataOptionalParameters object with any desired optional parameters set.
     *  @param detailed a bool that should be set to true if the detail format of a request is needed and false otherwise.
     *  @return the json of the resulting get recent notable observations in a region request.
     */
    nlohmann::json get_historic_observations_on_date_setup(const std::string& regionCode, int year, int month, int day, const DataOptionalParameters& params, bool detailed=false) const;

  public:
    /** The only available constructor, takes an api key as a string.
     *  @param key the api key the requester will use to formulate requests.
     */
    Requester(const std::string& key)
    {
      api_key = key;
    }

    /// Performs the "get recent observations in a region" request and returns the results.
    /** The only required argument is the region code as an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in an Observations object.
     */
    Observations get_recent_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent notable observations in a region" request and returns the results.
    /** The only required argument is the region code as an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in an Observations object.
     */
    Observations get_recent_notable_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent notable observations in a region" with the format parameter set to detail
    /** The only required argument is the region code as an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in a DetailedObservations object.
     */
    DetailedObservations get_detailed_recent_notable_observations_in_region(const std::string& regionCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent observations of a species in a region" request and returns the results.
    /** The required arguments are the region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and a species code in the current eBird taxonomy.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param speciesCode a string containing a species code in the current eBird taxonomy.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in an Observations object.
     */
    Observations get_recent_observations_of_species_in_region(const std::string& regionCode, const std::string& speciesCode, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;
     
    /// Performs the "get recent nearby observations" request and returns the results.
    /** The required arguments are the latitude and longitude of the area to check nearby.
     *  @param lat the latitude of the target area as a double in the range [-90.0, 90.0], precision will be truncated/extended to 6 digits.
     *  @param lng the longitude of the target area as a double in the range [-180.0, 180.0], precision will be truncated/extended to 6 digits.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in an Observations object.
     */
    Observations get_recent_nearby_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent nearby notable observations" request and returns the results.
    /** The required arguments are the latitude and longitude of the area to check nearby.
     *  @param lat the latitude of the target area as a double in the range [-90.0, 90.0], precision will be truncated/extended to 6 digits.
     *  @param lng the longitude of the target area as a double in the range [-180.0, 180.0], precision will be truncated/extended to 6 digits.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in an Observations object.
     */
    Observations get_recent_nearby_notable_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent nearby notable observations" request with the format parameter set to detail.
    /** The required arguments are the latitude and longitude of the area to check nearby.
     *  @param lat the latitude of the target area as a double in the range [-90.0, 90.0], precision will be truncated/extended to 6 digits.
     *  @param lng the longitude of the target area as a double in the range [-180.0, 180.0], precision will be truncated/extended to 6 digits.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return any observations returned by the request are returned in a DetailedObservations object.
     */
    DetailedObservations get_detailed_recent_nearby_notable_observations(double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get recent nearby observations of a species" request and returns the results.
    /** The required arguments are a species code in the current eBird taxonomy, and the latitude and longitude of the area to check nearby.
     *  @param speciesCode a string containing a species code in the current eBird taxonomy.
     *  @param lat the latitude of the target area as a double in the range [-90.0, 90.0], precision will be truncated/extended to 6 digits.
     *  @param lng the longitude of the target area as a double in the range [-180.0, 180.0], precision will be truncated/extended to 6 digits.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return Observations a container of the observations received from the request.
     */ 
    Observations get_recent_nearby_observations_of_species(const std::string& speciesCode, double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get nearest observations of a species" request and returns the results.
    /** The required arguments are a species code in the current eBird taxonomy, and the latitude and longitude of the area to check nearby.
     *  @param speciesCode a string containing a species code in the current eBird taxonomy.
     *  @param lat the latitude of the target area as a double in the range [-90.0, 90.0], precision will be truncated/extended to 6 digits.
     *  @param lng the longitude of the target area as a double in the range [-180.0, 180.0], precision will be truncated/extended to 6 digits.
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return Observations a container of the observations received from the request.
     */
    Observations get_nearest_observations_of_species(const std::string& speciesCode, double lat, double lng, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get historic observations on a date" request and returns the results.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return Observations a container of the observations received from the request.
     */
    Observations get_historic_observations_on_date(const std::string& regionCode, int year, int month, int day, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get historic observations on a date" request with the format argument set to detail and returns the results.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param params a DataOptionalParameters object that will be used for setting any optional parameters. This is optional, and uses all defaults if it isn't provided.
     *  @return Observations a container of the observations received from the request.
     */
    DetailedObservations get_detailed_historic_observations_on_date(const std::string& regionCode, int year, int month, int day, const DataOptionalParameters& params=DATA_DEFAULT_PARAMS) const;

    /// Performs the "get top 100" request and returns the results.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param checklistSort set to true to sort by number of complete checklists, optional and false by default.
     *  @param maxResults an int greater than or equal to 1 that sets the maximum number of contributors to return, optional, 100 by default.
     *  @returns a collection of the results as a Top100 object.
     */
    Top100 get_top_100(const std::string& regionCode, int year, int month, int day, bool checklistSort=false, unsigned int maxResults=100) const;

    /// Performs the "get top 100" request and returns the results.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param maxResults an int greater than or equal to 1 that sets the maximum number of contributors to return.
     *  @returns a collection of the results as a Top100 object.
     */
    Top100 get_top_100(const std::string& regionCode, int year, int month, int day, unsigned int maxResults) const;

    /// Performs the "get checklist feed on a date" request and returns the result.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param sortKey whether to sort results by latest observation date or by latest creation date. {SortType::obs_dt | SortType::creation_dt}. Optional, obs_dt by default.
     *  @param maxResults the maximum number of checklists to show as an int in the range [1-200], optional, 10 by default.
     *  @return a collection of the results as a checklists object.
     */
    Checklists get_checklist_feed_on_date(const std::string& regionCode, int year, int month, int day, SortType sortKey=SortType::obs_dt, unsigned int maxResults=10);

    /// Performs the "get checklist feed on a date" request and returns the result.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param maxResults the maximum number of checklists to show as an int in the range [1-200].
     *  @return a collection of the results as a checklists object.
     */
    Checklists get_checklist_feed_on_date(const std::string& regionCode, int year, int month, int day, unsigned int maxResults);

    /// Performs the "get recent checklists feed" request and returns the results.
    /** The only required argument is the region code as an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param maxResults the maximum number of checklists to show as an int in the range [1-200].
     *  @return a collection of the results as a checklists object.
     */
    Checklists get_recent_checklists_feed(const std::string& regionCode, unsigned int maxResults=10);

    /// Performs the "get regional statistics on a date" request and returns the results.
    /** The required arguments are a region code as an eBird locId, subnational2 code, subnational1 code, or country code
     *  and the year, month, and day of the desired date.
     *  @param regionCode a string containing either an eBird locId, subnational2 code, subnational1 code, or country code.
     *  @param year the year of the desired date as an int in the range [1800-current]
     *  @param month the month of the desired date as an int in the range [1-12]
     *  @param day the day of the desired date as an int in the range [1-31]
     *  @param maxResults the maximum number of checklists to show as an int in the range [1-200].
     *  @return the stats as a RegionalStats object.
     */
    RegionalStats get_regional_statistics_on_date(const std::string& regionCode, unsigned int year, unsigned int month, unsigned int day);

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
