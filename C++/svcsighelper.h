/*******************************************************************************
 * svcsighelper.h
 *
 * DESCRIPTION:
 *    Provides helper funcitons for the svcsig, svcsigcommonheader,
 *    and svcsigspectraheader classes
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

/* Name: void svcSigRemoveWhitespace( std::string &s )
 *
 * Description:	Removes leading and trailing whitespace from a string
 * Arguments: std::string &s: the string
 * Modifies: std::string &s
 * Returns:
 * Pre:
 * Post:
 * Exceptions:
 * Notes:
 */
void svcSigRemoveWhitespace( std::string &s )
{
  std::string whitespace( " \t\n\r" );
  size_t strBegin = s.find_first_not_of( whitespace );
  if ( strBegin != std::string::npos ) {
    // if there is no string, nothing to do
    size_t strEnd = s.find_last_not_of( whitespace );
    size_t strRange = strEnd - strBegin + 1;
    
    s = s.substr( strBegin, strRange );
  }
}

/* Name: void svcSigRemoveWhitespace( std::string &s )
 *
 * Description:	Splits a header string into the right and left side of the equals
 * Arguments: std::string &s: the string
 *            std::string &p: the key
 * Modifies: std::string &s, std::string &p
 * Returns:
 * Pre:
 * Post:
 * Exceptions: invalidSVCsigHeader if s does not contain an equal sign
 * Notes:
 */
void svcSigParseHeaderEquals( std::string &s, std::string &p )
{
  try {
    size_t loc;
    loc = s.find( "=" );
    if ( loc != std::string::npos ) {
      p = s.substr( 0, loc );
      s = s.substr( loc + 1, s.size() - loc - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
    else {
      throw invalidSVCsigHeader( "in svcSigParseHeaderEquals" );
    }
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

void svcSigParseHeaderComma( std::string &s, std::string &p )
{
  try {
    size_t loc;
    loc = s.find( "," );
    if ( loc != std::string::npos ) {
      p = s.substr( 0, loc );
      s = s.substr( loc + 1, s.size() - loc - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
    else {
      throw invalidSVCsigHeader( "in svcSigParseHeaderComma");
    }
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

void svcSigParseHeaderSquareBraces( std::string &s, std::string &p )
{
  p.clear();
  size_t loc1, loc2;
  loc1 = s.find( "[" );
  if ( loc1 != std::string::npos ) {
    loc2 = s.find( "]" );
    if ( loc2 != std::string::npos ) {
      p = s.substr( loc1 + 1, loc2 - loc1 - 1 );
      s = s.substr( 0, loc1 - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
  }
}

void svcSigParseWhitespace( std::string &s, std::string &p )
{
  try {
    size_t loc;
    loc = s.find_first_of( " \t" );
    if ( loc != std::string::npos ) {
      p = s.substr( 0, loc );
      s = s.substr( loc + 1, s.size() - loc - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
    else {
      throw invalidSVCsigHeader( "in svcSigParseWhitespace");
    }
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

void svcSigParseData( std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef )
{
  try {
    std::string p;
    svcSigParseWhitespace( s, p );
    wl = atof( p.c_str() );
    svcSigParseWhitespace( s, p );
    refRad = atof( p.c_str() );
    svcSigParseWhitespace( s, p );
    tarRad = atof( p.c_str() );
    tarRef = atof( s.c_str() );
    s.clear();
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

#endif // __svcsighelper_h_