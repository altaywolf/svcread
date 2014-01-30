#!/usr/bin/python

"""
File readSVCsig.py

Reads an SVC sig file

"""

import sys

def splitListInHalf( list ):
  """
    This function is used to split a list half
  """
  halfPoint = len( list ) / 2
  return ( list[:halfPoint], list[halfPoint:])

def readSVCheader( fid ):
  # prealocate some headers
  
  commonHeader = {}
  referenceHeader = {}
  targetHeader = {}
  
  run = True
  while run:
    line = fid.readline()
    splitLine = line.split( '=' )
    key = splitLine[0].strip()
    
    
    # parse the keys
    if key == 'data':
      run = False
    else:
      value = splitLine[1].strip()
      # should add error checking on this operation
      if key == 'name':
        commonHeader[key] = value
      elif key == 'instrument':
        commonHeader[key] = value
      elif key == 'integration':
        tmp = value.split( ', ' )
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'scan method':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'scan coadds':
        tmp = value.split( ', ' )
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'scan time':
        tmp = value.split( ', ' )
        # can this be an int?
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'scan settings':
        tmp = value.split( ', ' )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'external data set1':
        # these seem to not have a space after the comma....
        # I may want to switch to regualar expressions for this!
        tmp = value.split( ',' )
        # i need to check that this is an int
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'external data set2':
        # these seem to not have a space after the comma....
        # I may want to switch to regualar expressions for this!
        tmp = value.split( ',' )
        # i need to check that this is an int
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'external data dark':
        # these seem to not have a space after the comma....
        # I may want to switch to regualar expressions for this!
        tmp = value.split( ',' )
        # i need to check that this is an int
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'external data mask':
        commonHeader[key] = float( value )
      elif key == 'optic':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'temp':
        tmp = value.split( ', ' )
        # i need to check that this is an int
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'battery':
        tmp = value.split( ', ' )
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'error':
        tmp = value.split( ', ' )
        tmp = map( int, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'units':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'time':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'latitude':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'longitude':
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( value )
      elif key == 'gpstime':
        tmp = value.split( ', ' )
        tmp = map( float, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'comm':
        commonHeader[ key ] = value
      elif key == 'memory slot':
        tmp = value.split( ', ' )
        tmp = map( int, tmp )
        (referenceHeader[ key ], targetHeader[ key ] ) = splitListInHalf( tmp )
      elif key == 'factors':
        idx = value.find( '[' )
        if idx > 0:
          tmp = value[:idx]
          tmp = tmp.strip()
          tmp = tmp.split( ', ' )
          commonHeader[ key ] = map( float, tmp )
          tmp = value[idx+1:]
          idx = tmp.find( ']' )
          if idx > 0:
            tmp = tmp[:idx]
          commonHeader[ 'factors comment' ] = tmp
        else:
          # no comments
          tmp = value.split( ', ' )
          commonHeader[ key ] = map( float, tmp )
      else:
        # we are an unknown key, but we can add it since I can index dictionaries by strings
        # add __unknown_ in front to help with parsing in writing.
        commonHeader[ '__unknown_' + key ] = value


  return ( commonHeader, referenceHeader, targetHeader )

def readSVCdata( fid ):
  
  wavelength = []
  referenceDC = []
  targetDC = []
  reflectance = []
  
  text = fid.read()
  text = text.split( '\n' )
  print text
  for line in text:
    tmp = line.split()
    tmp = map( float, tmp )
    
    if len( line ):
      wavelength.append( tmp[ 0 ] )
      referenceDC.append( tmp[ 1 ] )
      targetDC.append( tmp[ 2 ] )
      reflectance.append( tmp[ 3 ] )
  
  return ( wavelength, referenceDC, targetDC, reflectance )

def readSVCsig( filename ):
  fid = open( filename, 'rU' )
  line = fid.readline()
  
  output = {}
  
  # make sure we are svc sig file
  if line[:-1] != '/*** Spectra Vista SIG Data ***/':
    # should throw an error here
    print 'Not a valid SVC SIG file'
  
  # read the header
  ( output['commonHeader'], output['referenceHeader'], output['targetHeader'] ) = readSVCheader( fid )
  
  # read the data
  ( output['wavelength'], output['referenceDC'], output['targetDC'], output['reflectance'] ) = readSVCdata( fid )
  
  fid.close()
  
  return output

def main():
  # get the arguments
  args = sys.argv[1:]

  # make sure we have some inputs
  if not args:
    print 'useage: readSVCsig.py file'
    sys.exit(1)

  # this may change in the future if there optional inputs
  if len( args ) != 1:
    # assume the user has spaces in the file name!
    print 'I assume you have spaces in the filename'
    print '\tuseage: readSVCsig.py file'
    filename = ''
    for part in args:
      filename + part + ' '
        
    # remove the last space
    filename = filename[:-1]
  else:
    filename = args[0]

  # try to read the file (I should use exceptions)
  print filename
  data = readSVCsig( filename )

  print data


if __name__ == '__main__':
  main()