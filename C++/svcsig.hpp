/*******************************************************************************
 * svcsig.hpp
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

#ifndef __svcsig_hpp_
#define __svcsig_hpp_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "svcsighelper.hpp"
#include "svcsigspectraheader.hpp"
#include "svcsigcommonheader.hpp"


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
  
  
  /* Name: void svcSigRemoveWhitespace( std::string &s ) const
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
  void svcSigRemoveWhitespace( std::string &s ) const;
  
  /* Name: svcSigParseHeaderEquals( std::string &s, std::string &p ) const
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
  void svcSigParseHeaderEquals( std::string &s, std::string &p ) const;

  
  /* Name: svcSigParseHeaderComma( std::string &s, std::string &p ) const
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
  void svcSigParseHeaderComma( std::string &s, std::string &p ) const;
  
  /* Name: svcSigParseHeaderColon( std::string &s, std::string &p ) const
   *
   * Description:	Splits a header string into the right and left side of a colon
   * Arguments: std::string &s: the string (rhs of colon)
   *            std::string &p: the key (lhs of colon)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain a colon
   * Notes:
   */
  void svcSigParseHeaderColon( std::string &s, std::string &p ) const;

  
  /* Name: svcSigParseHeaderSquareBraces( std::string &s, std::string &p ) const
   *
   * Description:	Splits a header string into parts inside square braces and 
   *    outside of them
   * Arguments: std::string &s: the string (the part outside square braces)
   *            std::string &p: the key (the part inside the square braces)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void svcSigParseHeaderSquareBraces( std::string &s, std::string &p ) const;
  
  /* Name: svcSigParseHeaderParentheses( std::string &s, std::string &p ) const
   *
   * Description:	Splits a header string into parts inside parentheses and
   *    outside of them
   * Arguments: std::string &s: the string (the part outside the parentheses)
   *            std::string &p: the key (the part inside the parentheses)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void svcSigParseHeaderParentheses( std::string &s, std::string &p ) const;
  
  /* Name: svcSigParseWhitespace( std::string &s, std::string &p ) const
   *
   * Description:	Splits a header string into the right and left side of 
   *    whitespace (space or tab)
   * Arguments: std::string &s: the string (rhs of whitespace)
   *            std::string &p: the key (lhs of whitespace)
   * Modifies: std::string &s, std::string &p
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain whitespace
   * Notes:
   */
  void svcSigParseWhitespace( std::string &s, std::string &p ) const;
  
  
  /* Name: svcSigParseData( std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef ) const
   *
   * Description:	Splits a data line into wavelength, refernce radiance, target 
   *    radiance, and target reflectancs
   * Arguments: std::string &s: the string of data (whitespace separated)
   *            float &wl: the wavelength
   *            float &refRad: the reference radiance
   *            float &tarRad: the target radince
   *            float &tarRef: the target reflectance
   * Modifies: std::string &s, float &wl, float &refRad, float &tarRad, float
   *    &tarRef
   * Returns:
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigHeader if s does not contain whitespace
   * Notes:
   */
  void svcSigParseData( std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef ) const;

