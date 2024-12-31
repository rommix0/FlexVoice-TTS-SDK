/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created:         2001.06.25.                                          *
*  Last modifified: 2001.08.22.                                          *
*                                                                        *
*************************************************************************/

#include <FVNetCS/FVNetClient.h>
#include "WaveFile.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/timeb.h>
#include <windows.h>
#include <mmsystem.h>

static std::string          g_host;
static int                  g_port          = 0;
static std::string          g_outputfile;
static std::string          g_text;
static std::string          g_lang;
static double               g_bytesPerSec   = 0.0;
static double               g_speed         = 0.0;

static bool usage()
{
    std::cout 
        << "Usage: FVNetClientTest [options] host port output_file { text | -f input_file }\n"
        << "\n"
        << "  Options:\n"
        << "    -h                  help\n"         
        << "    -lang language      the language (eng, hun, or malay) REQUIRED!\n"         
        << "    -spd speed          Generation speed [in percent]\n"
        << "\n"
        << "  host: the host address where the server runs\n"
        << "  port: the port where the server listens\n"
        << "  Text: a single word or text in quotes\n"
        ;
    return false;
}

static bool processCommandLine( int argc, char* argv[] )
{
    int count = 0;
    bool langok = false;
    for( int i=1; i < argc; ++i )
    {
        if( strcmp( argv[i], "-h" ) == 0 )
        {
            std::cout << "Connects to the server, sends the given text to it and writes\n";
            std::cout << "the output to a wave file\n";
            std::cout << std::endl;
            return usage();
        };
        if( strcmp( argv[i], "-f" ) == 0 )
        {
            if( ++i >= argc ) return usage();

            std::ifstream file( argv[i] );
            if( file.good() ) 
            {
                g_text.erase();
                for( unsigned char ch = file.get(); file.good() && !file.eof(); ch = file.get() )
                    g_text += ch;
            }
            else 
            {
                std::cout << "Cannot open input file " << argv[i] << std::endl;
                return false;
            }
        }
        else
        if( strcmp( argv[i], "-lang" ) == 0 )
        {
            if( ++i >= argc ) return usage();
            g_lang = argv[i];
            langok = true;
        }
        else
        if( strcmp( argv[i], "-spd" ) == 0 )
        {
            if( ++i >= argc ) return usage();

            g_speed = atoi( argv[i] ) / 100.0;
        }
        else
        {
            if( argv[i][0] == '-' )
            {
                std::cout << "Invalid option!" << std::endl;
                return usage();
            }
            switch( ++count )
            {
            case 1: g_host          = argv[i]        ; break;
            case 2: g_port          = atoi(argv[i])  ; break;
            case 3: g_outputfile    = argv[i]        ; break;
            case 4: g_text          = argv[i]        ; break;
            default:
                std::cout << "Too many arguments!" << std::endl;
                return usage();
            }
        }
    }

    if( !langok )
    {
        std::cout << "No language given!" << std::endl;
        return usage();
    }
    if( g_text.empty() ) 
    {
        std::cout << "No input text given!" << std::endl;
        return usage();
    }
    if( g_host.empty() ) 
    {
        std::cout << "No host given!" << std::endl;
        return usage();
    }
    if( g_host.empty() ) 
    {
        std::cout << "No port given!" << std::endl;
        return usage();
    }
    if( g_outputfile.empty() ) 
    {
        std::cout << "No output file given!" << std::endl;
        return usage();
    }
    return true;
}

static bool             g_first          = false;
static unsigned long    g_firstDataTime  = 0;
static unsigned long    g_dataTime       = 0;

/** wait method that controls the generation speed
 */

