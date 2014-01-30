#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "svcsig.hpp"

void svcrename_usage( void )
{
  std::cout << "svcrename" << std::endl;
  std::cout << "\tChanges the name of a svcsig file" << std::endl;
  std::cout << std::endl;
  std::cout << "Usage: svcrename [-aiostv] filename.sig" << std::endl;
  std::cout << "\ta:\tAppend the scan number(s) to the file name" << std::endl;
  std::cout << "\ti:\tApply the new name to the image if it exists" << std::endl;
  std::cout << "\to:\tDo NOT ask before overwriting" << std::endl;
  std::cout << "\tr:\tUse only the reference scan number" << std::endl;
  std::cout << "\tt:\tUse only the target scan number" << std::endl;
  std::cout << "\tv:\tVerbose" << std::endl;
  return;
};

int main( int argc, char* argv[] )
{
  std::string delimiter( "/" ); // I assume we are unix land
  std::string fullFileName;

  // set the default paramters
  bool _verbose( false );
  bool _tryImage( false );
  bool _append( false );
  bool _useReference( true );
  bool _useTarget( true );
  bool _overWriteProtection( true );

  int i;
  size_t loc;

  switch ( argc ) {
    case 1:
      // no arguments
      svcrename_usage();
      return 0;
      break;
    case 2:
      {
        fullFileName = argv[ 1 ];
        break;
      }
    case 3:
      {
        std::string options( argv[ 1 ] );
        fullFileName = argv[ 2 ];

        // check that the size is big enough
        if ( options.size() < 2 ) {
          std::cerr << "invalid options" << std::endl;
          std::cerr << std::endl;
          svcrename_usage();
          return 1;
        }
        // check that the first option character is a -
        if ( options.substr( 0, 1 ).compare( "-" ) != 0 ) {
          std::cerr << "invalid options" << std::endl;
          std::cerr << std::endl;
          svcrename_usage();
          return 1;
        }

        // parse the options
        for ( i = 1; i < options.size(); ++i ) {
          if ( options.substr( i, 1 ).compare( "a" ) == 0 ) {
            _append = true;
          }
          else if ( options.substr( i, 1 ).compare( "i" ) == 0 ) {
            _tryImage = true;
          }
          else if ( options.substr( i, 1 ).compare( "o" ) == 0 ) {
            _overWriteProtection = false;
          }
          else if ( options.substr( i, 1 ).compare( "r" ) == 0 ) {
            _useReference = true;
            _useTarget = false;
          }
          else if ( options.substr( i, 1 ).compare( "t" ) == 0 ) {
            _useReference = false;
            _useTarget = true;
          }
          else if ( options.substr( i, 1 ).compare( "v" ) == 0 ) {
            _verbose = true;
          }
          else {
            std::cerr << "invalid options" << std::endl;
            std::cerr << std::endl;
            svcrename_usage();
            return 1;
          }
        }
        break;
      }
    default:
      std::cerr << "Too many arguments" << std::endl;
      std::cout << std::endl;
      svcrename_usage();
      return 1;
  }

  svcsig sigfile;

  // set up some new names
  std::string path;
  std::string filename;
  std::string fileBaseName;
  std::string outputBaseName;
  std::stringstream ss;

  loc = fullFileName.rfind( delimiter );
  if ( loc != std::string::npos ) {
    path = fullFileName.substr( 0, loc + 1 );
    filename = fullFileName.substr( loc + 1, fullFileName.size() - loc - 1 );
  } 
  else {
    path = std::string();
    filename = fullFileName;
  }

  loc = filename.rfind( "." );
  if ( loc == std::string::npos ) {
    fileBaseName = filename;
  }
  else {
    fileBaseName = filename.substr( 0, loc );
  }

  // build the output base name
  outputBaseName = path;

  if ( _append ) {
    outputBaseName += ( fileBaseName + "__" );
  }

  if ( _useTarget ) {
    ss << sigfile.targetMemorySlot();
    outputBaseName += ss.str();
    ss.str( std::string() );
    if ( _useReference ) {
      ss << sigfile.referenceMemorySlot();
      outputBaseName += ( "_" + ss.str() );
    }
  }
  else {
    // if _useTarget is false, _useReference must be true
    ss << sigfile.referenceMemorySlot();
    outputBaseName += ss.str();
  }

  outputBaseName += ".sig";

  bool run;
  std::string test;

  try {
    sigfile.read( fullFileName );

    // try to copy the file 
    std::ifstream  src;
    if ( _overWriteProtection ) {
      // use overwrite protection
      src.open( outputBaseName.c_str() );
      if ( src.is_open() ) {
        std::cout << "WARNING The file '" + outputBaseName + "' already exists" << std::endl;
        run = true;
        while ( run ) {
          std::cout << "overwrite? [y/n]:" << std::endl;
          std::cin >> test;

          if ( test.compare( "y" ) == 0 ) {
            run = false;
            src.close();
          }
          else if ( test.compare( "n" ) == 0 ) {
            run = false;
            src.close();
            return 1;
          }
          else if ( test.compare( "Y" ) == 0 ) {
            run = false;
            src.close();
          }
          else if ( test.compare( "N" ) == 0 ) {
            run = false;
            src.close();
            return 1;
          }
          else {
            std::cout << "Invalid option" << std::endl;
          }
        }
      }
      else {
        // this might be a problem
        src.close();
      }
    }
    
    std::ofstream dst( outputBaseName.c_str() );
    src.open( fullFileName.c_str() );
    dst << src.rdbuf();
    src.close();
    dst.close();

    // copy image if desired
    if ( _tryImage ) {
      // update the names
      outputBaseName += ".jpg";
      fullFileName += ".jpg";

      // check if the original file exists
      src.open( fullFileName.c_str(), std::ios::binary | std::ios::in );
      if ( src.is_open() ) {
        if ( _overWriteProtection ) {
          src.close();
          src.open( outputBaseName.c_str(), std::ios::binary | std::ios::in );
          if ( src.is_open() ) {
            std::cout << "WARNING The file '" + outputBaseName + "' already exists" << std::endl;
            run = true;
            while ( run ) {
              std::cout << "overwrite? [y/n]:" << std::endl;
              std::cin >> test;

              if ( test.compare( "y" ) == 0 ) {
                run = false;
                src.close();
              }
              else if ( test.compare( "n" ) == 0 ) {
                run = false;
                src.close();
                return 1;
              }
              else if ( test.compare( "Y" ) == 0 ) {
                run = false;
                src.close();
              }
              else if ( test.compare( "N" ) == 0 ) {
                run = false;
                src.close();
                return 1;
              }
              else {
                std::cout << "Invalid option" << std::endl;
              }
            }
            // reopen the input file
            src.open( fullFileName.c_str(), std::ios::binary | std::ios::in );
          }
          else {
            // this might be a problem
            src.close();
          }
        }
        dst.open( outputBaseName.c_str(), std::ios::binary | std::ios::out );
        dst << src.rdbuf();
        src.close();
        dst.close();
      }
    }

    return 0;
  } catch ( std::exception &e ) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}