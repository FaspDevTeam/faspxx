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

#ifndef __ERRORLOG_HXX__ /*-- allow multiple inclusions --*/
#define __ERRORLOG_HXX__ /**< indicate ErrorLog.hxx has been included before */

// Standard header files
#include <iomanip>
#include <iostream>
#include <sstream>

/// Print out location at (file, line) and function name
#define FASPXX_LOCATION                                                                \
    "\n    --> function: " << __PRETTY_FUNCTION__                                      \
                           << "\n    --> file:     " << __FILE__ << "::" << __LINE__

/// Log error messages
//  msg: user-defined error message
//  We use do-while to allow the macro to be ended with ";"
#define FASPXX_MASSAGE(msg)                                                            \
    do {                                                                               \
        std::ostringstream info;                                                       \
        info << std::setprecision(16);                                                 \
        info << msg << FASPXX_LOCATION << '\n';                                        \
        std::cerr << info.str().c_str();                                               \
    } while (false)

/// Log warning messages
//  msg: user-defined warning message
//  We use do-while to allow the macro to be ended with ";"
#define FASPXX_WARNING(msg)                                                            \
    do {                                                                               \
        FASPXX_MASSAGE("### WARNING: " << (msg));                                      \
    } while (false)

/// Abort if critical error happens
//  msg: user-defined abort message
//  We use do-while to allow the macro to be ended with ";"
#define FASPXX_ABORT(msg)                                                              \
    do {                                                                               \
        FASPXX_MASSAGE("### ABORT: " << (msg));                                        \
        std::abort();                                                                  \
    } while (false)

/// Assert condition and log user messages in DEBUG mode
//  cond: check condition
//  msg: user-defined error message
//  We use do-while to allow the macro to be ended with ";"
#ifndef DEBUG
#define FASPXX_ASSERT(cond, msg)                                                       \
    do {                                                                               \
    } while (false)
#else
#define FASPXX_ASSERT(cond, msg)                                                       \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            FASPXX_MASSAGE("### ASSERT: " << msg << " (" << #cond << ")");             \
            std::abort();                                                              \
        }                                                                              \
    } while (false)
#endif

#endif /* end if for __ERRORLOG_HXX__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Kailei Zhang        Nov/01/2019      Create file                          */
/*  Chensong Zhang      Aug/20/2021      Redesign error messages              */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/