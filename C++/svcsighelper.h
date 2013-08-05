/*******************************************************************************
 * svcsighelper.h
 *
 * DESCRIPTION:
 *    Provides helper functions and exception classes for the svcsig,
 *    svcsigcommonheader, and svcsigspectraheader classes
 *
 * HISTORY:
 *    2013-08-04: Written by Paul Romanczyk (RIT)
 *
 * REFERENCES:
 *   "SVC HR-1024i / SVC HR-768i User Manual" v1.6
 *   http://spectravista.com/
 *
 * PUBLIC REPOSITORY:
 *   https://github.com/pavdpr/svcread.git
 *
 * LICENSE:
 *   The MIT License (MIT) Copyright (c) 2013 Paul Romanczyk
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************/

#ifndef __svcsighelper_h_
#define __svcsighelper_h_

#include <string>
#include <stdexcept>

class notSvcSigFile : public std::runtime_error {
public:
  notSvcSigFile()
  : std::runtime_error( "This file is not an SVC sig file" )
  {}
};

class invalidSVCsigHeader : public std::runtime_error {
public:
  invalidSVCsigHeader()
  : std::runtime_error( "Invalid SVC sig header" )
  {}
  invalidSVCsigHeader( const std::string &message )
  : std::runtime_error( "Invalid SVC sig header: " + message )
  {}
};


#endif // __svcsighelper_h_