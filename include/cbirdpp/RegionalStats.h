#ifndef CBIRDPP_REGIONALSTATS_H
#define CBIRDPP_REGIONALSTATS_H

namespace cbirdpp
{

  /*
   * A simple container class for holding the information returned by the eBird API when making the regional statistics on a date request.
   */
  struct RegionalStats
  {
    unsigned int numChecklists;
    unsigned int numContributors;
    unsigned int numSpecies;
  };

}

#endif
