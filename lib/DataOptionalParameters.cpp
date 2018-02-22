#include "../include/cbirdpp/DataOptionalParameters.h"

#include <set>
using std::set;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

namespace cbirdpp
{

  DataOptionalParameters DATA_DEFAULT_PARAMS;

  set<string> VALID_CATEGORIES = {"domestic", "form", "hybrid", "intergrade", "issf", "slash", "species", "spuh"};

  DataOptionalParameters::DataOptionalParameters() {}

      /*
       * Set the back optional parameter. The amount of days to look back for observations.
       * @param back An unsigned integer, range: [1-30], default: 14
       */
  void DataOptionalParameters::set_back(const unsigned int back)
  {
    if(back < 1 || back > 30) throw ArgumentOutOfRange(back);
    if(back == 14) {
      _back.reset();
    } else {
      _back = back;
    }
  }

  void DataOptionalParameters::set_cat(const std::string& cat)
  {
    stringstream ss(cat);
    string item;
    set<string> categories;
    while(getline(ss, item, ',')) {
      if(VALID_CATEGORIES.find(item) == VALID_CATEGORIES.end()) throw ArgumentOutOfRange(item);
      categories.emplace(item);
    }
    if(categories == VALID_CATEGORIES) {
      _cat.reset();
    }
    else {
      for(auto it = categories.begin(); it != categories.end(); ++it) {
        *_cat += *it;
        *_cat += ',';
      }
      *_cat = (*_cat).substr(0, (*_cat).substr().length()-2); 
    }
  }

  void DataOptionalParameters::set_maxResults(const unsigned int maxResults)
  {
    if(maxResults > 10000) throw ArgumentOutOfRange(maxResults);
    if(maxResults == 0) {
      _maxResults.reset();
    } else {
      _maxResults = maxResults;
    }
  }

  void DataOptionalParameters::set_includeProvisional(const bool includeProvisional)
  {
    if(!includeProvisional) {
      _includeProvisional.reset();
    } else {
      _includeProvisional = true;
    }
  }

  void DataOptionalParameters::set_hotspot(const bool hotspot)
  {
    if(!hotspot) {
      _includeProvisional.reset();
    } else {
      _includeProvisional = true;
    }
  }

  void DataOptionalParameters::set_detail(const DetailType& detail)
  {
    if(detail == simple) {
      _detail.reset();
    } else {
      _detail = full;
    }
  }

  void DataOptionalParameters::set_sort(const SortType& sort)
  {
    if(sort == date) {
      _sort.reset();   
    } else {
      _sort = species;
    }
  }

  void DataOptionalParameters::set_dist(const unsigned int dist)
  {
    if(dist > 50) throw ArgumentOutOfRange(dist);
    if(dist == 25) {
      _dist.reset();
    } else {
      _dist = 50;
    }
  }

  void DataOptionalParameters::set_rank(const RankType& rank)
  {
    if(rank == mrec) {
      _rank.reset();
    } else {
      _rank = create;
    }
  }

  void DataOptionalParameters::reset()
  {
    _back.reset();
    _cat.reset();
    _maxResults.reset();
    _includeProvisional.reset();
    _hotspot.reset();
    _detail.reset();
    _sort.reset();
    _dist.reset();
    _rank.reset();
  }

}
