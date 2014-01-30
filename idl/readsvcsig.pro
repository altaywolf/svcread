FUNCTION READSVCSIG, filename
;+ READSVCSIG reads an SVC sig file
;
; INPUTS:
;   filename: the file (including path in needed)
;
; OUTPUTS:
;   data: a structure containing the information in the sig file
;   data.commonHeader: The parts of the header corresponding to the entire
;       file
;   data.referenceHeader: The parts of the header corresponding to the
;       white reference spectrum
;   data.targetHeader: The parts of the header corresponding to the target
;       spectrum
;   data.wavelength: The wavelength values for the spectra
;   data.reference: The radiance spectrum of the white reference
;   data.target: The radiance spectrum of the target
;   data.reflectance: The reflectance spectrum of the target
;
; HISTORY:
;   2013-07-24: Written by Paul Romanczyk (RIT)
;   2013-08-04: Updated by Paul Romanczyk
;      + Added a commonHeader.factorsComment field
;      + Changed commonHeader.factors to be a float array
;   2013-09-20: Updated by Paul Romanczyk
;      + Changed copyright to RIT
;
; REFERENCES:
;   "SVC HR-1024i / SVC HR-768i User Manual" v1.6
;   http://spectravista.com/
;   
; PUBLIC REPOSITORY:
;   https://github.com/pavdpr/svcread.git
;
; LICENSE:
;   The MIT License (MIT) Copyright (c) 2013 Rochester Institute of Technology
;
;   Permission is hereby granted, free of charge, to any person obtaining a copy
;   of this software and associated documentation files (the "Software"), to 
;   deal in the Software without restriction, including without limitation the 
;   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
;   sell copies of the Software, and to permit persons to whom the Software is 
;   furnished to do so, subject to the following conditions:
;
;   The above copyright notice and this permission notice shall be included in 
;   all copies or substantial portions of the Software.
;
;   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
;   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
;   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
;   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
;   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
;   IN THE SOFTWARE.
;
;-


; set up structures
commonHeader = { $
  name:"", $
  instrument:"", $
  externalDataDark:INTARR(8), $
  externalDataMask: 0, $
  comm: "", $
  factors: FLTARR(3), $
  factorsComment: "" }
  
targetHeader = { $
  integration:FLTARR(3), $
  scanMethod:"", $
  scanCoadds:INTARR(3), $
  scanTime:0, $
  scanSettings:"", $
  externalDataSet1:INTARR(8), $
  externalDataSet2:INTARR(8), $
  optic:"", $
  temp:FLTARR(3), $
  battery:0D, $
  error:0, $
  units:"", $
  time:"", $
  longitude:"", $
  latitude:"", $
  gpstime:0D, $
  memorySlot:0 }

referenceHeader = targetHeader

; Attempt to read the file
OPENR, lun, filename, /GET_LUN

readHeader = 1

line = ""

READF, lun, line

; check that we are a SVC sig file
IF NOT STRMATCH( STRTRIM( line, 2 ), "/*** Spectra Vista SIG Data ***/" ) THEN BEGIN
  ; Throw an error
  PRINT, "ERROR"
ENDIF

