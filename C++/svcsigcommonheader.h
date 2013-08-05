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
  // private variables
  std::string _name;
  std::string _instrument;
  short _externalDataDark[ 8 ];
  char _externalDataMask;
  std::string _comm;
  float _factors[ 3 ];
  std::string _factorsComment;
protected:
  // protected member functions. These are intended to be used from sivsig.read( const std::string &filename )
  svcsigcommonheader& updateName( const std::string &name );
  svcsigcommonheader& updateInstrument( const std::string &instrument );
  svcsigcommonheader& updateExternalDataDark( short externalDataDark[ 8 ] );
  svcsigcommonheader& updateExternalDataDarkD1( const short &externalDataDarkD1 );
  svcsigcommonheader& updateExternalDataDarkD2( const short &externalDataDarkD2 );
  svcsigcommonheader& updateExternalDataDarkD3( const short &externalDataDarkD3 );
  svcsigcommonheader& updateExternalDataDarkD4( const short &externalDataDarkD4 );
  svcsigcommonheader& updateExternalDataDarkD5( const short &externalDataDarkD5 );
  svcsigcommonheader& updateExternalDataDarkD6( const short &externalDataDarkD6 );
  svcsigcommonheader& updateExternalDataDarkD7( const short &externalDataDarkD7 );
  svcsigcommonheader& updateExternalDataDarkD8( const short &externalDataDarkD8 );
  svcsigcommonheader& updateExternalDataMask( const char &externalDataMask );
  svcsigcommonheader& updateComm( const std::string &comm );
  svcsigcommonheader& updateFactors( float factors[ 3 ] );
  svcsigcommonheader& updateFactorsSi( const float &factorsSi );
  svcsigcommonheader& updateFactorsInGaAs1( const float &factorsInGaAs1 );
  svcsigcommonheader& updateFactorsInGaAs2( const float &factorsInGaAs2 );
  svcsigcommonheader& updateFactorsComment( const std::string &factorsComment );
public:
  svcsigcommonheader();
  svcsigcommonheader( const svcsigcommonheader &other );
  ~svcsigcommonheader();
  
  void display() const;
//  void displayExternalDataMaskBits() const;
  
  std::string name() const;
  std::string instrument() const;
  short * externalDataDark() const;
  short externalDataDarkD1() const;
  short externalDataDarkD2() const;
  short externalDataDarkD3() const;
  short externalDataDarkD4() const;
  short externalDataDarkD5() const;
  short externalDataDarkD6() const;
  short externalDataDarkD7() const;
  short externalDataDarkD8() const;
  char externalDataMask() const;
  std::string comm() const;
  float * factors() const;
  float factorsSi() const;
  float factorsInGaAs1() const;
  float factorsInGaAs2() const;
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

short svcsigcommonheader::externalDataDarkD1() const
{
  return _externalDataDark[ 0 ];
}

short svcsigcommonheader::externalDataDarkD2() const
{
  return _externalDataDark[ 1 ];
}

short svcsigcommonheader::externalDataDarkD3() const
{
  return _externalDataDark[ 2 ];
}

short svcsigcommonheader::externalDataDarkD4() const
{
  return _externalDataDark[ 3 ];
}

short svcsigcommonheader::externalDataDarkD5() const
{
  return _externalDataDark[ 4 ];
}

short svcsigcommonheader::externalDataDarkD6() const
{
  return _externalDataDark[ 5 ];
}

short svcsigcommonheader::externalDataDarkD7() const
{
  return _externalDataDark[ 6 ];
}

short svcsigcommonheader::externalDataDarkD8() const
{
  return _externalDataDark[ 7 ];
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

float svcsigcommonheader::factorsSi() const
{
  return _factors[ 0 ];
}

float svcsigcommonheader::factorsInGaAs1() const
{
  return _factors[ 1 ];
}

float svcsigcommonheader::factorsInGaAs2() const
{
  return _factors[ 2 ];
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

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD1( const short &externalDataDarkD1 )
{
  _externalDataDark[ 0 ] = externalDataDarkD1;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD2( const short &externalDataDarkD2 )
{
  _externalDataDark[ 1 ] = externalDataDarkD2;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD3( const short &externalDataDarkD3 )
{
  _externalDataDark[ 2 ] = externalDataDarkD3;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD4( const short &externalDataDarkD4 )
{
  _externalDataDark[ 3 ] = externalDataDarkD4;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD5( const short &externalDataDarkD5 )
{
  _externalDataDark[ 4 ] = externalDataDarkD5;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD6( const short &externalDataDarkD6 )
{
  _externalDataDark[ 5 ] = externalDataDarkD6;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD7( const short &externalDataDarkD7 )
{
  _externalDataDark[ 6 ] = externalDataDarkD7;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateExternalDataDarkD8( const short &externalDataDarkD8 )
{
  _externalDataDark[ 7 ] = externalDataDarkD8;
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

svcsigcommonheader& svcsigcommonheader::updateFactorsSi( const float &factorsSi )
{
  _factors[ 0 ] = factorsSi;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateFactorsInGaAs1( const float &factorsInGaAs1 )
{
  _factors[ 1 ] = factorsInGaAs1;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateFactorsInGaAs2( const float &factorsInGaAs2 )
{
  _factors[ 2 ] = factorsInGaAs2;
  return *this;
}

svcsigcommonheader& svcsigcommonheader::updateFactorsComment( const std::string &factorsComment )
{
  _factorsComment = factorsComment;
  return *this;
}
#endif // __svcsigcommonheader_h_