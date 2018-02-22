#ifndef CBIRDPP_OBSERVATION_H
#define CBIRDPP_OBSERVATION_H

#include <string>

namespace cbirdpp
{
  /*
   * A simple container class for holding the information returned by the eBird API when making observation requests.
   * The getter method names directly correlate with the names given to the variables by the JSON response.
   */
  struct Observation
  {
    //private:
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
      /*
    public:
      std::string speciesCode() {return _speciesCode;}
      std::string comName() {return _comName;}
      std::string sciName() {return _sciName;}
      std::string locId() {return _locId;}
      std::string locName() {return _locName;}
      std::string obsDt() {return _obsDt;}
      unsigned int howMany() {return _howMany;}
      double lat() {return _lat;}
      double lng() {return _lng;}
      bool obsValid() {return _obsValid;}
      bool obsReviewed() {return _obsReviewed;}
      bool locationPrivate() {return _locationPrivate;}
      */
  };
}

#endif
