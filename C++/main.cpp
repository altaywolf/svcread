#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "svcsig.h"

int main()
{
  svcsig foo;
  foo.read( "/Users/pavdpr/Documents/CaliData/Spectra/gr062113_000.sig" );
  
  std::string newName( "/Users/pavdpr/Desktop/" );
  std::stringstream ss;
  ss << foo.referenceMemorySlot();
  newName += ss.str();
  std::cout << ss.str() << std::endl;
  newName += "_";
  ss.str( std::string() );
  ss << foo.targetMemorySlot();
  std::cout << ss.str() << std::endl;
  newName += ss.str();
  newName += ".sig";
  
  foo.write( newName.c_str() );

  //foo.display();
  return 0;
}