WHILE readHeader DO BEGIN

  READF, lun, line
  ; Find where the location is
  loc = STRPOS( line, "=" )
  IF loc GT 0 THEN BEGIN
    extra = STRTRIM( STRMID( line, loc + 1, STRLEN( line ) - loc - 1 ), 2 )    
    line = STRTRIM( STRMID( line, 0, loc ), 2 )
    
    CASE line OF
      "data": $
        BEGIN
          readHeader = 0
        END
      "name": commonHeader.name = extra  
      "instrument": commonHeader.instrument = extra
      "integration": $
        BEGIN
          loc = STRPOS( extra, "," )
          referenceHeader.integration[0] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.integration[1] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.integration[2] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )    
          
          loc = STRPOS( extra, "," )
          targetHeader.integration[0] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.integration[1] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.integration[2] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
        END
      "scan method": $
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.scanMethod = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.scanMethod = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "scan coadds": $
        BEGIN
          loc = STRPOS( extra, "," )
          referenceHeader.scanCoadds[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.scanCoadds[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.scanCoadds[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )    
          
          loc = STRPOS( extra, "," )
          targetHeader.scanCoadds[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.scanCoadds[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.scanCoadds[2] = FIX( extra )
        END
      "scan time":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.scanTime = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.scanTime = FIX( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "scan settings":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.scanSettings = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.scanSettings = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "external data set1":$
        BEGIN
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[3] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[4] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[5] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[6] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet1[7] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[3] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[4] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[5] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet1[6] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          targetHeader.externalDataSet1[7] = FIX( extra )
          
        END
      "external data set2":$
        BEGIN
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[3] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[4] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[5] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[6] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          referenceHeader.externalDataSet2[7] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[3] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[4] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[5] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          targetHeader.externalDataSet2[6] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          targetHeader.externalDataSet2[7] = FIX( extra )
          
        END
      "external data dark": $
        BEGIN
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[0] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[1] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[2] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[3] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[4] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[5] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.externalDataDark[6] = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          commonHeader.externalDataDark[7] = FIX( extra )
        END
      "external data mask": commonHeader.externalDataMask = FIX( extra )
      "optic":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.optic = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.optic = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "temp":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.temp = DOUBLE( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.temp = DOUBLE( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "battery":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.battery = DOUBLE( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.battery = DOUBLE( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "error":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.error = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.error = FIX( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "units": $
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.units = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.units = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "time":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.time = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.time = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "longitude":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.longitude = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.longitude = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "latitude":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.latitude = STRTRIM( STRMID( extra, 0, loc ), 2 )
          referenceHeader.latitude = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
        END
      "gpstime":$
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.gpsTime = DOUBLE( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.gpsTime = DOUBLE( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "comm": commonHeader.comm = extra
      "memory slot": $
        BEGIN
          loc = STRPOS( extra, "," )
          targetHeader.memorySlot = FIX( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          referenceHeader.memorySlot = FIX( STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 ) )
        END
      "factors": $
        BEGIN
          ; search for a comment
          loc = STRPOS( extra, "[" )
          loc2 = STRPOS( extra, "]" )
          IF loc GE 0 THEN BEGIN
            IF loc2 GE loc THEN BEGIN
              commonHeader.factorsComment = STRTRIM( STRMID( extra, loc + 1, loc2 - loc - 1 ), 2 )
            ENDIF
            extra = STRTRIM( STRMID( extra, 0, loc - 1 ), 2 )
          ENDIF
          
          loc = STRPOS( extra, "," )
          commonHeader.factors[0] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.factors[1] = FLOAT( STRTRIM( STRMID( extra, 0, loc ), 2 ) )
          extra = STRTRIM( STRMID( extra, loc + 1, STRLEN( extra ) - loc - 1 ), 2 )
          loc = STRPOS( extra, "," )
          commonHeader.factors[2] = FLOAT( extra )
        END
      ELSE: $
        BEGIN
          PRINT, line
        END
    ENDCASE
  
  ENDIF 

ENDWHILE

wavelength = []
reference = []
target = []
reflectance = []

; Read the "data"
WHILE NOT EOF( lun ) DO BEGIN
  READF, lun, line
  
  loc = STRPOS( line, " " )
  wavelength = [ wavelength, DOUBLE( STRTRIM( STRMID( line, 0, loc ), 2 ) ) ]
  line = STRTRIM( STRMID( line, loc + 1, STRLEN( line ) - loc - 1 ), 2 ) 
  loc = STRPOS( line, " " )  
  reference = [ wavelength, DOUBLE( STRTRIM( STRMID( line, 0, loc ), 2 ) ) ]
  line = STRTRIM( STRMID( line, loc + 1, STRLEN( line ) - loc - 1 ), 2 )  
  loc = STRPOS( line, " " )
  target = [ wavelength, DOUBLE( STRTRIM( STRMID( line, 0, loc ), 2 ) ) ]
  reflectance = [ reflectance, DOUBLE( STRTRIM( STRMID( line, loc + 1, STRLEN( line ) - loc - 1 ), 2 ) ) ]   
 
ENDWHILE

FREE_LUN, lun

; construct the returned structure

data = { $
  commonHeader:commonHeader, $
  referenceHeader:referenceHeader, $
  targetHeader:targetHeader, $
  wavelength:wavelength, $
  reference:reference, $
  target:target, $
  reflectance:reflectance }
  
return, data
END