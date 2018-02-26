#ifndef CBIRDPP_TOP100_H
#define CBIRDPP_TOP100_H

#include <string>
#include <vector>

namespace cbirdpp
{

  /*
   * A simple container class for holding the information returned by the eBird API when making Top 100 request
   * The getter method names directly correlate with the names given to the variables by the JSON response.
   */
  struct Top100Base
  {
    std::string profileHandle;
    std::string userDisplayName;
    unsigned int numSpecies;
    unsigned int numCompleteChecklists;
    unsigned int rowNum;
    std::string userId;
  };
  
  struct Top100 : public std::vector<Top100Base>
  {
    Top100() = default;
  };
}

#endif
