#!/usr/bin/env python

"""READSVCSIG reads an SVC sig file

INPUTS:
    filename: the file (including path in needed)

OUTPUTS:
    output: a dictionary containing the information in the sig file
    output['header']: The file header
    output['header']['commonHeader']: The parts of the header corresponding
          to the entire file
    output['header']['referenceHeader']: The parts of the header corresponding
          to the white reference spectrum
    output['header']['targetHeader']: The parts of the header corresponding to
          the target spectrum
    output['wavelength']: The wavelength values for the spectra.
    output['reference']: The radiance spectrum of the white reference.
    output['target']: The radiance spectrum of the target.
    output['reflectance']: The reflectance spectrum of the target. The
            reflectance is percentage (0-100 range).

HISTORY:
    2015-03-19: Updated Code.
      -- Fixed a bug when the gpstime was not set.
      -- Switched to regular expressions to parse data.
    2015-02-09: Switched the shebang to #!/usr/bin/env python
    2015-02-08: Updated code.
      -- Switched to numpy for holding data
      -- Updated logic
    2014-03-14: Updated documentation
    2014-01-29: Initial version

REFERENCES:
    "SVC HR-1024i / SVC HR-768i User Manual" v1.6
    http://spectravista.com/

PUBLIC REPOSITORY:
    https://github.com/pavdpr/svcread.git

LICENSE:
    The MIT License (MIT) Copyright
    (c) 2013-2015 Rochester Institute of Technology

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

"""

__author__ = "Paul Romanczyk"
__copyright__ = "Copyright 2013--2015, Rochester Institute of Technology"
__credits__ = []
__license__ = "MIT"
#__version__ = "0.1"
__maintainer__ = "Paul Romanczyk"
__email__ = "par4249@rit.edu"
__status__ = "Production"

import re
import sys
import numpy

def readSVCheader(filename):
    """ Reads an SVC sig header

    Keyword arguments:
    filename -- a string containing the filename (including path if needed) of the
                sig file to read

    Returns:


    Exceptions:

    """

    def splitListInHalf(thelist):
        """ This function is used to split a list half

        Keyword arguments:
        thelist -- A list to split in half

        Returns:
        tuple of two lists
        """
        halfPoint = len(thelist) / 2
        return (thelist[:halfPoint], thelist[halfPoint:])

    try:
        fid = open(filename, 'rU')

        # make sure we are svc sig file
        line = fid.readline()
        if line[:-1] != '/*** Spectra Vista SIG Data ***/':
            # use line[:-1] to ignore the newline
            raise RuntimeError("'" + filename + "' is not valid SVC sig file.")

        linect = 1
        # prealocate some headers
        commonHeader = {}
        referenceHeader = {}
        targetHeader = {}

        commaregex = re.compile(r'\s*,\s*')

        run = True
        while run:
            line = fid.readline()
            linect += 1
            splitLine = line.split('=')
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
                    tmp = commaregex.split(value)
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'scan method':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'scan coadds':
                    tmp = commaregex.split(value)
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'scan time':
                    tmp = commaregex.split(value)
                    # can this be an int?
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'scan settings':
                    tmp = commaregex.split(value)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'external data set1':
                    # these seem to not have a space after the comma....
                    # I may want to switch to regualar expressions for this!
                    tmp = commaregex.split(value)
                    # i need to check that this is an int
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'external data set2':
                    # these seem to not have a space after the comma....
                    # I may want to switch to regualar expressions for this!
                    tmp = commaregex.split(value)
                    # i need to check that this is an int
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'external data dark':
                    # these seem to not have a space after the comma....
                    # I may want to switch to regualar expressions for this!
                    tmp = commaregex.split(value)
                    # i need to check that this is an int
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'external data mask':
                    commonHeader[key] = float(value)
                elif key == 'optic':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'temp':
                    tmp = commaregex.split(value)
                    # i need to check that this is an int
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'battery':
                    tmp = commaregex.split(value)
                    tmp = map(float, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'error':
                    tmp = commaregex.split(value)
                    tmp = map(int, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'units':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'time':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'latitude':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'longitude':
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(value)
                elif key == 'gpstime':
                    tmp = commaregex.split(value)
                    # check to see if the value was set.
                    if not tmp:
                        referenceHeader[key] = None
                        targetHeader[key] = None
                    elif tmp[0] and tmp[1]:
                        tmp = map(float, tmp)
                        (referenceHeader[key], targetHeader[key]) = \
                            splitListInHalf(tmp)
                    else:
                        referenceHeader[key] = None
                        targetHeader[key] = None
                elif key == 'comm':
                    commonHeader[key] = value
                elif key == 'memory slot':
                    tmp = commaregex.split(value)
                    tmp = map(int, tmp)
                    (referenceHeader[key], targetHeader[key]) = \
                        splitListInHalf(tmp)
                elif key == 'factors':
                    idx = value.find('[')
                    if idx > 0:
                        tmp = value[:idx]
                        tmp = tmp.strip()
                        tmp = commaregex.split(tmp)
                        commonHeader[key] = map(float, tmp)
                        tmp = value[idx+1:]
                        idx = tmp.find(']')
                        if idx > 0:
                            tmp = tmp[:idx]
                        commonHeader['factors comment'] = tmp
                    else:
                        # no comments
                        tmp = commaregex.split(value)
                        commonHeader[key] = map(float, tmp)
                else:
                    # we are an unknown key, but we can add it since I can index
                    # dictionaries by strings
                    # add __unknown_ in front to help with parsing in writing.
                    commonHeader['__unknown_' + key] = value


        return commonHeader, referenceHeader, targetHeader, linect
    except IOError:
        pass
    except RuntimeError:
        pass
    finally:
        fid.close()

def readSVCdata(filename, skip=25):
    """reads the data from a SVC sig file

    Keyword arguments:
    filename -- (str) the filename
    skip     -- (int) the number of lines to skip (default=25)

    Returns
    numpy.array -- wavelength
    numpy.array -- reference digital count (radiance)
    numpy.array -- target digital cound (radiance)
    numpy.array -- reflectance (0-100 units)
    """

    try:
        data = numpy.loadtxt(filename, skiprows=skip)
        return data[:, 0], data[:, 1], data[:, 2], data[:, 3]

    except IOError:
        pass

    finally:
        pass


def readSVCsig(filename):
    """reads a SVC sig file

    Keyword arguments:
    filename -- (str) the filename

    Returns
    dict
    """

    output = {}
    output['header']={}

    # read the header
    output['header']['commonHeader'], output['header']['referenceHeader'], \
    output['header']['targetHeader'], linect = readSVCheader(filename)

    # read the data
    output['wavelength'], output['referenceDC'], output['targetDC'], \
        output['reflectance'] = readSVCdata(filename, linect)

    return output


if __name__ == '__main__':
    # get the arguments
    args = sys.argv[1:]

    # make sure we have some inputs
    if not args:
        print 'useage: readSVCsig.py file'
        sys.exit(1)

    # this may change in the future if there optional inputs
    filename = ' '.join(args)

    data = readSVCsig(filename)

    print data