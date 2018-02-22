#ifndef CBIRDPP_PARAMETERS_H
#define CBIRDPP_PARAMETERS_H

#include <optional>
#include "ParameterExceptions.h"

namespace cbirdpp
{
  /*
   * A class providing an interface for setting optional parameters for request of the dat" variety.
   * This set of parameters can optionally be passed to the data request methods and the approrpiate optional parameters
   * will be used when making the request.
   */
  class DataOptionalParameters
  {
    private:
      std::optional<unsigned int> _back;
      std::optional<std::string> _cat;
      std::optional<unsigned int> _maxResults;
      std::optional<bool> _includeProvisional;
      std::optional<bool> _hotspot;

      enum DetailType {simple, full};
      std::optional<DetailType> _detail;

      enum SortType {species, date};
      std::optional<SortType> _sort;

      std::optional<unsigned int> _dist;

      enum RankType {create, mrec};
      std::optional<RankType> _rank;

    public:
      /*
       * Default constructor. Every optional will be default initialized, and thus will be ignored until they are set.
       */ 
      DataOptionalParameters();

      /*
       * Set the back optional parameter. The amount of days to look back for observations.
       * @param back An unsigned integer, range: [1-30], default: 14
       */
      void set_back(const unsigned int back);

      /*
       * Set the cat optional parameter. The taxonomic categories to filter results by.
       * @param cat A comma separated string consisting of any combination of the following:
       *    [domestic|form|hybrid|intergrade|issf|slash|species|spuh], default: all categories.
       */
      void set_cat(const std::string& cat);

      /*
       * Set the maxResults optional parameter. The upper limit on the amount of result rows to return.
       * @param maxResults an unsigned integer, range: [1-10000], default: 0 (no max)
       */
      void set_maxResults(const unsigned int maxResults);

      /*
       * Set the includeProvisional optional parameter. Set to true if you want to include unreviewed flagged observations.
       * @param includeProvisional a bool, range: [true|false], default: false
       */
      void set_includeProvisional(const bool includeProvisional);

      /*
       * Set the hotspot optional parameter. Set to true if results should be limited to sightings at birding hotspots.
       * @param hotspot a bool, range: [true|false], default: false
       */
      void set_hotspot(const bool hotspot);

      /*
       * Set the detail optional parameter. Set to full if you want all sighting fields
       * @param detail A DetailType enum, range: [simple|full], default: simple
       */
      void set_detail(const DetailType& detail);

      /*
       * Set the sort optional parameter. You can sort by species or date.
       * @param sort a SortType enum, range: [species|date], default: date
       */
      void set_sort(const SortType& sort);

      /*
       * Set the dist optional parameter. The radial distance in kilometers from the region to get observations from.
       * @param dist an unsigned int, range: [0-50], default: 25
       */
      void set_dist(const unsigned int dist);

      /*
       * Set the rank optional parameter. This parameter lets you choose whether to fetch the most recent observation
       * by observation date (mrec) or the most recently created observation (create).
       * @param rank a RankType enum, range: [create|mrec], default: mrec
       */
      void set_rank(const RankType& rank);

      /*
       * Reset all parameters back to defaults.
       */
      void reset();
  };
}

#endif
