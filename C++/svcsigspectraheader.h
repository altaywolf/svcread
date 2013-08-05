/*******************************************************************************
 * svcsigspectraheader.h
 *
 * DESCRIPTION:
 *    Provides header and implimentaiton for the svcsigspectraheader class
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

#ifndef __svcsigspectraheader_h_
#define __svcsigspectraheader_h_

#include <string>
#include <stdexcept>

#include "svcsighelper.h"

class svcsig;

class svcsigspectraheader {
private:
  float _integration[ 3 ];
  std::string _scanMethod;
  float _scanCoadds[ 3 ];
  float _scanTime;
  std::string _scanSettings;
  short _externalDataSet1[ 8 ];
  short _externalDataSet2[ 8 ];
  std::string _optic;
  float _temp[ 3 ];
  float _battery;
  unsigned int _error;
  std::string _units;
  std::string _time;
  std::string _longitude;
  std::string _latitude;
  float _gpstime;
  unsigned int _memorySlot;
protected:
  svcsigspectraheader& updateIntegration( float integration[ 3 ] );
  svcsigspectraheader& updateScanMethod( const std::string &scanMethod );
  svcsigspectraheader& updateScanCoadds( float scanCoadds[ 3 ] );
  svcsigspectraheader& updateScanTime( const float &scanTime );
  svcsigspectraheader& updateScanSettings( const std::string &scanSettings );
  svcsigspectraheader& updateExternalDataSet1( short externalDataSet1[ 8 ] );
  svcsigspectraheader& updateExternalDataSet2( short externalDataSet2[ 8 ] );
  svcsigspectraheader& updateOptic( const std::string &optic );
  svcsigspectraheader& updateTemp( float temp[ 3 ] );
  svcsigspectraheader& updateBattery( const float &battery );
  svcsigspectraheader& updateError( const unsigned int &error );
  svcsigspectraheader& updateUnits( const std::string &units );
  svcsigspectraheader& updateTime( const std::string &time );
  svcsigspectraheader& updateLongitude( const std::string &longitude );
  svcsigspectraheader& updateLatitude( const std::string &latitude );
  svcsigspectraheader& updateGpsTime( const float &gpstime );
  svcsigspectraheader& updateMemorySlot( const unsigned int &memorySlot );
  
  void display( const std::string &type ) const;
public:
  /* Name: svcsigspectraheader()
   *
   * Description:	Null constructor for svcsigspectraheader
   * Arguments:
   * Modifies:
   * Returns:	A Null svcsigspectraheader object
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is not intented to be used on its own, but as a part of the svcsig class
   */
  svcsigspectraheader();
  
  /* Name: svcsigspectraheader( const svcsigspectraheader &other )
   *
   * Description:	Copy constructor for svcsigspectraheader
   * Arguments:
   * Modifies:
   * Returns:	A copied svcsigspectraheader object
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is not intented to be used on its own, but as a part of the svcsig class
   */
  svcsigspectraheader( const svcsigspectraheader &other );
  
  /* Name: ~svcsigspectraheader()
   *
   * Description:	Destructor for svcsigspectraheader
   * Arguments:
   * Modifies:
   * Returns:	A copied svcsigspectraheader object
   * Pre:
   * Post:
   * Exceptions:
   * Notes: There is no code in its implimentation
   */
  ~svcsigspectraheader();
  
  
  void display() const;
  
  float* integration() const;
  std::string scanMethod() const;
  float* scanCoadds() const;
  float scanTime() const;
  std::string scanSettings() const;
  short* externalDataSet1() const;
  short* externalDataSet2() const;
  std::string optic() const;
  float* temp() const;
  float battery() const;
  unsigned int error() const;
  std::string units() const;
  std::string time() const;
  std::string longitude() const;
  std::string latitude() const;
  float gpstime() const;
  unsigned int memorySlot() const;
  
  friend class svcsig;
};

// --- --- Implimentation --- --- //

