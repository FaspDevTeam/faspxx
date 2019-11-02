
#ifndef FASPXX_ERROR_HXX
#define FASPXX_ERROR_HXX

#include <sstream>
#include <iomanip>

using namespace std;


#define _FASPXX_LOCATION_                                         \
        "\n ---> in file:     " << __FILE__ << ':' << __LINE__ << \
        "\n ---> in function: " << __PRETTY_FUNCTION__ << '\n'


#define _FASPXX_MASSAGE_(msg)                         \
        {                                             \
            std::ostringstream info;                  \
            info << std::setprecision(16);            \
            info << '\n' << msg << _FASPXX_LOCATION_; \
            std::cout << info.str().c_str();          \
        }


#define FASPXX_ABORT(msg)                              \
        {                                              \
            _FASPXX_MASSAGE_("FASPXX abort: " << msg); \
            std::abort();                              \
        }


#define FASPXX_ASSERT(TRUE, msg)                              \
   if (!(TRUE))                                               \
   {                                                          \
      _FASPXX_MASSAGE_("FASPXX assertion failed: ("           \
                    << #TRUE << ") is false:\n --> " << msg); \
   }

#define FASPXX_WARNING(msg) _FASPXX_MASSAGE_(msg)


#endif //FASPXX_ERROR_HXX
