/*******************************************************************************
 * svcsigcommonheader.h
 *
 * DESCRIPTION:
 *    Provides header and implimentaiton for the svcsigcommonheader class
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

#ifndef __svcsigcommonheader_h_
#define __svcsigcommonheader_h_

#include <string>
#include <stdexcept>

#include "svcsighelper.h"


class svcsigcommonheader {
private:
  std::string _name;
  std::string _instrument;
  short _externalDataDark[ 8 ];
  char _externalDataMask;
  std::string _comm;
  float _factors[ 3 ];
  std::string _factorsComment;
protected:
  svcsigcommonheader& updateName( const std::string &name );
  svcsigcommonheader& updateInstrument( const std::string &instrument );
  svcsigcommonheader& updateExternalDataDark( short externalDataDark[ 8 ] );
  svcsigcommonheader& updateExternalDataMask( const char &externalDataMask );
  svcsigcommonheader& updateComm( const std::string &comm );
  svcsigcommonheader& updateFactors( float factors[ 3 ] );
  svcsigcommonheader& updateFactorsComment( const std::string &factorsComment );
public:
  svcsigcommonheader();
  svcsigcommonheader( const svcsigcommonheader &other );
  ~svcsigcommonheader();
  
  void display() const;
  
  std::string name() const;
  std::string instrument() const;
  short * externalDataDark() const;
  char externalDataMask() const;
  std::string comm() const;
  float * factors() const;
  std::string factorsComment() const;
  
  friend class svcsig;
};



// --- --- Implimentation --- --- //

// -- -- Constructors -- -- //
svcsigcommonheader::svcsigcommonheader()
{
  int i;
  _name = "";
  _instrument = "";
  for ( i = 0; i < 8; i++ ) {
    _externalDataDark[ i ] = 0;
  }
  _externalDataMask = 0;
  _comm = "";
  for ( i = 0; i < 3; i++ ) {
    _factors[ i ] = 0.0;
  }
  _factorsComment = "";
}

svcsigcommonheader::svcsigcommonheader( const svcsigcommonheader &other )
{
  int i;
  _name = other._name;
  _instrument = other._instrument;
  for ( i = 0; i < 8; i++ ) {
    _externalDataDark[ i ] = other._externalDataDark[ i ];
  }
  _externalDataMask = other._externalDataMask;
  _comm = other._comm;
  for ( i = 0; i < 3; i++ ) {
    _factors[ i ] = other._factors[ i ];
  }
  _factorsComment = other._factorsComment;
}

// -- -- Destructor -- -- //
svcsigcommonheader::~svcsigcommonheader()
{
  // nothing to do!
}

// -- -- Display -- -- //
void svcsigcommonheader::display() const
{
  int i;
  std::cout << "Common Header:" << std::endl;
  std::cout << "\tName: " << _name << std::endl;
  std::cout << "\tInstrument: " << _instrument << std::endl;
  std::cout << "\tExternal Dark Data:";
  for ( i = 0; i < 8; i++ ) {
    std::cout << " " << _externalDataDark[ i ];
    if ( i < 7 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tExternal Dark Mask: " << (int) _externalDataMask << std::endl;
  std::cout << "\tComm: " << _comm << std::endl;
  std::cout << "\tFactors:";
  for ( i = 0; i < 3; i++ ) {
    std::cout << " " << _factors[ i ];
    if ( i < 2 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tFactors Comment: " << _factorsComment << std::endl;
  std::cout << "End of Common Header" << std::endl;
}

// -- -- Getters -- -- //

std::string svcsigcommonheader::name() const
{
  return _name;
}

std::string svcsigcommonheader::instrument() const
{
  return _instrument;
}

short * svcsigcommonheader::externalDataDark() const
{
  short * tmp = new short[ 8 ];
  for ( int i = 0; i < 8; i++ ) {
    tmp[ i ] = _externalDataDark[ i ];
  }
  return tmp;
}

char svcsigcommonheader::externalDataMask() const
{
  return _externalDataMask;
}

std::string svcsigcommonheader::comm() const
{
  return _comm;
}

float * svcsigcommonheader::factors() const
{
  float * tmp = new float[ 3 ];
  for ( int i = 0; i < 3; i++ ) {
    tmp[ i ] = _factors[ i ];
  }
  return tmp;
}

std::string svcsigcommonheader::factorsComment() const
{
  return _factorsComment;
}

// -- -- Updaters -- -- //

svcsigcommonheader& svcsigcommonheader::updateName( const std::string &name )
{
  _name = name;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateInstrument( const std::string &instrument )
{
  _instrument = instrument;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDark( short externalDataDark[ 8 ] )
{
  for ( int i = 0; i < 8; i++ ) {
    _externalDataDark[ i ] = externalDataDark[ i ];
  }
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataMask( const char &externalDataMask )
{
  _externalDataMask = externalDataMask;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateComm( const std::string &comm )
{
  _comm = comm;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateFactors( float factors[ 3 ] )
{
  for ( int i = 0; i < 3; i++ ) {
    _factors[ i ] = factors[ i ];
  }
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateFactorsComment( const std::string &factorsComment )
{
  _factorsComment = factorsComment;
  return *this;
}
#endif // __svcsigcommonheader_h_