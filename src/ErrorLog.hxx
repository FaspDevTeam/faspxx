/** \file ErrorLog.hxx
 *  \brief Logging error and warning messages
 *  \author Ronghong Fan
 *  \date Nov/01/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __ERRORLOG_HXX__      /*-- allow multiple inclusions --*/
#define __ERRORLOG_HXX__      /**< indicate ErrorLog.hxx has been included before */

#include <sstream>
#include <iomanip>

using namespace std;

#define _FASPXX_LOCATION_                                         \
        "\n  ---> in file:     " << __FILE__ << ':' << __LINE__ << \
        "\n  ---> in function: " << __PRETTY_FUNCTION__ << '\n'

#define _FASPXX_MASSAGE_(msg)                         \
        {                                             \
            std::ostringstream info;                  \
            info << std::setprecision(16);            \
            info << '\n' << msg << _FASPXX_LOCATION_; \
            std::cout << info.str().c_str();          \
        }

#define FASPXX_ABORT(msg)                              \
        {                                              \
            _FASPXX_MASSAGE_("### ABORT: " << msg);    \
            std::abort();                              \
        }

#define FASPXX_ASSERT(TRUE, msg)                                       \
   if (!(TRUE)) {                                                      \
      _FASPXX_MASSAGE_("### ASSERT: " << msg << " (" << #TRUE << ")"); \
   }

#define FASPXX_WARNING(msg)                            \
        {                                              \
            _FASPXX_MASSAGE_("### WARNING: " << msg);  \
        }

#endif /* end if for __ERRORLOG_HXX__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/