public:
  /* Name: svcsig::svcsig()
   *
   * Description:	svcsig null constructor
   * Arguments: 
   * Modifies: 
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes: All values set to 0 or ""
   */
  svcsig();
  
  /* Name: svcsig::svcsig( const svcsig &other )
   *
   * Description:	svcsig copy constructor
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  svcsig( const svcsig &other );
  
  /* Name: svcsig::~svcsig()
   *
   * Description:	svcsig destructor
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This doesn't need to do anything special
   */
  ~svcsig();

  /* Name: svcsig& svcsig::read( const std::string &filename )
   *
   * Description:	svcsig reader
   * Arguments: const std::string &filename: the filename of the SVC sig file to
   *     parse
   * Modifies: Everything!  !!!! I need to fill this in !!!!
   * Returns: *this
   * Pre:
   * Post:
   * Exceptions: !!!! I need to fill this in !!!! 
   * Notes: This is the "real" constructor
   */
  svcsig& read( const std::string &filename );
  
  /* Name: void svcsig::write( const std::string &filename ) const
   *
   * Description:	svcsig writer
   * Arguments: const std::string &filename: the filename of the SVC sig file to
   *     write
   * Modifies: 
   * Returns: 
   * Pre:
   * Post:
   * Exceptions: 
   * Notes: 
   */
  void write( const std::string &filename ) const;
  
  // accessors
  
  /* Name: svcsigcommonheader svcsig::commonHeader() const
   *
   * Description: Returns the common header
   * Arguments:
   * Modifies: 
   * Returns: svcsigcommonheader
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  svcsigcommonheader commonHeader() const;
  
  /* Name: svcsig::
   *
   * Description:
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  svcsigspectraheader referenceHeader() const;
  
  /* Name: svcsigspectraheader svcsig::referenceHeader() const
   *
   * Description: returns the reference spectra header
   * Arguments:
   * Modifies:
   * Returns: svcsigspectraheader
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  svcsigspectraheader targetHeader() const;
  
  /* Name: svcsigspectraheader svcsig::targetHeader() const
   *
   * Description: returns the target spectra header
   * Arguments:
   * Modifies:
   * Returns: svcsigspectraheader
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::vector<float> wavelength() const;
  
  /* Name: std::vector<float> svcsig::wavelength() const
   *
   * Description: returns the wavelengths
   * Arguments:
   * Modifies:
   * Returns: std::vector<float>
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::vector<float> referenceRadiance() const;
  
  /* Name: std::vector<float> svcsig::referenceRadiance() const
   *
   * Description: returns the reference radiance/irradiance
   * Arguments:
   * Modifies:
   * Returns: std::vector<float>
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::vector<float> targetRadiance() const;
  
  /* Name: std::vector<float> svcsig::targetRadiance() const
   *
   * Description: returns the target radiance/irradiance
   * Arguments:
   * Modifies:
   * Returns: std::vector<float>
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::vector<float> targetReflectance() const;
  
  // from the common header
  
  /* Name: std::string svcsig::name() const
   *
   * Description: returns the name from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string name() const;
  
  /* Name: std::string svcsig::instrument() const
   *
   * Description: returns the full instrument name from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string instrument() const;
  
  /* Name: std::string svcsig::instrumentModelNumber() const
   *
   * Description: returns the instrument model number from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string instrumentModelNumber() const;
  
  /* Name: std::string svcsig::instrumentExtendedSerialNumber() const
   *
   * Description: returns the instrument extended serial number from the common 
   *    header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This may change to an unsigned int or unsigned long in future releases
   */
  std::string instrumentExtendedSerialNumber() const;
  
  /* Name: std::string svcsig::instrumentCommonName() const
   *
   * Description: returns the instrument common name from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string instrumentCommonName() const;
  
  /* Name: short * svcsig:externalDataDark() const
   *
   * Description: returns the most recent dark data samples from the common header
   * Arguments:
   * Modifies:
   * Returns: short[ 8 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  short * externalDataDark() const;
  
  /* Name: short * svcsig:externalDataDarkD1() const
   *
   * Description: returns first element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD1() const;
  
  /* Name: short svcsig:externalDataDarkD2() const
   *
   * Description: returns second element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD2() const;
  
  /* Name: short svcsig:externalDataDarkD3() const
   *
   * Description: returns third element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD3() const;
  
  /* Name: short svcsig:externalDataDarkD4() const
   *
   * Description: returns fourth element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD4() const;
  
  /* Name: short svcsig:externalDataDarkD5() const
   *
   * Description: returns fifth element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD5() const;
  
  /* Name: short svcsig:externalDataDarkD6() const
   *
   * Description: returns sixth element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD6() const;
  
  /* Name: short svcsig:externalDataDarkD7() const
   *
   * Description: returns seventh element of the most recent dark data samples 
   *    from the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD7() const;
  
  /* Name: short svcsig:externalDataDarkD8() const
   *
   * Description: returns eigth element of the most recent dark data samples from
   *    the common header
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short externalDataDarkD8() const;
  
  /* Name: char svcsig:externalDataMask() const
   *
   * Description: returns mask with which channels have been activated. Each bit
   *    refers to a channel. 5 means channels 1 and 3 are enabled.
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  char externalDataMask() const;
  
  /* Name: std::string svcsig::comm() const
   *
   * Description: returns any user supplied commands from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string comm() const;
  
  /* Name: float * svcsig::factors() const
   *
   * Description: returns the factors used for matching the Si, InGaAs1, and 
   *    InGaAs2 detectors. The values are reference radiances, target radiance, 
   *    and reflectance matchingFactors
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * factors() const;
  
  /* Name: float svcsig::factorsReference() const
   *
   * Description: returns the reference factor used for matching the Si, InGaAs1, 
   *    and InGaAs2 detectors. 
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float factorsReference() const;
  
  /* Name: float svcsig::factorsTarget() const
   *
   * Description: returns the target factor used for matching the Si, InGaAs1, and
   *    InGaAs2 detectors.
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float factorsTarget() const;
  
  /* Name: float svcsig::factorsReflectance() const
   *
   * Description: returns the reflectance factor used for matching the Si, 
   *    InGaAs1, and InGaAs2 detectors.
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float factorsReflectance() const;
  
  /* Name: std::string svcsig::factorsComment() const
   *
   * Description: returns any comment about the factors from the common header
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string factorsComment() const;
  
  
  /* Name: float * svcsig::referenceIntegration() const
   *
   * Description: Returns the reference intergration time in ms of the Si, 
   *    InGaAs1, and InGaAs2 arrays for the target scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * referenceIntegration() const;
  
  /* Name: float svcsig::referenceIntegrationSi() const
   *
   * Description: Returns the reference intergration time in ms of the Si array 
   *    for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  float referenceIntegrationSi() const;
  
  /* Name: float svcsig::referenceIntegrationInGaAs1() const
   *
   * Description: Returns the reference intergration time in ms of the InGaAs1 
   *    array for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceIntegrationInGaAs1() const;
  
  /* Name: float svcsig::referenceIntegrationInGaAs2() const
   *
   * Description: Returns the reference intergration time in ms of the InGaAs2 
   *    array for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceIntegrationInGaAs2() const;
  
  /* Name: std::string svcsig::referenceScanMethod() const
   *
   * Description: Returns the reference scan method ("Time-based" or 
   *    "Coadd-based")
   * Arguments:
   * Modifies:
   * Returns: std::strint
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceScanMethod() const;
  
  /* Name: float * svcsig::referenceScanCoadds() const
   *
   * Description: Returns the scan coadds of the Si, InGaAs1, and InGaAs2 arrays
   *    for the reference scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * referenceScanCoadds() const;
  
  /* Name: float svcsig::referenceScanCoaddsSi() const
   *
   * Description: Returns the scan coadds of the Si array for the reference scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  float referenceScanCoaddsSi() const;
  
  /* Name: float svcsig::referenceScanCoaddsInGaAs1() const
   *
   * Description: Returns the scan coadds of the InGaAs1 array for the reference 
   *    scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceScanCoaddsInGaAs1() const;
  
  /* Name: float svcsig::referenceScanCoaddsInGaAs2() const
   *
   * Description: Returns the scan coadds of the InGaAs2 arrays for the reference
   *    scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceScanCoaddsInGaAs2() const;
  
  /* Name: float svcsig::referenceScanTime() const
   *
   * Description: Returns the refrerence scan time in seconds
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceScanTime() const;
  
  /* Name: std::string svcsig::referenceScanTime() const
   *
   * Description: Returns the scan settings ("AI" = auto integration, "FI" = fixed
   *    integration, "UI" = unknown) for the reference scan
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceScanSettings() const;
  
  /* Name: short * svcsig::referenceExternalDataSet1() const
   *
   * Description: returns the set #1 of the reference scan samples from the 
   *    reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short[ 8 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  short * referenceExternalDataSet1() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R1() const
   *
   * Description: returns the first value of the set #1 of the reference scan 
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  short referenceExternalDataSet1R1() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R2() const
   *
   * Description: returns the second value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R2() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R3() const
   *
   * Description: returns the third value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R3() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R4() const
   *
   * Description: returns the fourth value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R4() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R5() const
   *
   * Description: returns the fifth value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R5() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R6() const
   *
   * Description: returns the sixth value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R6() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R7() const
   *
   * Description: returns the seventh value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R7() const;
  
  /* Name: short svcsig::referenceExternalDataSet1R8() const
   *
   * Description: returns the eigth value of the set #1 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet1R8() const;
  
  /* Name: short * svcsig::referenceExternalDataSet2() const
   *
   * Description: returns the set #2 of the reference scan samples from the 
   *    reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short[ 8 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  short * referenceExternalDataSet2() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R1() const
   *
   * Description: returns the first value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R1() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R2() const
   *
   * Description: returns the second value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R2() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R3() const
   *
   * Description: returns the third value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R3() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R4() const
   *
   * Description: returns the fourth value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R4() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R5() const
   *
   * Description: returns the fifth value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R5() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R6() const
   *
   * Description: returns the sixth value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R6() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R7() const
   *
   * Description: returns the seventh value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R7() const;
  
  /* Name: short svcsig::referenceExternalDataSet2R8() const
   *
   * Description: returns the eigth value of the set #2 of the reference scan
   *    samples from the reference header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short referenceExternalDataSet2R8() const;
  
  /* Name: std::string svcsig::referenceOptic() const
   *
   * Description: returns the name of the fore-optic for the reference scan
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceOptic() const;
  
  /* Name: float * svcsig::referenceTemp() const
   *
   * Description: Returns the temperature in degrees C of the Si, InGaAs1, and
   *    InGaAs2 arrays for the reference scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * referenceTemp() const;
  
  /* Name: float svcsig::referenceTempSi() const
   *
   * Description: Returns the temperature in degrees C of the Si array for the 
   *    reference scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes: 
   */
  float referenceTempSi() const;
  
  /* Name: float svcsig::referenceTempInGaAs1() const
   *
   * Description: Returns the temperature in degrees C of the InGaAs1 array for the
   *    reference scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceTempInGaAs1() const;
  
  /* Name: float svcsig::referenceTempInGaAs2() const
   *
   * Description: Returns the temperature in degrees C of the InGaAs2 array for the
   *    reference scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceTempInGaAs2() const;
  
  /* Name: float svcsig::referenceBattery() const
   *
   * Description: Returns the battery voltage for the reference scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceBattery() const;
  
  /* Name: float svcsig::referenceError() const
   *
   * Description: Returns the error state for the reference scan. 0 (zero) means no
   *    error occured
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes: According to our SVC contact, the error states are not fully implimented
   *    and may not have any meaning.
   */
  unsigned int referenceError() const;
  
  /* Name: std::string svcsig::referenceUnits() const
   *
   * Description: Returns the types of units associated with the refernce scan data.
   *    ("Radiance": 10^-10 W/(cm^2*sr*nm), "Irradiance": 10^-10W/(cm^2*nm), or
   *    "Counts")
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceUnits() const;
  
  /* Name: std::string svcsig::referenceTime() const
   *
   * Description: Returns the time of the reference scan aquisition. Time is in
   *    mm/dd/yyyy HH:MM:SS AM format where mm is month, dd is the day, yyyy is the
   *    year, HH is the hour, MM is the minute, SS is the seconds, and AM is either
   *    AM or PM
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceTime() const;
  
  /* Name: std::string svcsig::referenceLongitude() const
   *
   * Description: Returns the longitude of the reference scan aquisition in
   *    DDDmm.mmmmC format where D is degrees, m is decimal minutes, and C is 
   *    quadrant (E or W).
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceLongitude() const;
  
  /* Name: float svcsig::referenceDecimalLongitude() const
   *
   * Description: Returns the longitude of the reference scan aquisition in 
   *    degrees.
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigLatLonQuad if the quadrant is not N, E, S, or W
   * Notes:
   */
  float referenceDecimalLongitude() const;
  
  /* Name: std::string svcsig::referenceLatitude() const
   *
   * Description: Returns the latitude of the reference scan aquisition in
   *    DDmm.mmmmC format where D is degrees, m is decimal minutes, and C is
   *    quadrant (N or S).
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string referenceLatitude() const;
  
  /* Name: float svcsig::referenceDecimalLatitude() const
   *
   * Description: Returns the latitude of the reference scan aquisition in
   *    degrees.
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigLatLonQuad if the quadrant is not N, E, S, or W
   * Notes:
   */
  float referenceDecimalLatitude() const;
  
  /* Name: std::string svcsig::referenceGpstime() const
   *
   * Description: Returns the gps time of the reference scan. The format is HHmmSS.SSS where H is hours, m is minutes, and s is seconds
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions: 
   * Notes:
   */
  std::string referenceGpstime() const;
  
  /* Name: float svcsig::referenceDecimalGpstime() const
   *
   * Description: Returns the gps time of the reference scan in decimal hours
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float referenceDecimalGpstime() const;
  
  /* Name: unsigned int svcsig::referenceMemorySlot() const
   *
   * Description: Returns the memory slot of the reference scan
   * Arguments:
   * Modifies:
   * Returns: unsigned int
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  unsigned int referenceMemorySlot() const;
  
  // from the target header
  
  /* Name: float * svcsig::targetIntegration() const
   *
   * Description: Returns the target intergration time in ms of the Si,
   *    InGaAs1, and InGaAs2 arrays for the target scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * targetIntegration() const;
  
  /* Name: float svcsig::targetIntegrationSi() const
   *
   * Description: Returns the target intergration time in ms of the Si array
   *    for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetIntegrationSi() const;
  
  /* Name: float svcsig::targetIntegrationInGaAs1() const
   *
   * Description: Returns the target intergration time in ms of the InGaAs1
   *    array for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetIntegrationInGaAs1() const;
  
  /* Name: float svcsig::targetIntegrationInGaAs2() const
   *
   * Description: Returns the target intergration time in ms of the InGaAs2
   *    array for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetIntegrationInGaAs2() const;
  
  /* Name: std::string svcsig::targetScanMethod() const
   *
   * Description: Returns the target scan method ("Time-based" or
   *    "Coadd-based")
   * Arguments:
   * Modifies:
   * Returns: std::strint
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetScanMethod() const;
  
  /* Name: float * svcsig::targetScanCoadds() const
   *
   * Description: Returns the scan coadds of the Si, InGaAs1, and InGaAs2 arrays
   *    for the target scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * targetScanCoadds() const;
  
  /* Name: float svcsig::targetScanCoaddsSi() const
   *
   * Description: Returns the scan coadds of the Si array for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetScanCoaddsSi() const;
  
  /* Name: float svcsig::targetScanCoaddsInGaAs1() const
   *
   * Description: Returns the scan coadds of the InGaAs1 array for the target
   *    scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetScanCoaddsInGaAs1() const;
  
  /* Name: float svcsig::targetScanCoaddsInGaAs2() const
   *
   * Description: Returns the scan coadds of the InGaAs2 arrays for the target
   *    scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetScanCoaddsInGaAs2() const;
  
  /* Name: float svcsig::targetScanTime() const
   *
   * Description: Returns the refrerence scan time in seconds
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetScanTime() const;
  
  /* Name: std::string svcsig::targetScanTime() const
   *
   * Description: Returns the scan settings ("AI" = auto integration, "FI" = fixed
   *    integration, "UI" = unknown) for the target scan
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetScanSettings() const;
  
  /* Name: short * svcsig::targetExternalDataSet1() const
   *
   * Description: returns the set #1 of the target scan samples from the
   *    target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short[ 8 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  short * targetExternalDataSet1() const;
  
  /* Name: short svcsig::targetExternalDataSet1T1() const
   *
   * Description: returns the first value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T1() const;
  
  /* Name: short svcsig::targetExternalDataSet1T2() const
   *
   * Description: returns the second value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T2() const;
  
  /* Name: short svcsig::targetExternalDataSet1T3() const
   *
   * Description: returns the third value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T3() const;
  
  /* Name: short svcsig::targetExternalDataSet1T4() const
   *
   * Description: returns the fourth value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T4() const;
  
  /* Name: short svcsig::targetExternalDataSet1T5() const
   *
   * Description: returns the fifth value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T5() const;
  
  /* Name: short svcsig::targetExternalDataSet1T6() const
   *
   * Description: returns the sixth value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T6() const;
  
  /* Name: short svcsig::targetExternalDataSet1T7() const
   *
   * Description: returns the seventh value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T7() const;
  
  /* Name: short svcsig::targetExternalDataSet1T8() const
   *
   * Description: returns the eigth value of the set #1 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet1T8() const;
  
  /* Name: short * svcsig::targetExternalDataSet2() const
   *
   * Description: returns the set #2 of the target scan samples from the
   *    target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short[ 8 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  short * targetExternalDataSet2() const;
  
  /* Name: short svcsig::targetExternalDataSet2T1() const
   *
   * Description: returns the first value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T1() const;
  
  /* Name: short svcsig::targetExternalDataSet2T2() const
   *
   * Description: returns the second value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T2() const;
  
  /* Name: short svcsig::targetExternalDataSet2T3() const
   *
   * Description: returns the third value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T3() const;
  
  /* Name: short svcsig::targetExternalDataSet2T4() const
   *
   * Description: returns the fourth value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T4() const;
  
  /* Name: short svcsig::targetExternalDataSet2T5() const
   *
   * Description: returns the fifth value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T5() const;
  
  /* Name: short svcsig::targetExternalDataSet2T6() const
   *
   * Description: returns the sixth value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T6() const;
  
  /* Name: short svcsig::targetExternalDataSet2T7() const
   *
   * Description: returns the seventh value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T7() const;
  
  /* Name: short svcsig::targetExternalDataSet2T8() const
   *
   * Description: returns the eigth value of the set #2 of the target scan
   *    samples from the target header, 0 if disabled
   * Arguments:
   * Modifies:
   * Returns: short
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  short targetExternalDataSet2T8() const;
  
  /* Name: std::string svcsig::targetOptic() const
   *
   * Description: returns the name of the fore-optic for the target scan
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetOptic() const;
  
  /* Name: float * svcsig::targetTemp() const
   *
   * Description: Returns the temperature in degrees C of the Si, InGaAs1, and
   *    InGaAs2 arrays for the target scan
   * Arguments:
   * Modifies:
   * Returns: float[ 3 ]
   * Pre:
   * Post:
   * Exceptions:
   * Notes: This is a heap variable. It is up to the user to delete it.
   */
  float * targetTemp() const;
  
  /* Name: float svcsig::targetTempSi() const
   *
   * Description: Returns the temperature in degrees C of the Si array for the
   *    target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetTempSi() const;
  
  /* Name: float svcsig::targetTempInGaAs1() const
   *
   * Description: Returns the temperature in degrees C of the InGaAs1 array for the
   *    target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetTempInGaAs1() const;
  
  /* Name: float svcsig::targetTempInGaAs2() const
   *
   * Description: Returns the temperature in degrees C of the InGaAs2 array for the
   *    target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetTempInGaAs2() const;
  
  /* Name: float svcsig::targetBattery() const
   *
   * Description: Returns the battery voltage for the target scan
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetBattery() const;
  
  /* Name: float svcsig::targetError() const
   *
   * Description: Returns the error state for the target scan. 0 (zero) means no
   *    error occured
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes: According to our SVC contact, the error states are not fully implimented
   *    and may not have any meaning.
   */
  unsigned int targetError() const;
  
  /* Name: std::string svcsig::targetUnits() const
   *
   * Description: Returns the types of units associated with the refernce scan data.
   *    ("Radiance": 10^-10 W/(cm^2*sr*nm), "Irradiance": 10^-10W/(cm^2*nm), or
   *    "Counts")
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetUnits() const;
  
  /* Name: std::string svcsig::targetTime() const
   *
   * Description: Returns the time of the target scan aquisition. Time is in
   *    mm/dd/yyyy HH:MM:SS AM format where mm is month, dd is the day, yyyy is the
   *    year, HH is the hour, MM is the minute, SS is the seconds, and AM is either
   *    AM or PM
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetTime() const;
  
  /* Name: std::string svcsig::targetLongitude() const
   *
   * Description: Returns the longitude of the target scan aquisition in
   *    DDDmm.mmmmC format where D is degrees, m is decimal minutes, and C is
   *    quadrant (E or W).
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetLongitude() const;
  
  /* Name: float svcsig::targetDecimalLongitude() const
   *
   * Description: Returns the longitude of the target scan aquisition in
   *    degrees.
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigLatLonQuad if the quadrant is not N, E, S, or W
   * Notes:
   */
  float targetDecimalLongitude() const;
  
  /* Name: std::string svcsig::targetLatitude() const
   *
   * Description: Returns the latitude of the target scan aquisition in
   *    DDmm.mmmmC format where D is degrees, m is decimal minutes, and C is
   *    quadrant (N or S).
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetLatitude() const;
  
  /* Name: float svcsig::targetDecimalLatitude() const
   *
   * Description: Returns the latitude of the target scan aquisition in
   *    degrees.
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions: invalidSVCsigLatLonQuad if the quadrant is not N, E, S, or W
   * Notes:
   */
  float targetDecimalLatitude() const;
  
  /* Name: std::string svcsig::targetGpstime() const
   *
   * Description: Returns the gps time of the target scan. The format is HHmmSS.SSS where H is hours, m is minutes, and s is seconds
   * Arguments:
   * Modifies:
   * Returns: std::string
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  std::string targetGpstime() const;
  
  /* Name: float svcsig::targetDecimalGpstime() const
   *
   * Description: Returns the gps time of the target scan in decimal hours
   * Arguments:
   * Modifies:
   * Returns: float
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  float targetDecimalGpstime() const;
  
  /* Name: unsigned int svcsig::targetMemorySlot() const
   *
   * Description: Returns the memory slot of the target scan
   * Arguments:
   * Modifies:
   * Returns: unsigned int
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  unsigned int targetMemorySlot() const;
  
  /* Name: size_t svcsig::size() const
   *
   * Description: Returns the number of spectral measurements in the file
   * Arguments:
   * Modifies:
   * Returns: size_t
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  size_t size() const;
 
  /* Name: void svcsig::display() const
   *
   * Description: Displays the file to std::out
   * Arguments:
   * Modifies:
   * Returns: 
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void display() const;
  
  /* Name: void svcsig::displayCommonHeader() const
   *
   * Description: Displays the common header to std::out
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void displayCommonHeader() const;
  
  /* Name: void svcsig::displayReferenceHeader() const
   *
   * Description: Displays the referenceHeader to std::out
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void displayReferenceHeader() const;
  
  /* Name: void svcsig::displayTargetHeader() const
   *
   * Description: Displays the targetHeader to std::out
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void displayTargetHeader() const;
  
  /* Name: void svcsig::display() const
   *
   * Description: Displays the all headers to std::out
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void displayHeaders() const;
  
  /* Name: void svcsig::displayData() const
   *
   * Description: Displays the data to std::out
   * Arguments:
   * Modifies:
   * Returns:
   * Pre:
   * Post:
   * Exceptions:
   * Notes:
   */
  void displayData() const;
};



