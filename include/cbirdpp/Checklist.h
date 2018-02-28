#ifndef CBIRDPP_CHECKLIST_H
#define CBIRDPP_CHECKLIST_H

#include <string>
#include <vector>

namespace cbirdpp
{

  /*
   * A simple container class for holding the information returned by the eBird API when making checklist related requests.
   * The getter method names directly correlate with the names given to the variables by the JSON response.
   */
  struct Checklist
  {
    std::string locId;
    std::string subID;
    std::string userDisplayName;
    unsigned int numSpecies;
    std::string obsDt;
    std::string obsTime;
    std::string obsMonth;
    unsigned int obsDay;
    unsigned int obsYear;
    std::string name;
    double latitude;
    double longitude;
    std::string countryCode;
    std::string countryName;
    std::string subnational1Name;
    std::string subnational1Code;
    std::string subnational2Name;
    std::string subnational2Code;
    bool isHotspot;
    std::string hierarchicalName;
  };

  struct Checklists : public std::vector<Checklist>
  {
    Checklists() = default;
  };

}

#endif
