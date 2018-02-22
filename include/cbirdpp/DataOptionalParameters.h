#ifndef CBIRDPP_PARAMETERS_H
#define CBIRDPP_PARAMETERS_H

#include <optional>
#include "ParameterExceptions.h"

namespace cbirdpp
{
  enum DetailType {simple=0, full};
  enum SortType {species=0, date};
  enum RankType {create=0, mrec};

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

      std::optional<DetailType> _detail;

      std::optional<SortType> _sort;

      std::optional<unsigned int> _dist;

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

      /*
       *  Getters for each optional argument
       */
      const std::optional<unsigned int>& back() const;

      const std::optional<std::string>& cat() const;

      const std::optional<unsigned int>& maxResults() const;

      const std::optional<bool>& includeProvisional() const;

      const std::optional<bool>& hotspot() const;

      const std::optional<DetailType>& detail() const;
      
      const std::optional<SortType>& sort() const;

      const std::optional<unsigned int>& dist() const;

      const std::optional<RankType>& rank() const;

      /*
       * Returns a string formatted for the API request. That is the form NAME=VALUE
       */

      std::string format_back() const;

      std::string format_cat() const;

      std::string format_maxResults() const;

      std::string format_includeProvisional() const;

      std::string format_hotspot() const;

      std::string format_detail() const;

      std::string format_sort() const;

      std::string format_dist() const;
      
      std::string format_rank() const;
  };
}

#endif