/****************************  Implimentation  ********************************/

// -- -- Helper Functions -- -- //
void svcsig::svcSigRemoveWhitespace( std::string &s ) const
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

void svcsig::svcSigParseHeaderEquals( std::string &s, std::string &p ) const
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
  } catch ( std::exception &e ) {
    std::cerr << e.what();
    throw;
  }
}

void svcsig::svcSigParseHeaderColon( std::string &s, std::string &p ) const
{
  try {
    size_t loc;
    loc = s.find( ":" );
    if ( loc != std::string::npos ) {
      p = s.substr( 0, loc );
      s = s.substr( loc + 1, s.size() - loc - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
    else {
      throw invalidSVCsigHeader( "in svcSigParseHeaderColon" );
    }
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

void svcsig::svcSigParseHeaderComma( std::string &s, std::string &p ) const
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

void svcsig::svcSigParseHeaderParentheses( std::string &s, std::string &p ) const
{
  p.clear();
  size_t loc1, loc2;
  loc1 = s.find( "(" );
  if ( loc1 != std::string::npos ) {
    loc2 = s.find( ")" );
    if ( loc2 != std::string::npos ) {
      p = s.substr( loc1 + 1, loc2 - loc1 - 1 );
      s = s.substr( 0, loc1 - 1 );
      svcSigRemoveWhitespace( s );
      svcSigRemoveWhitespace( p );
    }
  }
}

void svcsig::svcSigParseHeaderSquareBraces( std::string &s, std::string &p ) const
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

void svcsig::svcSigParseWhitespace( std::string &s, std::string &p ) const
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
      std::string message( "in svcSigParseWhitespace.\n" );
      message += "\tNo space in string '" + s + "'";
      throw invalidSVCsigHeader( message );
    }
  } catch ( invalidSVCsigHeader &e ) {
    throw;
  }
}

void svcsig::svcSigParseData( std::string &s, float &wl, float &refRad, float &tarRad, float &tarRef ) const
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
    std::cerr << "In svcsig::svcSigParseData" << std::endl;
    throw;
  }
}


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

