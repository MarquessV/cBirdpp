#include "../include/cbirdpp/cbirdpp.h"
#include "../include/nlohmann/json.hpp"
using nlohmann::json;

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <iostream>

#include <string>
using std::string;
using std::to_string;

#include <list>
using std::list;

#include <sstream>
using std::ostringstream;

#include <utility>
using std::initializer_list;

#include <vector>
using std::vector;

namespace cbirdpp
{  

  vector<string> Requester::process_args(const initializer_list<DataParams>& optional_params, const DataOptionalParameters& params, double lat, double lng, bool detailed) const
  {
    vector<string> args;
    if(lat != 1000.0 && lng != 1000.0) {args = generate_nearby_arguments(lat, lng);}
    if(detailed) {args.emplace_back("detailed=full");}
    for(const DataParams& p : optional_params)
    {
      switch(p) {
        case DataParams::back:
          if(params.back()) {args.emplace_back(params.format_back());}
          break;
        case DataParams::cat:
          if(params.cat()) {args.emplace_back(params.format_cat());}
          break;
        case DataParams::maxResults:
          if(params.maxResults()) {args.emplace_back(params.format_maxResults());}
          break;
        case DataParams::includeProvisional:
          if(params.includeProvisional()) {args.emplace_back(params.format_includeProvisional());}
          break;
        case DataParams::hotspot:
          if(params.hotspot()) {args.emplace_back(params.format_hotspot());}
          break;
        case DataParams::detail:
          if(params.detail()) {args.emplace_back(params.format_detail());}
          break;
        case DataParams::sort:
          if(params.sort()) {args.emplace_back(params.format_sort());}
          break;
        case DataParams::dist:
          if(params.dist()) {args.emplace_back(params.format_dist());}
          break;
        default:
          throw "Somehow an invalid data parameter was given to DataOptionalParameters::process_args()";
      }
    }
    return args;
  }

  string Requester::generate_argument_string(const vector<string>& args) const
  {
    string arguments;
    if(!args.empty()) {
      arguments += "?";
      for(const string& arg : args) {
        arguments += arg;
        if(arg != args.back()) {
          arguments += "&";
        }
      }
    }
    return arguments;
  }

  vector<string> Requester::generate_nearby_arguments(const double lat, const double lng) const
  {
    if(lat < -90.0 || lat > 90.0) {throw ArgumentOutOfRange(lat);}
    if(lng < -180.0 || lng > 180.0) {throw ArgumentOutOfRange(lng);}
    return {"lat=" + to_string(lat), "lng=" + to_string(lng)};
  }

  json Requester::request_json(const std::string& request_url) const
  {
    cURLpp::Cleanup cleaner;
    cURLpp::Easy request_handle;
    request_handle.setOpt(cURLpp::Options::Url(request_url));
    request_handle.setOpt(cURLpp::Options::Header(true));
    request_handle.setOpt(cURLpp::Options::HttpHeader(list<string>({"X-eBirdApiToken: " + api_key})));
    ostringstream out_stream;
    request_handle.setOpt(new cURLpp::Options::WriteStream(&out_stream));
    request_handle.perform();

    size_t json_start;
    if((json_start = out_stream.str().find('[')) == string::npos) {throw cbirdpp::RequestFailed();}

    /*
    std::cout << request_url << std::endl;
    std::cin.get();
    std::cout << out_stream.str() << std::endl;
    std::cin.get();
    */

    json response = json::parse(out_stream.str().substr(json_start));

    return response;
  }


}