// -- -- Constructors -- -- //
svcsigspectraheader::svcsigspectraheader()
{
  int i;
  for ( i = 0; i < 3; i++ ) {
    _integration[ i ] = 0.0;
    _scanCoadds[ i ] = 0.0;
    _temp[ i ] = 0.0;
  }
  _scanMethod = "";
  _scanTime = 0.0;
  _scanSettings = "";
  for ( i = 0; i < 8; i++ ) {
    _externalDataSet1[ i ] = 0;
    _externalDataSet2[ i ] = 0;
  }
  _optic = "";
  _battery = 0.0;
  _error = 0;
  _units = "";
  _time = "";
  _longitude = "";
  _latitude = "";
  _gpstime = 0;
  _memorySlot = 0;
}

svcsigspectraheader::svcsigspectraheader( const svcsigspectraheader &other )
{
  int i;
  for ( i = 0; i < 3; i++ ) {
    _integration[ i ] = other._integration[ i ];
    _scanCoadds[ i ] = other._scanCoadds[ i ];
    _temp[ i ] = other._temp[ i ];
  }
  _scanMethod = other._scanMethod;
  _scanTime = other._scanTime;
  _scanSettings = other._scanSettings;
  for ( i = 0; i < 8; i++ ) {
    _externalDataSet1[ i ] = other._externalDataSet1[ i ];
    _externalDataSet2[ i ] = other._externalDataSet2[ i ];
  }
  _optic = other._optic;
  _battery = other._battery;
  _error = other._error;
  _units = other._units;
  _time = other._time;
  _longitude = other._longitude;
  _latitude = other._latitude;
  _gpstime = other._gpstime;
  _memorySlot = other._memorySlot;
}

// -- -- destructor -- -- //
svcsigspectraheader::~svcsigspectraheader()
{
  // nothing to do!
}


// -- -- display -- -- //
void svcsigspectraheader::display() const
{
  display( "Undefined" );
}