// -- -- IO -- -- //
svcsig& svcsig::read( const std::string &filename )
{
  std::ifstream input;
  input.open( filename.c_str() );
  
  std::string l;
  
  
  try {
    if ( !input.is_open() ) {
      throw notSvcSigFile( "File '" + filename + "' does not exist." );
    }
    
    // I need i/o error checking & Exceptions here
    std::string line;
    std::string part;
    int i;
    
    
    // Check that the header line is good
    getline( input, line );
    svcSigRemoveWhitespace( line );
    if ( line.compare( "/*** Spectra Vista SIG Data ***/" ) != 0 ) {
      throw notSvcSigFile( "Header does not contain: /*** Spectra Vista SIG Data ***/");
    }
    
    bool readHeader( true );
  
    while ( readHeader && input.good() ) {
      
      getline( input, line );
      l = line;
      svcSigParseHeaderEquals( line, part );
      
      if ( part.compare( "data" ) == 0 ) {
        readHeader = false;
      }
      else if ( part.compare( "name" ) == 0 ) {
        _commonHeader.updateName( line );
      }
      else if ( part.compare( "instrument" ) == 0 ) {
        svcSigParseHeaderColon( line, part );
        _commonHeader.updateInstrumentModelNumber( part );
        svcSigParseHeaderParentheses( line, part );
        _commonHeader.updateInstrumentCommonName( part );
        _commonHeader.updateInstrumentExtendedSerialNumber( line );
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
        _referenceHeader.updateGpsTime( part.c_str() );
        _targetHeader.updateGpsTime( line.c_str() );
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
        _commonHeader.updateFactorsReference( atof( part.c_str() ) );
        svcSigParseHeaderComma( line, part );
        _commonHeader.updateFactorsTarget( atof( part.c_str() ) );
        _commonHeader.updateFactorsReflectance( atof( line.c_str() ) );
      }
      else {
        std::cerr << "Unkown key: '" << part << "'." << std::endl;
      }
    } // end read header
    
    
    
    // read the data
    float wl, refRad, tarRad, tarRef;
    getline( input, line );
    while ( input.good() ) {
      svcSigParseData( line, wl, refRad, tarRad, tarRef );
      _wavelength.push_back( wl );
      _referenceRadiance.push_back( refRad );
      _targetRadiance.push_back( tarRad );
      _targetReflectance.push_back( tarRef );
      getline( input, line );
    }
    
    
  } catch ( notSvcSigFile &e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Failed to complete read" << std::endl;
    std::cerr << e.what() << std::endl;
    input.close();
    return *this;
  } catch ( invalidSVCsigHeader &e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Failed to complete read" << std::endl;
    std::cerr << e.what() << std::endl;
    input.close();
    return *this;
  }
  
  input.close();
  return *this;
}

