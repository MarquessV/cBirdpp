#include "../include/cbirdpp/DataOptionalParameters.h"

#include <optional>
using std::optional;

#include <set>
using std::set;

#include <string>
using std::string;
using std::to_string;

#include <sstream>
using std::stringstream;

#include <utility>
using std::initializer_list;

#include <vector>
using std::vector;


namespace cbirdpp
{

  DataOptionalParameters DATA_DEFAULT_PARAMS;

  set<string> VALID_CATEGORIES = {"domestic", "form", "hybrid", "intergrade", "issf", "slash", "species", "spuh"};

  DataOptionalParameters::DataOptionalParameters() = default;

      /*
       * Set the back optional parameter. The amount of days to look back for observations.
       * @param back An unsigned integer, range: [1-30], default: 14
       */
  void DataOptionalParameters::set_back(const unsigned int back)
  {
    if(back < 1 || back > 30) {throw ArgumentOutOfRange(back);}
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
      if(VALID_CATEGORIES.find(item) == VALID_CATEGORIES.end()) {throw ArgumentOutOfRange(item);}
      categories.emplace(item);
    }
    if(categories == VALID_CATEGORIES) {
      _cat.reset();
    }
    else {
      _cat = "";
      for(const string& s : categories) {
        *_cat += s;
        if(s != *categories.rbegin()) {
          *_cat += ','; 
        }
      }
    }
  }

  void DataOptionalParameters::set_maxResults(const unsigned int maxResults)
  {
    if(maxResults > 10000) {throw ArgumentOutOfRange(maxResults);}
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
      _hotspot.reset();
    } else {
      _hotspot = true;
    }
  }

  void DataOptionalParameters::set_detail(const DetailType& detail)
  {
    if(detail == DetailType::simple) {
      _detail.reset();
    } else {
      _detail = DetailType::full;
    }
  }

  void DataOptionalParameters::set_sort(const SortType& sort)
  {
    if(sort == SortType::date) {
      _sort.reset();   
    } else {
      _sort = SortType::species;
    }
  }

  void DataOptionalParameters::set_dist(const unsigned int dist)
  {
    if(dist > 50) {throw ArgumentOutOfRange(dist);}
    if(dist == 25) {
      _dist.reset();
    } else {
      _dist = 50;
    }
  }

  void DataOptionalParameters::set_rank(const RankType& rank)
  {
    if(rank == RankType::mrec) {
      _rank.reset();
    } else {
      _rank = RankType::create;
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

  const optional<unsigned int>& DataOptionalParameters::back() const
  {
    return _back;  
  }

  const optional<std::string>& DataOptionalParameters::cat() const
  {
    return _cat;  
  }

  const optional<unsigned int>& DataOptionalParameters::maxResults() const
  {
    return _maxResults;
  }

  const optional<bool>& DataOptionalParameters::includeProvisional() const
  {
    return _includeProvisional;
  }

  const optional<bool>& DataOptionalParameters::hotspot() const
  {
    return _hotspot;
  }

  const optional<DetailType>& DataOptionalParameters::detail() const
  {
    return _detail;
  }

  const optional<SortType>& DataOptionalParameters::sort() const
  {
    return _sort;
  }
  const optional<unsigned int>& DataOptionalParameters::dist() const
  {
    return _dist;
  }
  const optional<RankType>& DataOptionalParameters::rank() const
  {
    return _rank;
  }

  string DataOptionalParameters::format_back() const
  {
    if(!_back) {return "";}
    return "back=" + to_string(*_back);
  }

  string DataOptionalParameters::format_cat() const
  {
    if(!_cat) {return "";}
    return "cat=" + *_cat;
  }

  string DataOptionalParameters::format_maxResults() const
  {
    if(!_maxResults) {return "";}
    return "maxResults=" + to_string(*_maxResults);
  }

  string DataOptionalParameters::format_includeProvisional() const
  {
    if(!_includeProvisional) {return "";}
    return (string)"includeProvisional=" + (*_includeProvisional ? "true" : "false");
  }

  string DataOptionalParameters::format_hotspot() const
  {
    if(!_hotspot) {return "";}
    return "hotspot=true";
  }

  string DataOptionalParameters::format_detail() const
  {
    if(!_detail) {return "";}
    return "detail=full";
  }

  string DataOptionalParameters::format_sort() const
  {
    if(!_sort) {return "";}
    return "sort=species";
  }

  string DataOptionalParameters::format_dist() const
  {
    if(!_dist) {return "";}
    return "dist=" + to_string(*_dist);
  }

  string DataOptionalParameters::format_rank() const
  {
    if(!_rank) {return "";}
    return "rank=create";
  }

  
}
