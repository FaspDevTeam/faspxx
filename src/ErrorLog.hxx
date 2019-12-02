/** \file    ErrorLog.hxx
 *  \brief   Logging error and warning messages
 *  \author  Ronghong Fan
 *  \date    Nov/01/2019
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
#include <iostream>

/// Print out location at (file, line) and function name
#define _FASPXX_LOCATION_                                 \
    "\n    --> function: " << __PRETTY_FUNCTION__ <<      \
    "\n    --> file:     " << __FILE__ << ':' << __LINE__

/// Log error messages
//  msg: user-defined error message
#define _FASPXX_MASSAGE_(msg)                      \
    {                                              \
        std::ostringstream info;                   \
        info << std::setprecision(16);             \
        info << msg << _FASPXX_LOCATION_ << '\n';  \
        std::cout << info.str().c_str();           \
    }

/// Log warning messages
//  msg: user-defined error message
#define FASPXX_WARNING(msg)                        \
    {                                              \
        _FASPXX_MASSAGE_("### WARNING: " << (msg));\
    }

/// Abort if critical error happens
//  msg: user-defined error message
#define FASPXX_ABORT(msg)                          \
    {                                              \
        _FASPXX_MASSAGE_("### ABORT: " << msg);    \
        std::abort();                              \
    }

/// Check condition and log user messages
//  cond: check condition
//  msg: user-defined error message
#define FASPXX_ASSERT(cond, msg)                                       \
   if (!(cond)) {                                                      \
      _FASPXX_MASSAGE_("### ASSERT: " << msg << " (" << #cond << ")"); \
   }

#endif /* end if for __ERRORLOG_HXX__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/