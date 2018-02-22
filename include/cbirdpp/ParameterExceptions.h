#ifndef CBIRDPP_PARAMETEREXCEPTIONS
#define CBIRDPP_PARAMETEREXCEPTIONS

#include <exception> // for std::exception

namespace cbirdpp 
{

  template <typename Arg>
  class ArgumentOutOfRange: public std::exception
  {
    private:
      Arg argument;

    public:
      ArgumentOutOfRange(Arg arg) : argument(arg) {};

      virtual const char* what() const throw()
      {
        return "Out of range argument given to an optional parameter";
      }
  };
}

#endif
