function data = readSVCsig( filename )
%READSVCSIG reads an SVC sig file
%
% INPUTS:
%   filename: the file (including path in needed)
%
% OUTPUTS:
%   data: a structure containing the information in the sig file
%   data.commonHeader: The parts of the header corresponding to the entire
%       file
%   data.referenceHeader: The parts of the header corresponding to the
%       white reference spectrum
%   data.targetHeader: The parts of the header corresponding to the target
%       spectrum
%   data.wavelength: The wavelength values for the spectra
%   data.reference: The radiance spectrum of the white reference
%   data.target: The radiance spectrum of the target
%   data.reflectance: The reflectance spectrum of the target
%
% HISTORY:
%   2013-07-24: Written by Paul Romanczyk (RIT)
%
% REFERENCES:
%   "SVC HR-1024i / SVC HR-768i User Manual" v1.6
%   http://spectravista.com/
%
% PUBLIC REPOSITORY:
%   https://github.com/pavdpr/svcread.git
%
% LICENSE:
%   The MIT License (MIT) Copyright (c) 2013 Paul Romanczyk
%
%   Permission is hereby granted, free of charge, to any person obtaining a copy
%   of this software and associated documentation files (the "Software"), to 
%   deal in the Software without restriction, including without limitation the 
%   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
%   sell copies of the Software, and to permit persons to whom the Software is 
%   furnished to do so, subject to the following conditions:
%
%   The above copyright notice and this permission notice shall be included in 
%	all copies or substantial portions of the Software.
%
%   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
%   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
%   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
%   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
%   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
%   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
%   IN THE SOFTWARE.	
%
%
	
% attempt to open the file
fid = fopen( filename, 'r' );

if fid < 0
    error( 'readSVCsig:fileDoesNotExist', ...
        [ '"' filename '" is does not exist' ] );
end

% check that we are a SVC data file
% => The strtrim is to remove the new line character from fgets
tline = strtrim( fgets( fid ) );

if ~strcmp( tline, '/*** Spectra Vista SIG Data ***/' )
    fclose( fid );
    error( 'readSVCsig:invalidSigFile', ...
        [ '"' filename '" is not a valid SVC sig file' ] );
end

