/*! \file    Doxygen.hxx
 *  \brief   Main page for Doxygen documentation
 *  \author  Chensong Zhang
 *  \date    Sep/29/2019
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2019--present by the FASP++ team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __DOXYGEN_HXX__ /*-- allow multiple inclusions --*/
#define __DOXYGEN_HXX__ /**< indicate Doxygen.hxx has been included before */

/** \mainpage Introduction
 *
 * The FASPxx is a C++ package designed for developing parallel iterative solvers
 * and preconditioners for PDEs and systems of PDEs. The main components of the
 * package are standard Krylov methods, algebraic multigrid methods, geometric
 * multigrid methods, Schwarz methods, and incomplete factorization methods.
 *
 * This software distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 */

/**
 * \page download How to obtain FASP++
 *
 * The code is freely available on GitHub https://github.com/FaspDevTeam/faspxx.
 *
 */

/**
 * \page build Building and Installation
 *
 * This is a simple instruction on building and testing. There is a top level cmake
 * for configuration and building of the FASPxx shared library and the test programs
 * suite. You can use a cmake-style way to compile the package; see https://cmake.org
 * on how to use cmake for your own operating system. To compile, you alos need a C++
 * compiler.
 *
 * > $ mkdir Build; cd Build; cmake ..
 *
 * > $ make
 *
 * You may config with different cmake options; for example:
 * 
 * > $ cmake -DCMAKE_BUILD_TYPE=Debug .             // build in Debug configuration
 * 
 * > $ cmake -DCMAKE_BUILD_TYPE=Debug -DGTEST=ON .  // build with GTEST support
 * 
 * > $ cmake -DCMAKE_VERBOSE_MAKEFILE=ON .          // build with verbose on
 * 
 */

/**
 * \page developers Developers
 *
 * Project coordinator:
 *
 * - Zhang, Chensong (AMSS, Chinese Academy of Sciences, China)
 *
 * Developers (in alphabetic order):
 *
 * - Fan, Ronghong (AMSS, Chinese Academy of Sciences, China)
 *
 * - Zhang, Kailei (AMSS, Chinese Academy of Sciences, China)
 *
 */

/**
 * \page doxygen_comment Doxygen
 *
 * We use Doxygen as our automatically documentation generator which will make our
 * future maintainance minimized. You can obtain the software (Windows, Linux and
 * OS X) as well as its manual on the official website
 *
 * http://www.doxygen.org
 *
 * For an ordinary user, Doxygen is completely trivial to use. We only need to use
 * some special marker in the usual comment as we put in c-files.
 *
 */

#endif /* end if for __DOXYGEN_HXX__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Chensong Zhang      Sep/29/2019      Create file                          */
/*  Chensong Zhang      Sep/16/2021      Restructure file                     */
/*----------------------------------------------------------------------------*/