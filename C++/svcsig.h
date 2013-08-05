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
  // Private variables
  svcsigcommonheader _commonHeader; // the part of the header that is common to all spectra
  svcsigspectraheader _referenceHeader; // the part of the header that belongs to the reference spectra
  svcsigspectraheader _targetHeader; // the part of the header that belongs to the target spectra
  std::vector<float> _wavelength; // the wavelengths
  std::vector<float> _referenceRadiance; // the refernce radiance spectrum
  std::vector<float> _targetRadiance; // the target radiance spectrum
  std::vector<float> _targetReflectance; // the target reflectance spectrum
  
  // private functions ( used for reading )
  /* Name: void svcSigRemoveWhitespace( std::string &s )
   *
   * Description:	Removes leading and trailing whitespace (spaces, tabs, new lines, and carage returns) from a string
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
  
  /* Name: svcSigParseHeaderEquals( std::string &s, std::string &p )
   *
   * Description:	Splits a header string into the right and left side of the equals
   * Arguments: std::string &s: the string (rhs of equals)
   *            std::string &p: the key (lhs of equals)
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
  
  /* Name: svcSigParseHeaderComma( std::string &s, std::string &p )
   *
   * Description:	Splits a header string into the right and left side of a comma
   * Arguments: std::string &s: the string (rhs of comma)
   *            std::string &p: the key (lhs of comma)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain a comma
   * Notes:
   */
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
  
  /* Name: svcSigParseHeaderSquareBraces( std::string &s, std::string &p )
   *
   * Description:	Splits a header string into parts inside square braces and outside of them
   * Arguments: std::string &s: the string (the part outside square braces)
   *            std::string &p: the key (the part inside the square braces)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
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
  
  /* Name: svcSigParseWhitespace( std::string &s, std::string &p )
   *
   * Description:	Splits a header string into the right and left side of whitespace (space or tab)
   * Arguments: std::string &s: the string (rhs of whitespace)
   *            std::string &p: the key (lhs of whitespace)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain whitespace
   * Notes:
   */
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
  
  /* Name: svcSigParseData( std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef )
   *
   * Description:	Splits a data line into wavelength, refernce radiance, target radiance, and target reflectancs
   * Arguments: std::string &s: the string of data (whitespace separated)
   *            float &wl: the wavelength
   *            float &refRad: the reference radiance
   *            float &tarRad: the target radince
   *            float &tarRef: the target reflectance
   * Modifies: std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain whitespace
   * Notes:
   */
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
  
  // from the reference header
  float * referenceIntegration() const;
  float referenceIntegrationSi() const;
  float referenceIntegrationInGaAs1() const;
  float referenceIntegrationInGaAs2() const;
  std::string referenceScanMethod() const;
  float * referenceScanCoadds() const;
  float referenceScanCoaddsSi() const;
  float referenceScanCoaddsInGaAs1() const;
  float referenceScanCoaddsInGaAs2() const;
  float referenceScanTime() const;
  std::string referenceScanSettings() const;
  short * referenceExternalDataSet1() const;
  short referenceExternalDataSet1R1() const;
  short referenceExternalDataSet1R2() const;
  short referenceExternalDataSet1R3() const;
  short referenceExternalDataSet1R4() const;
  short referenceExternalDataSet1R5() const;
  short referenceExternalDataSet1R6() const;
  short referenceExternalDataSet1R7() const;
  short referenceExternalDataSet1R8() const;
  short * referenceExternalDataSet2() const;
  short referenceExternalDataSet2R1() const;
  short referenceExternalDataSet2R2() const;
  short referenceExternalDataSet2R3() const;
  short referenceExternalDataSet2R4() const;
  short referenceExternalDataSet2R5() const;
  short referenceExternalDataSet2R6() const;
  short referenceExternalDataSet2R7() const;
  short referenceExternalDataSet2R8() const;
  std::string referenceOptic() const;
  float * referenceTemp() const;
  float referenceTempSi() const;
  float referenceTempInGaAs1() const;
  float referenceTempInGaAs2() const;
  float referenceBattery() const;
  unsigned int referenceError() const;
  std::string referenceUnits() const;
  std::string referenceTime() const;
  std::string referenceLongitude() const;
  float referenceDecimalLongitude() const;
  std::string referenceLatitude() const;
  float referenceDecimalLatitude() const;
  float referenceGpstime() const;
  unsigned int referenceMemorySlot() const;
  
  // from the target header
  // from the target header
  float * targetIntegration() const;
  float targetIntegrationSi() const;
  float targetIntegrationInGaAs1() const;
  float targetIntegrationInGaAs2() const;
  std::string targetScanMethod() const;
  float * targetScanCoadds() const;
  float targetScanCoaddsSi() const;
  float targetScanCoaddsInGaAs1() const;
  float targetScanCoaddsInGaAs2() const;
  float targetScanTime() const;
  std::string targetScanSettings() const;
  short * targetExternalDataSet1() const;
  short targetExternalDataSet1T1() const;
  short targetExternalDataSet1T2() const;
  short targetExternalDataSet1T3() const;
  short targetExternalDataSet1T4() const;
  short targetExternalDataSet1T5() const;
  short targetExternalDataSet1T6() const;
  short targetExternalDataSet1T7() const;
  short targetExternalDataSet1T8() const;
  short * targetExternalDataSet2() const;
  short targetExternalDataSet2T1() const;
  short targetExternalDataSet2T2() const;
  short targetExternalDataSet2T3() const;
  short targetExternalDataSet2T4() const;
  short targetExternalDataSet2T5() const;
  short targetExternalDataSet2T6() const;
  short targetExternalDataSet2T7() const;
  short targetExternalDataSet2T8() const;
  std::string targetOptic() const;
  float * targetTemp() const;
  float targetTempSi() const;
  float targetTempInGaAs1() const;
  float targetTempInGaAs2() const;
  float targetBattery() const;
  unsigned int targetError() const;
  std::string targetUnits() const;
  std::string targetTime() const;
  std::string targetLongitude() const;
  float targetDecimalLongitude() const;
  std::string targetLatitude() const;
  float targetDecimalLatitude() const;
  float targetGpstime() const;
  unsigned int targetMemorySlot() const;
 
  void display() const;
  void displayCommonHeader() const;
  void displayReferenceHeader() const;
  void displayTargetHeader() const;
  void displayHeaders() const;
  void displayData() const;
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
        _referenceHeader.updateIntegrationSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateIntegrationInGaAs1( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateIntegrationInGaAs2( atof( part.c_str() ) );
        
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateIntegrationSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateIntegrationInGaAs1( atof( part.c_str() ) );
        _targetHeader.updateIntegrationInGaAs2( atof( line.c_str() ) );
      }
      else if ( part.compare( "scan method" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanMethod( part );
        _targetHeader.updateScanMethod( line );
      }
      else if ( part.compare( "scan coadds" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanCoaddsSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanCoaddsInGaAs1( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateScanCoaddsInGaAs2( atof( part.c_str() ) );
        
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateScanCoaddsSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateScanCoaddsInGaAs1( atof( part.c_str() ) );
        _targetHeader.updateScanCoaddsInGaAs2( atof( line.c_str() ) );
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
        _referenceHeader.updateExternalDataSet1X1( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X2( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X3( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X4( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X5( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X6( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X7( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet1X8( atoi( part.c_str() ) );
        
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X1( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X2( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X3( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X4( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X5( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X6( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet1X7( atoi( part.c_str() ) );
        _targetHeader.updateExternalDataSet1X8( atoi( line.c_str() ) );
      }
      else if ( part.compare( "external data set2" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X1( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X2( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X3( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X4( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X5( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X6( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X7( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateExternalDataSet2X8( atoi( part.c_str() ) );
        
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X1( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X2( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X3( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X4( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X5( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X6( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateExternalDataSet2X7( atoi( part.c_str() ) );
        _targetHeader.updateExternalDataSet2X8( atoi( line.c_str() ) );
      }
      else if ( part.compare( "external data dark" ) == 0 ) {
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD1( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD2( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD3( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD4( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD5( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD6( atoi( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateExternalDataDarkD7( atoi( part.c_str() ) );
        _commonHeader.updateExternalDataDarkD8( atoi( line.c_str() ) );
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
        _referenceHeader.updateTempSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateTempInGaAs1( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _referenceHeader.updateTempInGaAs2( atof( part.c_str() ) );
        
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateTempSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _targetHeader.updateTempInGaAs1( atof( part.c_str() ) );
        _targetHeader.updateTempInGaAs2( atof( line.c_str() ) );
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
        _commonHeader.updateFactorsSi( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateFactorsInGaAs1( atof( part.c_str() ) );
        _commonHeader.updateFactorsInGaAs2( atof( line.c_str() ) );
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

short svcsig::externalDataDarkD1() const
{
  return _commonHeader.externalDataDarkD1();
}

short svcsig::externalDataDarkD2() const
{
  return _commonHeader.externalDataDarkD2();
}

short svcsig::externalDataDarkD3() const
{
  return _commonHeader.externalDataDarkD3();
}

short svcsig::externalDataDarkD4() const
{
  return _commonHeader.externalDataDarkD4();
}

short svcsig::externalDataDarkD5() const
{
  return _commonHeader.externalDataDarkD5();
}

short svcsig::externalDataDarkD6() const
{
  return _commonHeader.externalDataDarkD6();
}

short svcsig::externalDataDarkD7() const
{
  return _commonHeader.externalDataDarkD7();
}

short svcsig::externalDataDarkD8() const
{
  return _commonHeader.externalDataDarkD8();
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

float svcsig::factorsSi() const
{
  return _commonHeader.factorsSi();
}

float svcsig::factorsInGaAs1() const
{
  return _commonHeader.factorsInGaAs1();
}

float svcsig::factorsInGaAs2() const
{
  return _commonHeader.factorsInGaAs2();
}

std::string svcsig::factorsComment() const
{
  return _commonHeader.factorsComment();
}

float * svcsig::referenceIntegration() const
{
  return _referenceHeader.integration();
}

float svcsig::referenceIntegrationSi() const
{
  return _referenceHeader.integrationSi();
}

float svcsig::referenceIntegrationInGaAs1() const
{
  return _referenceHeader.integrationInGaAs1();
}

float svcsig::referenceIntegrationInGaAs2() const
{
  return _referenceHeader.integrationInGaAs2();
}

std::string svcsig::referenceScanMethod() const
{
  return _referenceHeader.scanMethod();
}

float * svcsig::referenceScanCoadds() const
{
  return _referenceHeader.scanCoadds();
}

float svcsig::referenceScanCoaddsSi() const
{
  return _referenceHeader.scanCoaddsSi();
}

float svcsig::referenceScanCoaddsInGaAs1() const
{
  return _referenceHeader.scanCoaddsInGaAs1();
}

float svcsig::referenceScanCoaddsInGaAs2() const
{
  return _referenceHeader.scanCoaddsInGaAs2();
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

short svcsig::referenceExternalDataSet1R1() const
{
  return _referenceHeader.externalDataSet1X1();
}

short svcsig::referenceExternalDataSet1R2() const
{
  return _referenceHeader.externalDataSet1X2();
}

short svcsig::referenceExternalDataSet1R3() const
{
  return _referenceHeader.externalDataSet1X3();
}

short svcsig::referenceExternalDataSet1R4() const
{
  return _referenceHeader.externalDataSet1X4();
}

short svcsig::referenceExternalDataSet1R5() const
{
  return _referenceHeader.externalDataSet1X5();
}

short svcsig::referenceExternalDataSet1R6() const
{
  return _referenceHeader.externalDataSet1X6();
}

short svcsig::referenceExternalDataSet1R7() const
{
  return _referenceHeader.externalDataSet1X7();
}

short svcsig::referenceExternalDataSet1R8() const
{
  return _referenceHeader.externalDataSet1X8();
}

short * svcsig::referenceExternalDataSet2() const
{
  return _referenceHeader.externalDataSet2();
}

short svcsig::referenceExternalDataSet2R1() const
{
  return _referenceHeader.externalDataSet2X1();
}

short svcsig::referenceExternalDataSet2R2() const
{
  return _referenceHeader.externalDataSet2X2();
}

short svcsig::referenceExternalDataSet2R3() const
{
  return _referenceHeader.externalDataSet2X3();
}

short svcsig::referenceExternalDataSet2R4() const
{
  return _referenceHeader.externalDataSet2X4();
}

short svcsig::referenceExternalDataSet2R5() const
{
  return _referenceHeader.externalDataSet2X5();
}

short svcsig::referenceExternalDataSet2R6() const
{
  return _referenceHeader.externalDataSet2X6();
}

short svcsig::referenceExternalDataSet2R7() const
{
  return _referenceHeader.externalDataSet2X7();
}

short svcsig::referenceExternalDataSet2R8() const
{
  return _referenceHeader.externalDataSet2X8();
}

std::string svcsig::referenceOptic() const
{
  return _referenceHeader.optic();
}

float * svcsig::referenceTemp() const
{
  return _referenceHeader.temp();
}

float svcsig::referenceTempSi() const
{
  return _referenceHeader.tempSi();
}

float svcsig::referenceTempInGaAs1() const
{
  return _referenceHeader.tempInGaAs1();
}

float svcsig::referenceTempInGaAs2() const
{
  return _referenceHeader.tempInGaAs2();
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

float svcsig::referenceDecimalLongitude() const
{
  return _referenceHeader.decimalLongitude();
}

std::string svcsig::referenceLatitude() const
{
  return _referenceHeader.latitude();
}

float svcsig::referenceDecimalLatitude() const
{
  return _referenceHeader.decimalLatitude();
}

float svcsig::referenceGpstime() const
{
  return _referenceHeader.gpstime();
}

unsigned int svcsig::referenceMemorySlot() const
{
  return _referenceHeader.memorySlot();
}

float * svcsig::targetIntegration() const
{
  return _targetHeader.integration();
}

float svcsig::targetIntegrationSi() const
{
  return _targetHeader.integrationSi();
}

float svcsig::targetIntegrationInGaAs1() const
{
  return _targetHeader.integrationInGaAs1();
}

float svcsig::targetIntegrationInGaAs2() const
{
  return _targetHeader.integrationInGaAs2();
}

std::string svcsig::targetScanMethod() const
{
  return _targetHeader.scanMethod();
}

float * svcsig::targetScanCoadds() const
{
  return _targetHeader.scanCoadds();
}

float svcsig::targetScanCoaddsSi() const
{
  return _targetHeader.scanCoaddsSi();
}

float svcsig::targetScanCoaddsInGaAs1() const
{
  return _targetHeader.scanCoaddsInGaAs1();
}

float svcsig::targetScanCoaddsInGaAs2() const
{
  return _targetHeader.scanCoaddsInGaAs2();
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

short svcsig::targetExternalDataSet1T1() const
{
  return _targetHeader.externalDataSet1X1();
}

short svcsig::targetExternalDataSet1T2() const
{
  return _targetHeader.externalDataSet1X2();
}

short svcsig::targetExternalDataSet1T3() const
{
  return _targetHeader.externalDataSet1X3();
}

short svcsig::targetExternalDataSet1T4() const
{
  return _targetHeader.externalDataSet1X4();
}

short svcsig::targetExternalDataSet1T5() const
{
  return _targetHeader.externalDataSet1X5();
}

short svcsig::targetExternalDataSet1T6() const
{
  return _targetHeader.externalDataSet1X6();
}

short svcsig::targetExternalDataSet1T7() const
{
  return _targetHeader.externalDataSet1X7();
}

short svcsig::targetExternalDataSet1T8() const
{
  return _targetHeader.externalDataSet1X8();
}

short * svcsig::targetExternalDataSet2() const
{
  return _targetHeader.externalDataSet2();
}

short svcsig::targetExternalDataSet2T1() const
{
  return _targetHeader.externalDataSet2X1();
}

short svcsig::targetExternalDataSet2T2() const
{
  return _targetHeader.externalDataSet2X2();
}

short svcsig::targetExternalDataSet2T3() const
{
  return _targetHeader.externalDataSet2X3();
}

short svcsig::targetExternalDataSet2T4() const
{
  return _targetHeader.externalDataSet2X4();
}

short svcsig::targetExternalDataSet2T5() const
{
  return _targetHeader.externalDataSet2X5();
}

short svcsig::targetExternalDataSet2T6() const
{
  return _targetHeader.externalDataSet2X6();
}

short svcsig::targetExternalDataSet2T7() const
{
  return _targetHeader.externalDataSet2X7();
}

short svcsig::targetExternalDataSet2T8() const
{
  return _targetHeader.externalDataSet2X8();
}

std::string svcsig::targetOptic() const
{
  return _targetHeader.optic();
}

float * svcsig::targetTemp() const
{
  return _targetHeader.temp();
}

float svcsig::targetTempSi() const
{
  return _targetHeader.tempSi();
}

float svcsig::targetTempInGaAs1() const
{
  return _targetHeader.tempInGaAs1();
}

float svcsig::targetTempInGaAs2() const
{
  return _targetHeader.tempInGaAs2();
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

float svcsig::targetDecimalLongitude() const
{
  return _targetHeader.decimalLongitude();
}

std::string svcsig::targetLatitude() const
{
  return _targetHeader.latitude();
}

float svcsig::targetDecimalLatitude() const
{
  return _targetHeader.decimalLatitude();
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
  displayData();
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

void svcsig::displayData() const
{
  std::cout << "Wavelength  Reference   Target      Reflectance" << std::endl;
  for ( int i = 0; i < _wavelength.size(); i++ ) {
    std::cout << std::fixed;
    std::cout.precision( 2 );
    std::cout << std::setw( 12 ) << _wavelength.at( i );
    std::cout.precision( 3 );
    std::cout << std::setw( 12 ) << _referenceRadiance.at( i );
    std::cout.precision( 3 );
    std::cout << std::setw( 12 ) << _targetRadiance.at( i );
    std::cout.precision( 3 );
    std::cout << std::setw( 12 ) << _targetReflectance.at( i ) << std::endl;
  }
}

#endif //__svcsig_h_