% read the header
run = true;
while run
    tline = fgets( fid );
    
    % find the '='
    loc = strfind( tline, '=' );
    if numel( loc ) == 0
        error( 'readSVCsig:invalidSigHeader', ...
        [ 'The line:\n' tline '\n does not contain an =' ] );
    end
    if numel( loc ) > 1
        error( 'readSVCsig:invalidSigHeader', ...
        [ 'The line:\n' tline '\n does contains more than 1 =' ] );
    end
    
    % convert into a key (lhs) and data (rhs)
    lhs = lower( strtrim( tline( 1:loc - 1 ) ) );
    rhs = strtrim( tline( loc + 1:end ) );
    
    switch lhs
        case 'data'
            % we have found the begining of the data, stop reading the
            % header and start reading data
            run = false;
            
        case 'name'
            commonHeader.name = rhs;
            
        case 'instrument'
            commonHeader.instrument = rhs;
            
        case 'integration'
            tline = rhs;
            loc = strfind( tline, ',' );
            
            referenceHeader.integration = [ ...
                str2double( tline( 1:loc( 1 ) - 1 ) ), ...
                str2double( tline( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( tline( loc( 2 ) + 1:loc( 3 ) - 1 ) ) ];
            targetHeader.integration = [ ...
                str2double( tline( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( tline( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( tline( loc( 5 ) + 1:end ) ) ];
        case 'scan method'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.scanMethod = lhs;
            targetHeader.scanMethod = rhs;
        case 'scan coadds'
            tline = rhs;
            loc = strfind( tline, ',' );
            
            referenceHeader.scanCoadds = [ ...
                str2double( tline( 1:loc( 1 ) - 1 ) ), ...
                str2double( tline( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( tline( loc( 2 ) + 1:loc( 3 ) - 1 ) ) ];
            targetHeader.scanCoadds = [ ...
                str2double( tline( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( tline( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( tline( loc( 5 ) + 1:end ) ) ];
            
        case 'scan time'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.scanTime = str2double( lhs );
            targetHeader.scanTime = str2double( rhs );
            
        case 'scan settings'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.scanSettings = lhs;
            targetHeader.scanSettings = rhs; 
            
        case 'external data set1'            
            tline = rhs;
            loc = strfind( tline, ',' );
            
            referenceHeader.externalDataSet1 = [ ...
                str2double( tline( 1:loc( 1 ) - 1 ) ), ...
                str2double( tline( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( tline( loc( 2 ) + 1:loc( 3 ) - 1 ) ), ...
                str2double( tline( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( tline( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( tline( loc( 5 ) + 1:loc( 6 ) - 1 ) ), ...
                str2double( tline( loc( 6 ) + 1:loc( 7 ) - 1 ) ), ...
                str2double( tline( loc( 7 ) + 1:loc( 8 ) - 1 ) ) ];
            targetHeader.externalDataSet1 = [ ...
                str2double( tline( loc( 8 ) + 1:loc( 9 ) - 1 ) ), ...
                str2double( tline( loc( 9 ) + 1:loc( 10 ) - 1 ) ), ...
                str2double( tline( loc( 10 ) + 1:loc( 11 ) - 1 ) ), ...
                str2double( tline( loc( 11 ) + 1:loc( 12 ) - 1 ) ), ...
                str2double( tline( loc( 12 ) + 1:loc( 13 ) - 1 ) ), ...
                str2double( tline( loc( 13 ) + 1:loc( 14 ) - 1 ) ), ...
                str2double( tline( loc( 14 ) + 1:loc( 15 ) - 1 ) ), ...
                str2double( tline( loc( 15 ) + 1:end ) ) ];
            
        case 'external data set2'
            tline = rhs;
            loc = strfind( tline, ',' );
            
            referenceHeader.externalDataSet2 = [ ...
                str2double( tline( 1:loc( 1 ) - 1 ) ), ...
                str2double( tline( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( tline( loc( 2 ) + 1:loc( 3 ) - 1 ) ), ...
                str2double( tline( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( tline( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( tline( loc( 5 ) + 1:loc( 6 ) - 1 ) ), ...
                str2double( tline( loc( 6 ) + 1:loc( 7 ) - 1 ) ), ...
                str2double( tline( loc( 7 ) + 1:loc( 8 ) - 1 ) ) ];
            targetHeader.externalDataSet2 = [ ...
                str2double( tline( loc( 8 ) + 1:loc( 9 ) - 1 ) ), ...
                str2double( tline( loc( 9 ) + 1:loc( 10 ) - 1 ) ), ...
                str2double( tline( loc( 10 ) + 1:loc( 11 ) - 1 ) ), ...
                str2double( tline( loc( 11 ) + 1:loc( 12 ) - 1 ) ), ...
                str2double( tline( loc( 12 ) + 1:loc( 13 ) - 1 ) ), ...
                str2double( tline( loc( 13 ) + 1:loc( 14 ) - 1 ) ), ...
                str2double( tline( loc( 14 ) + 1:loc( 15 ) - 1 ) ), ...
                str2double( tline( loc( 15 ) + 1:end ) ) ];
            
        case 'external data dark'
            loc = strfind( rhs, ',' );
            commonHeader.externalDataDark = [ ...
                str2double( rhs( 1:loc( 1 ) - 1 ) ), ...
                str2double( rhs( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( rhs( loc( 2 ) + 1:loc( 3 ) - 1 ) ), ...
                str2double( rhs( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( rhs( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( rhs( loc( 5 ) + 1:loc( 6 ) - 1 ) ), ...
                str2double( rhs( loc( 6 ) + 1:loc( 7 ) - 1 ) ), ...
                str2double( rhs( loc( 7 ) + 1:loc( 8 ) - 1 ) ) ];
            
        case 'external data mask'
            commonHeader.externalDataMask = rhs;
            
        case 'optic'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.optic = lhs;
            targetHeader.optic = rhs;
            
        case 'temp'
            tline = rhs;
            loc = strfind( tline, ',' );
            
            referenceHeader.temp = [ ...
                str2double( tline( 1:loc( 1 ) - 1 ) ), ...
                str2double( tline( loc( 1 ) + 1:loc( 2 ) - 1 ) ), ...
                str2double( tline( loc( 2 ) + 1:loc( 3 ) - 1 ) ) ];
            targetHeader.temp = [ ...
                str2double( tline( loc( 3 ) + 1:loc( 4 ) - 1 ) ), ...
                str2double( tline( loc( 4 ) + 1:loc( 5 ) - 1 ) ), ...
                str2double( tline( loc( 5 ) + 1:end ) ) ];
            
        case 'battery'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            referenceHeader.battery = str2double( lhs );
            targetHeader.battery = str2double( rhs );
            
        case 'error'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.error = str2double( lhs );
            targetHeader.error = str2double( rhs );
            
        case 'units'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.units = lhs;
            targetHeader.units = rhs;
            
        case 'time'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.time = lhs;
            targetHeader.time = rhs;
            
        case 'longitude'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.longitude = lhs;
            targetHeader.longitude = rhs;
            
        case 'latitude'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.latitude = lhs;
            targetHeader.latitude = rhs;
            
        case 'gpstime'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.gpsTime = str2double( lhs );
            targetHeader.gpsTime = str2double( rhs );
            
        case 'comm'
            commonHeader.com = rhs;
            
        case 'memory slot'
            tline = rhs;
            loc = strfind( tline, ',' );
            lhs = strtrim( tline( 1:loc - 1 ) );
            rhs = strtrim( tline( loc + 1:end ) );
            
            referenceHeader.memorySlot = str2double( lhs );
            targetHeader.memorySlot = str2double( rhs );
            
        case 'factors'
            commonHeader.factors = rhs;
            
        otherwise 
            fprintf( 'Unknown Header Field: "%s"\n', lhs );
    end
end
% set the data header
data.commonHeader = commonHeader;
data.referenceHeader = referenceHeader;
data.targetHeader = targetHeader;

% read the data
tmp = textscan( fid, '%f%f%f%f' );

% add the SVC data to the data structure
data.wavelength = tmp{ 1 };
data.reference = tmp{ 2 };
data.target = tmp{ 3 };
data.reflectance = tmp{ 4 };
% close the file
fclose( fid );
end

