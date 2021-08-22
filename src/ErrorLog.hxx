/** \file    ErrorLog.hxx
 *  \brief   Logging error and warning messages
 *  \author  Ronghong Fan
 *  \date    Nov/01/2019
 * 
 *  Modified by Chensong Zhang on Aug/20/2021: Redesign error messages
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
    "\n    --> file:     " << __FILE__ << '::' << __LINE__

/// Log error messages
//  msg: user-defined error message
//  We use do-while to allow the macro to be ended with ";"
#define _FASPXX_MASSAGE_(msg)                      \
    do {                                           \
        std::ostringstream info;                   \
        info << std::setprecision(16);             \
        info << msg << _FASPXX_LOCATION_ << '\n';  \
        std::cerr << info.str().c_str();           \
    } while(false)

/// Log warning messages
//  msg: user-defined warning message
//  We use do-while to allow the macro to be ended with ";"
#define FASPXX_WARNING(msg)                        \
    do {                                           \
        _FASPXX_MASSAGE_("### WARNING: " << (msg));\
    } while(false)

/// Abort if critical error happens
//  msg: user-defined abort message
//  We use do-while to allow the macro to be ended with ";"
#define FASPXX_ABORT(msg)                          \
    do {                                           \
        _FASPXX_MASSAGE_("### ABORT: " << (msg));  \
        std::abort();                              \
    } while(false)

/// Assert condition and log user messages in DEBUG mode
//  cond: check condition
//  msg: user-defined error message
//  We use do-while to allow the macro to be ended with ";"
#ifndef DEBUG
#define FASPXX_ASSERT(cond, msg) \
    do { } while(false) 
#else
#define FASPXX_ASSERT(cond, msg) \
   do { \
       if (!(cond)) { \
           _FASPXX_MASSAGE_("### ASSERT: " << msg << " (" << #cond << ")"); \
           std::abort(); \
       } \
   } while(false)
#endif 

#endif /* end if for __ERRORLOG_HXX__ */

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/