void svcsigspectraheader::display( const std::string &type ) const
{
  int i;
  std::cout << type << " Spectra Header: " << std::endl;
  std::cout << "\tIntegration:";
  for ( i = 0; i < 3; i++ ) {
    std::cout << " " << _integration[ i ];
    if ( i < 2 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tScan Method: " << _scanMethod << std::endl;
  std::cout << "\tScan Coadds:";
  for ( i = 0; i < 3; i++ ) {
    std::cout << " " << _scanCoadds[ i ];
    if ( i < 2 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tScan Time: " << _scanTime << std::endl;
  std::cout << "\tScan Settings: " << _scanSettings << std::endl;
  std::cout << "\tExternal Data Set 1:";
  for ( i = 0; i < 8; i++ ) {
    std::cout << " " << _externalDataSet1[ i ];
    if ( i < 7 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tExternal Data Set 2:";
  for ( i = 0; i < 8; i++ ) {
    std::cout << " " << _externalDataSet2[ i ];
    if ( i < 7 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tOptic: " << _optic << std::endl;
  std::cout << "\tTemp:";
  for ( i = 0; i < 3; i++ ) {
    std::cout << " " << _temp[ i ];
    if ( i < 2 ) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  std::cout << "\tBattery: " << _battery << std::endl;
  std::cout << "\tError: " << _error << std::endl;
  std::cout << "\tUnits: " << _units << std::endl;
  std::cout << "\tTime: " << _time << std::endl;
  std::cout << "\tLongitude: " << _longitude << std::endl;
  std::cout << "\tLatitude: " << _latitude << std::endl;
  std::cout << "\tGPS Time: " << _gpstime << std::endl;
  std::cout << "\tMemory Slot: " << _memorySlot << std::endl;
  std::cout << "End of " << type << " Spectra Header" << std::endl;
}

// -- -- Getters -- -- //

float* svcsigspectraheader::integration() const
{
  float * tmp = new float[3];
  for ( int i = 0; i < 3; i++ ) {
    tmp[ i ] = _integration[ i ];
  }
  return tmp;
}

std::string svcsigspectraheader::scanMethod() const
{
  return _scanMethod;
}

float* svcsigspectraheader::scanCoadds() const
{
  float * tmp = new float[3];
  for ( int i = 0; i < 3; i++ ) {
    tmp[ i ] = _scanCoadds[ i ];
  }
  return tmp;
}

float svcsigspectraheader::scanTime() const
{
  return _scanTime;
}

std::string svcsigspectraheader::scanSettings() const
{
  return _scanSettings;
}

short * svcsigspectraheader::externalDataSet1() const
{
  short * tmp = new short[8];
  for ( int i = 0; i < 8; i++ ) {
    tmp[ i ] = _externalDataSet2[ i ];
  }
  return tmp;
}

short * svcsigspectraheader::externalDataSet2() const
{
  short * tmp = new short[8];
  for ( int i = 0; i < 8; i++ ) {
    tmp[ i ] = _externalDataSet2[ i ];
  }
  return tmp;
}

std::string svcsigspectraheader::optic() const
{
  return _optic;
}

float* svcsigspectraheader::temp() const
{
  float * tmp = new float[3];
  for ( int i = 0; i < 3; i++ ) {
    tmp[ i ] = _temp[ i ];
  }
  return tmp;
}

float svcsigspectraheader::battery() const
{
  return _battery;
}

unsigned int svcsigspectraheader::error() const
{
  return _battery;
}

std::string svcsigspectraheader::units() const
{
  return _units;
}

std::string svcsigspectraheader::time() const
{
  return _time;
}

std::string svcsigspectraheader::longitude() const
{
  return _longitude;
}

std::string svcsigspectraheader::latitude() const
{
  return _latitude;
}

float svcsigspectraheader::gpstime() const
{
  return _gpstime;
}

unsigned int svcsigspectraheader::memorySlot() const
{
  return _memorySlot;
}

// -- -- Updaters -- -- //
svcsigspectraheader& svcsigspectraheader::updateIntegration( float integration[ 3 ] )
{
  for ( int i = 0; i < 3; i++ ) {
    _integration[ i ] = integration[ i ];
  }
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateScanMethod( const std::string &scanMethod )
{
  _scanMethod = scanMethod;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateScanCoadds( float scanCoadds[ 3 ] )
{
  for ( int i = 0; i < 3; i++ ) {
    _scanCoadds[ i ] = scanCoadds[ i ];
  }
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateScanTime( const float &scanTime )
{
  _scanTime = scanTime;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateScanSettings( const std::string &scanSettings )
{
  _scanSettings = scanSettings;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateExternalDataSet1( short externalDataSet1[ 8 ] )
{
  for ( int i = 0; i < 8; i++ ) {
    _externalDataSet1[ i ] = externalDataSet1[ i ];
  }
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateExternalDataSet2( short externalDataSet2[ 8 ] )
{
  for ( int i = 0; i < 8; i++ ) {
    _externalDataSet2[ i ] = externalDataSet2[ i ];
  }
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateOptic( const std::string &optic )
{
  _optic = optic;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateTemp( float temp[ 3 ] )
{
  for ( int i = 0; i < 3; i++ ) {
    _temp[ i ] = temp[ i ];
  }
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateBattery( const float &battery )
{
  _battery = battery;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateError( const unsigned int &error )
{
  _error = error;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateUnits( const std::string &units )
{
  _units = units;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateTime( const std::string &time )
{
  _time = time;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateLongitude( const std::string &longitude )
{
  _longitude = longitude;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateLatitude( const std::string &latitude )
{
  _latitude = latitude;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateGpsTime( const float &gpstime )
{
  _gpstime = gpstime;
  return *this;
}

svcsigspectraheader& svcsigspectraheader::updateMemorySlot( const unsigned int &memorySlot )
{
  _memorySlot = memorySlot;
  return *this;
}



#endif // __svcsigspectraheader_h_