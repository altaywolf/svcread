/*******************************************************************************
 * svcsig.h
 * 
 * DESCRIPTION:
 *    Contains the header (and implimentation) of the svcsig class
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

#ifndef __svcsig_h_
#define __svcsig_h_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "svcsighelper.h"
#include "svcsigspectraheader.h"
#include "svcsigcommonheader.h"


class svcsig {
private:
  svcsigcommonheader _commonHeader;
  svcsigspectraheader _referenceHeader;
  svcsigspectraheader _targetHeader;
  std::vector<float> _wavelength;
  std::vector<float> _referenceRadiance;
  std::vector<float> _targetRadiance;
  std::vector<float> _targetReflectance;
  
public:
  svcsig();
  svcsig( const svcsig &other );
  ~svcsig();

  svcsig& read( const std::string &filename );
  
  // accessors
  svcsigcommonheader commonHeader() const;
  svcsigspectraheader referenceHeader() const;
  svcsigspectraheader targetHeader() const;
  std::vector<float> wavelength() const;
  std::vector<float> referenceRadiance() const;
  std::vector<float> targetRadiance() const;
  std::vector<float> targetReflectance() const;
  
  // from the common header
  std::string name() const;
  std::string instrument() const;
  short * externalDataDark() const;
  char externalDataMask() const;
  std::string comm() const;
  float * factors() const;
  std::string factorsComment() const;
  
  // from the reference header
  float * referenceIntegration() const;
  std::string referenceScanMethod() const;
  float * referenceScanCoadds() const;
  float referenceScanTime() const;
  std::string referenceScanSettings() const;
  short * referenceExternalDataSet1() const;
  short * referenceExternalDataSet2() const;
  std::string referenceOptic() const;
  float * referenceTemp() const;
  float referenceBattery() const;
  unsigned int referenceError() const;
  std::string referenceUnits() const;
  std::string referenceTime() const;
  std::string referenceLongitude() const;
  std::string referenceLatitude() const;
  float referenceGpstime() const;
  unsigned int referenceMemorySlot() const;
  
  // from the target header
  float* targetIntegration() const;
  std::string targetScanMethod() const;
  float* targetScanCoadds() const;
  float targetScanTime() const;
  std::string targetScanSettings() const;
  short * targetExternalDataSet1() const;
  short * targetExternalDataSet2() const;
  std::string targetOptic() const;
  float* targetTemp() const;
  float targetBattery() const;
  unsigned int targetError() const;
  std::string targetUnits() const;
  std::string targetTime() const;
  std::string targetLongitude() const;
  std::string targetLatitude() const;
  float targetGpstime() const;
  unsigned int targetMemorySlot() const;
 
  void display() const;
  void displayCommonHeader() const;
  void displayReferenceHeader() const;
  void displayTargetHeader() const;
  void displayHeaders() const;
};

// --- --- Implimentation --- --- //

// -- -- Constructors -- -- //
svcsig::svcsig()
{
  _commonHeader = svcsigcommonheader();
  _referenceHeader = svcsigspectraheader();
  _targetHeader = svcsigspectraheader();
  _wavelength.clear();
  _referenceRadiance.clear();
  _targetRadiance.clear();
  _targetReflectance.clear();
}

svcsig::svcsig( const svcsig &other )
{
  _commonHeader = other._commonHeader;
  _referenceHeader = other._referenceHeader;
  _targetHeader = other._targetHeader;
  _wavelength = other._wavelength;
  _referenceRadiance = other._referenceRadiance;
  _targetRadiance = other._targetRadiance;
  _targetReflectance = other._targetReflectance;
}

// -- -- Destructor -- -- //
svcsig::~svcsig()
{
  // Nothing to do!
}

// -- -- Read -- -- //
svcsig& svcsig::read( const std::string &filename )
{
  std::ifstream input;
  input.open( filename.c_str() );
  
  
  try {
    // I need error checking & Exceptions here
    
    std::string line;
    std::string part;
    float fltarr3[ 3 ];
    short shortarr8[ 8 ];
    int i;
    
    
    // Check that the header line is good
    getline( input, line );
    svcSigRemoveWhitespace( line );
    if ( line.compare( "/*** Spectra Vista SIG Data ***/" ) != 0 ) {
      throw notSvcSigFile();
    }
    
    bool readHeader( true );
  
    while ( readHeader && input.good() ) {
      
      getline( input, line );
      svcSigParseHeaderEquals( line, part );
      
      if ( part.compare( "data" ) == 0 ) {
        readHeader = false;
      }
      else if ( part.compare( "name" ) == 0 ) {
        _commonHeader.updateName( line );
      }
      else if ( part.compare( "instrument" ) == 0 ) {
        _commonHeader.updateInstrument( line );
      }
      else if ( part.compare( "integration" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 2 ] = atof( part.c_str() );
        _referenceHeader.updateIntegration( fltarr3 );
        
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        fltarr3[ 2 ] = atof( line.c_str() );
        _targetHeader.updateIntegration( fltarr3 );
      }
      else if ( part.compare( "scan method" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanMethod( part );
        _targetHeader.updateScanMethod( line );
      }
      else if ( part.compare( "scan coadds" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 2 ] = atof( part.c_str() );
        _referenceHeader.updateScanCoadds( fltarr3 );
        
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        fltarr3[ 2 ] = atof( line.c_str() );
        _targetHeader.updateScanCoadds( fltarr3 );
      }
      else if ( part.compare( "scan time" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanTime( atof( part.c_str() ) );
        _targetHeader.updateScanTime( atof( line.c_str() ) );
      }
      else if ( part.compare( "scan settings" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanSettings( part );
        _targetHeader.updateScanSettings( line );
      }
      else if ( part.compare( "external data set1" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        shortarr8[ 0 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 1 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 2 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 3 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 4 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 5 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 6 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 7 ] = atoi( part.c_str() );
        _referenceHeader.updateExternalDataSet1( shortarr8 );
        
        svcSigParseHeaderComma( line, part );
        shortarr8[ 0 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 1 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 2 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 3 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 4 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 5 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 6 ] = atoi( part.c_str() );
        shortarr8[ 7 ] = atoi( line.c_str() );
        _targetHeader.updateExternalDataSet1( shortarr8 );
      }
      else if ( part.compare( "external data set2" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        shortarr8[ 0 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 1 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 2 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 3 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 4 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 5 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 6 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 7 ] = atoi( part.c_str() );
        _referenceHeader.updateExternalDataSet2( shortarr8 );
        
        svcSigParseHeaderComma( line, part );
        shortarr8[ 0 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 1 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 2 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 3 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 4 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 5 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 6 ] = atoi( part.c_str() );
        shortarr8[ 7 ] = atoi( line.c_str() );
        _targetHeader.updateExternalDataSet2( shortarr8 );
      }
      else if ( part.compare( "external data dark" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        shortarr8[ 0 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 1 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 2 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 3 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 4 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 5 ] = atoi( part.c_str() );
        svcSigParseHeaderComma( line, part );
        shortarr8[ 6 ] = atoi( part.c_str() );
        shortarr8[ 7 ] = atoi( part.c_str() );
        _commonHeader.updateExternalDataDark( shortarr8 );
      }
      else if ( part.compare( "external data mask" ) == 0 ) {
        _commonHeader.updateExternalDataMask( atoi( line.c_str() ) );
      }
      else if ( part.compare( "optic" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateOptic( part );
        _targetHeader.updateOptic( line );
      }
      else if ( part.compare( "temp" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 2 ] = atof( part.c_str() );
        _referenceHeader.updateTemp( fltarr3 );
        
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        fltarr3[ 2 ] = atof( line.c_str() );
        _targetHeader.updateTemp( fltarr3 );
      }
      else if ( part.compare( "battery" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateBattery( atof( part.c_str() ) );
        _targetHeader.updateBattery( atof( line.c_str() ) );
      }
      else if ( part.compare( "error" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateError( atoi( part.c_str() ) );
        _targetHeader.updateError( atoi( line.c_str() ) );
      }
      else if ( part.compare( "units" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateUnits( part );
        _targetHeader.updateUnits( line );
      }
      else if ( part.compare( "time" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateTime( part );
        _targetHeader.updateTime( line );
      }
      else if ( part.compare( "longitude" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateLongitude( part );
        _targetHeader.updateLongitude( line );
      }
      else if ( part.compare( "latitude" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateLatitude( part );
        _targetHeader.updateLatitude( line );
      }
      else if ( part.compare( "gpstime" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateGpsTime( atof( part.c_str() ) );
        _targetHeader.updateGpsTime( atof( line.c_str() ) );
      }
      else if ( part.compare( "comm" ) == 0 ) {
        _commonHeader.updateComm( line );
      }
      else if ( part.compare( "memory slot" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateMemorySlot( atoi( part.c_str() ) );
        _targetHeader.updateMemorySlot( atoi( line.c_str() ) );
      }
      else if ( part.compare( "factors" ) == 0 ) {
        svcSigParseHeaderSquareBraces( line, part );
        _commonHeader.updateFactorsComment( part );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 0 ] = atof( part.c_str() );
        svcSigParseHeaderComma( line, part );
        fltarr3[ 1 ] = atof( part.c_str() );
        fltarr3[ 2 ] = atof( line.c_str() );
        _commonHeader.updateFactors( fltarr3 );
      }
      else {
        std::cerr << "Unkown key: '" << part << "'." << std::endl;
      }
    } // end read header
    
    // read the data
    float wl, refRad, tarRad, tarRef;
    while ( input.good() ) {
      getline( input, line );
      svcSigParseData( line, wl, refRad, tarRad, tarRef );
      _wavelength.push_back( wl );
      _referenceRadiance.push_back( refRad );
      _targetRadiance.push_back( tarRad );
      _targetReflectance.push_back( tarRef );
    }
    
  } catch ( notSvcSigFile &e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Failed to complete read" << std::endl;
    input.close();
    return *this;
  } catch ( invalidSVCsigHeader &e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Failed to complete read" << std::endl;
    input.close();
    return *this;
  }
  
  input.close();
  return *this;
}

// -- -- Accessors -- -- //
svcsigcommonheader svcsig::commonHeader() const
{
  return _commonHeader;
}

svcsigspectraheader svcsig::referenceHeader() const
{
  return _referenceHeader;
}

svcsigspectraheader svcsig::targetHeader() const
{
  return _targetHeader;
}

std::vector<float> svcsig::wavelength() const
{
  return _wavelength;
}

std::vector<float> svcsig::referenceRadiance() const
{
  return _referenceRadiance;
}

std::vector<float> svcsig::targetRadiance() const
{
  return _targetRadiance;
}

std::vector<float> svcsig::targetReflectance() const
{
  return _targetReflectance;
}

std::string svcsig::name() const
{
  return _commonHeader.name();
}

std::string svcsig::instrument() const
{
  return _commonHeader.instrument();
}

short * svcsig::externalDataDark() const
{
  return _commonHeader.externalDataDark();
}

char svcsig::externalDataMask() const
{
  return _commonHeader.externalDataMask();
}

std::string svcsig::comm() const
{
  return _commonHeader.comm();
}

float * svcsig::factors() const
{
  return _commonHeader.factors();
}

std::string svcsig::factorsComment() const
{
  return _commonHeader.factorsComment();
}

float* svcsig::referenceIntegration() const
{
  return _referenceHeader.integration();
}

std::string svcsig::referenceScanMethod() const
{
  return _referenceHeader.scanMethod();
}

float * svcsig::referenceScanCoadds() const
{
  return _referenceHeader.scanCoadds();
}

float svcsig::referenceScanTime() const
{
  return _referenceHeader.scanTime();
}

std::string svcsig::referenceScanSettings() const
{
  return _referenceHeader.scanSettings();
}

short * svcsig::referenceExternalDataSet1() const
{
  return _referenceHeader.externalDataSet1();
}

short * svcsig::referenceExternalDataSet2() const
{
  return _referenceHeader.externalDataSet2();
}

std::string svcsig::referenceOptic() const
{
  return _referenceHeader.optic();
}

float * svcsig::referenceTemp() const
{
  return _referenceHeader.temp();
}

float svcsig::referenceBattery() const
{
  return _referenceHeader.battery();
}

unsigned int svcsig::referenceError() const
{
  return _referenceHeader.error();
}

std::string svcsig::referenceUnits() const
{
  return _referenceHeader.units();
}

std::string svcsig::referenceTime() const
{
  return _referenceHeader.time();
}

std::string svcsig::referenceLongitude() const
{
  return _referenceHeader.longitude();
}

std::string svcsig::referenceLatitude() const
{
  return _referenceHeader.latitude();
}

float svcsig::referenceGpstime() const
{
  return _referenceHeader.gpstime();
}

unsigned int svcsig::referenceMemorySlot() const
{
  return _referenceHeader.memorySlot();
}

float* svcsig::targetIntegration() const
{
  return _targetHeader.integration();
}

std::string svcsig::targetScanMethod() const
{
  return _targetHeader.scanMethod();
}

float* svcsig::targetScanCoadds() const
{
  return _targetHeader.scanCoadds();
}

float svcsig::targetScanTime() const
{
  return _targetHeader.scanTime();
}

std::string svcsig::targetScanSettings() const
{
  return _targetHeader.scanSettings();
}

short * svcsig::targetExternalDataSet1() const
{
  return _targetHeader.externalDataSet1();
}

short * svcsig::targetExternalDataSet2() const
{
  return _targetHeader.externalDataSet2();
}

std::string svcsig::targetOptic() const
{
  return _targetHeader.optic();
}

float* svcsig::targetTemp() const
{
  return _targetHeader.temp();
}

float svcsig::targetBattery() const
{
  return _targetHeader.battery();
}

unsigned int svcsig::targetError() const
{
  return _targetHeader.error();
}

std::string svcsig::targetUnits() const
{
  return _targetHeader.units();
}

std::string svcsig::targetTime() const
{
  return _targetHeader.time();
}

std::string svcsig::targetLongitude() const
{
  return _targetHeader.longitude();
}

std::string svcsig::targetLatitude() const
{
  return _targetHeader.latitude();
}

float svcsig::targetGpstime() const
{
  return _targetHeader.gpstime();
}

unsigned int svcsig::targetMemorySlot() const
{
  return _targetHeader.memorySlot();
}

// -- -- Display -- -- //
void svcsig::display() const
{
  displayHeaders();
  std::cout << std::endl << std::endl;
  std::cout << "Wavelength  Reference   Target      Reflectance" << std::endl;
  for ( int i = 0; i < _wavelength.size(); i++ ) {
    std::cout << std::setw( 12 ) << _wavelength.at( i ) << std::setw( 12 ) << _referenceRadiance.at( i ) << std::setw( 12 ) << _targetRadiance.at( i ) << std::setw( 12 ) << _targetReflectance.at( i ) << std::endl;
  }
}

void svcsig::displayCommonHeader() const
{
  _commonHeader.display();
}

void svcsig::displayReferenceHeader() const
{
  _referenceHeader.display( "Reference" );
}

void svcsig::displayTargetHeader() const
{
  _targetHeader.display( "Target" );
}

void svcsig::displayHeaders() const
{
  displayCommonHeader();
  std::cout << std::endl;
  displayReferenceHeader();
  std::cout << std::endl;
  displayTargetHeader();
}

#endif //__svcsig_h_