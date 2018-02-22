#ifndef CBIRDPP_OBSERVATION_H
#define CBIRDPP_OBSERVATION_H

#include <string>
#include <variant>
#include <vector>

namespace cbirdpp
{

  /*
   * A simple container class for holding the information returned by the eBird API when making observation requests.
   * The getter method names directly correlate with the names given to the variables by the JSON response.
   */
  struct Observation
  {
    std::string speciesCode;
    std::string comName;
    std::string sciName;
    std::string locId;
    std::string locName;
    std::string obsDt;
    unsigned int howMany;
    double lat;
    double lng;
    bool obsValid;
    bool obsReviewed;
    bool locationPrivate;
  };

  struct DetailedObservation : public Observation
  {
    std::string checklistId;
    std::string countryCode;
    std::string countryName;
    std::string firstName;
    bool hasComments;
    bool hasRichMedia;
    std::string lastName;
    std::string locID;  // eBird provides locId in normal responses too, they appear to be the same, will remove if that turns out to be the case.
    std::string obsId;
    bool presenceNoted;
    std::string subId;
    std::string subnational1Code;
    std::string subnational1Name;
    std::string subnational2Code;
    std::string subnational2Name;
    std::string userDisplayName;

    operator Observation() const {
      return {speciesCode, comName, sciName, locId, locName, obsDt, howMany, lat, lng,
              obsValid, obsReviewed, locationPrivate};
    }

  };

  struct DetailedObservations : public std::vector<DetailedObservation>
  {
  };

  struct Observations : public std::vector<Observation>
  {
    Observations(const DetailedObservations& rhs)
    {
      for(const DetailedObservation& o : rhs) {
        push_back((Observation)o);
      }
    }

    /*
    Observations& operator=(const DetailedObservations& rhs)
    {
      for(const DetailedObservation& o : rhs) {
        push_back((Observation)o);
      }
      return *this;
    } 
    */
  };

  /*
   * An extension of the Observation class to include field for requests that have the detail argument set
   */
  
}

#endif