void svcsig::write( const std::string &filename ) const
{
  std::ofstream output;
  output.open( filename.c_str() );
  
  int i;
  
  // error checking!!!
  
  try {
    // write the header
    output << "/*** Spectra Vista SIG Data ***/" << std::endl;
    output << "name= "
    << name() << std::endl;
    output << "instrument= "
    << instrument() << std::endl;
    output << "integration= "
    << referenceIntegrationSi() << ", "
    << referenceIntegrationInGaAs1() << ", "
    << referenceIntegrationInGaAs2() << ", "
    << targetIntegrationSi() << ", "
    << targetIntegrationInGaAs1() << ", "
    << targetIntegrationInGaAs2() << std::endl;
    output << "scan method= "
    << referenceScanMethod() << ", "
    << targetScanMethod() << std::endl;
    output << "scan coadds= "
    << referenceScanCoaddsSi() << ", "
    << referenceScanCoaddsInGaAs1() << ", "
    << referenceScanCoaddsInGaAs2() << ", "
    << targetScanCoaddsSi() << ", "
    << targetScanCoaddsInGaAs1() << ", "
    << targetScanCoaddsInGaAs2() << std::endl;
    output << "scan time= "
    << referenceScanTime() << ", "
    << targetScanTime() << std::endl;
    output << "scan settings= "
    << referenceScanSettings() << ", "
    << targetScanSettings() << std::endl;
    output << "external data set1= "
    << referenceExternalDataSet1R1() << ", "
    << referenceExternalDataSet1R2() << ", "
    << referenceExternalDataSet1R3() << ", "
    << referenceExternalDataSet1R4() << ", "
    << referenceExternalDataSet1R5() << ", "
    << referenceExternalDataSet1R6() << ", "
    << referenceExternalDataSet1R7() << ", "
    << referenceExternalDataSet1R8() << ", "
    << targetExternalDataSet1T1() << ", "
    << targetExternalDataSet1T2() << ", "
    << targetExternalDataSet1T3() << ", "
    << targetExternalDataSet1T4() << ", "
    << targetExternalDataSet1T5() << ", "
    << targetExternalDataSet1T6() << ", "
    << targetExternalDataSet1T7() << ", "
    << targetExternalDataSet1T8() << std::endl;
    output << "external data set2= "
    << referenceExternalDataSet2R1() << ", "
    << referenceExternalDataSet2R2() << ", "
    << referenceExternalDataSet2R3() << ", "
    << referenceExternalDataSet2R4() << ", "
    << referenceExternalDataSet2R5() << ", "
    << referenceExternalDataSet2R6() << ", "
    << referenceExternalDataSet2R7() << ", "
    << referenceExternalDataSet2R8() << ", "
    << targetExternalDataSet2T1() << ", "
    << targetExternalDataSet2T2() << ", "
    << targetExternalDataSet2T3() << ", "
    << targetExternalDataSet2T4() << ", "
    << targetExternalDataSet2T5() << ", "
    << targetExternalDataSet2T6() << ", "
    << targetExternalDataSet2T7() << ", "
    << targetExternalDataSet2T8() << std::endl;
    output << "external data dark= "
    << externalDataDarkD1() << ", "
    << externalDataDarkD2() << ", "
    << externalDataDarkD3() << ", "
    << externalDataDarkD4() << ", "
    << externalDataDarkD5() << ", "
    << externalDataDarkD6() << ", "
    << externalDataDarkD7() << ", "
    << externalDataDarkD8() << std::endl;
    output << "external data mask= "
    << ( (int) externalDataMask() ) << std::endl;
    output << "optic= "
    << referenceOptic() << ", "
    << targetOptic() << std::endl;
    output << "temp= "
    << referenceTempSi() << ", "
    << referenceTempInGaAs1() << ", "
    << referenceTempInGaAs2() << ", "
    << targetTempSi() << ", "
    << targetTempInGaAs1() << ", "
    << targetTempInGaAs2() << std::endl;
    output << "battery= "
    << referenceBattery() << ", "
    << targetBattery() << std::endl;
    output << "error= "
    << referenceError() << ", "
    << targetError() << std::endl;
    output << "units= "
    << referenceUnits() << ", "
    << targetUnits() << std::endl;
    output << "time= "
    << referenceTime() << ", "
    << targetTime() << std::endl;
    output << "longitude= "
    << referenceLongitude() << ", "
    << targetLongitude() << std::endl;
    output << "latitude= "
    << referenceLatitude() << ", "
    << targetLatitude() << std::endl;
    output << "gpstime= "
    << referenceGpstime() << ", "
    << targetGpstime() << std::endl;
    output << "comm= "
    << comm() << std::endl;
    output << "memory slot= "
    << referenceMemorySlot() << ", "
    << targetMemorySlot() << std::endl;
    output << "factors= "
    << factorsReference() << ", "
    << factorsTarget() << ", "
    << factorsReflectance();
    if ( factorsComment().size() > 0 ) {
      output << " [" << factorsComment() << "]";
    }
    output << std::endl;
    
    output << "data=" << std::endl;
    
    // write the data
    for ( i = 0; i < _wavelength.size(); i++ ) {
      output << _wavelength.at( i ) << "  "
      << _referenceRadiance.at( i ) << "  "
      << _targetRadiance.at( i ) << "  "
      << _targetReflectance.at( i ) << std::endl;
    }
    output.close();

  } catch ( std::exception &e ) {
    throw;
  }
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

std::string svcsig::instrumentModelNumber() const
{
  return _commonHeader.instrumentModelNumber();
}

std::string svcsig::instrumentExtendedSerialNumber() const
{
  return _commonHeader.instrumentExtendedSerialNumber();
}

std::string svcsig::instrumentCommonName() const
{
  return _commonHeader.instrumentCommonName();
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

float svcsig::factorsReference() const
{
  return _commonHeader.factorsReference();
}

float svcsig::factorsTarget() const
{
  return _commonHeader.factorsTarget();
}

float svcsig::factorsReflectance() const
{
  return _commonHeader.factorsReflectance();
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
  try {
    return _referenceHeader.decimalLongitude();
  } catch ( invalidSVCsigLatLonQuad &e ) {
    throw;
  }
}

std::string svcsig::referenceLatitude() const
{
  return _referenceHeader.latitude();
}

float svcsig::referenceDecimalLatitude() const
{
  try {
    return _referenceHeader.decimalLatitude();
  } catch ( invalidSVCsigLatLonQuad &e ) {
    throw;
  }
}

std::string svcsig::referenceGpstime() const
{
  return _referenceHeader.gpstime();
}

float svcsig::referenceDecimalGpstime() const
{
  return _referenceHeader.decimalGpstime();
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
  try {
    return _targetHeader.decimalLongitude();
  } catch ( invalidSVCsigLatLonQuad &e ) {
    throw;
  }
}

std::string svcsig::targetLatitude() const
{
  return _targetHeader.latitude();
}

float svcsig::targetDecimalLatitude() const
{
  try {
    return _targetHeader.decimalLatitude();
  } catch ( invalidSVCsigLatLonQuad &e ) {
    throw;
  }
}

std::string svcsig::targetGpstime() const
{
  return _targetHeader.gpstime();
}

float svcsig::targetDecimalGpstime() const
{
  return _targetHeader.decimalGpstime();
}

unsigned int svcsig::targetMemorySlot() const
{
  return _targetHeader.memorySlot();
}

size_t svcsig::size() const
{
  return _wavelength.size();
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