void waitData( int size )
{
    if( g_bytesPerSec == 0.0 )
        return;

    unsigned long  currtime = GetTickCount();

    if( g_first )
    {
        g_firstDataTime = currtime;
    }
    g_first = false;

    int datasize = 1000 * size / g_bytesPerSec;

    if( g_speed > 0.0 ) 
        datasize /= g_speed;
    else
        return;

    if( currtime >= g_firstDataTime ) // required!!!
    {
        currtime -= g_firstDataTime;

        g_dataTime += datasize;
        if( g_dataTime > currtime )
        {
            int towait = g_dataTime - currtime;

            HANDLE  eventhandle = CreateEvent( NULL, TRUE, FALSE, NULL );
            WaitForSingleObject( eventhandle, towait );
            CloseHandle( eventhandle );
        }
    }
}

/** Callback function passed to FVNetClient
  */
static int callback( void * param, const unsigned char * data, int size )
{
    WaveFile* wave = reinterpret_cast<WaveFile*>( param );

    wave->writeData( data, size );
    waitData( size );

    return 1;
}

/** Callback function passed to FVNetClient for raw formats
  */
static int callback_RAW( void * param, const unsigned char * data, int size )
{
    FILE* file = reinterpret_cast<FILE*>( param );

    fwrite( data, size, 1, file );
    waitData( size );

    return 1;
}

void printError( int error )
{
    switch( error )
    {
    case FVNetClientError_OK: 
        break;
    case FVNetClientError_CannotConnect: 
        std::cout << "Can't connect to the server!\n";
        break;
    case FVNetClientError_TransferError: 
        std::cout << "Transfer error!\n";
        break;
    case FVNetClientError_LanguageNotServed: 
        std::cout << "Language not served!\n";
        break;
    case FVNetClientError_CannotSpeak: 
        std::cout << "Can't speak!\n";
        break;
    case FVNetClientError_ServerBusy:
        std::cout << "Server busy!\n";
        break;
    default:
        std::cout << "Error during speaking!\n";
    }
}

/** Tests the flexvoice client.
  */
int main(int argc, char* argv[])
{
    if( !processCommandLine( argc, argv ) )
        return 1;

    unsigned long handle = FVNetClientCreate();
    if( handle == 0 )
    {
        printf("Can't create to the server!\n" );
        return 0;
    }

    if( FVNetClientConnect( handle, g_host.c_str(), g_port ) != FVNetClientError_OK )
    {
        printf("Can't connect to the server:%s,%d\n", g_host.c_str(), g_port );
        return 0;
    }
    
    WaveFile    wave;

    int waveformat = 0;

    FVNetCSOutputFormat  format = FVNetCSOutputFormat_Invalid;
    int sampleFreq    = 0; 
    int bitResolution = 0;
    FVNetClientGetFormat( handle, format, sampleFreq, bitResolution );

    switch( format )
    {
    case FVNetCSOutputFormat_PCM_Signed     : waveformat = 1; break;
    case FVNetCSOutputFormat_PCM_Unsigned   : waveformat = 1; break;
    case FVNetCSOutputFormat_ULAW           : waveformat = 7; break;
    case FVNetCSOutputFormat_ALAW           : waveformat = 6; break;
    }

    if(  format == FVNetCSOutputFormat_VOX 
      || format == FVNetCSOutputFormat_VIS )
    {
        FILE * file = fopen( g_outputfile.c_str(), "wb" );

        FVNetClientSpeak( handle
                        , g_text.c_str()
                        , g_lang.c_str()
                        , callback_RAW, reinterpret_cast<void*>(file) );

        printError( FVNetClientWait( handle ) );

        fclose( file );
    }
    else
    {
        g_first          = true;
        g_firstDataTime  = 0;
        g_dataTime       = 0;
        g_bytesPerSec    = sampleFreq * bitResolution / 8;

        wave.open( g_outputfile.c_str(), waveformat, sampleFreq, bitResolution ); 

        FVNetClientSpeak( handle
                        , g_text.c_str()
                        , g_lang.c_str()
                        , callback, reinterpret_cast<void*>(&wave) );

        printError( FVNetClientWait( handle ) );

        wave.close();
    }

    FVNetClientDestroy( handle );
    return 0;
}

