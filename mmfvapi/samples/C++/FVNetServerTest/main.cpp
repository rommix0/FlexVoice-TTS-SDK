/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.06.26.                                                  *
*                                                                        *
*************************************************************************/
#include <FVNetCS/FVNetServer.h>

#include <vector>
#include <iostream>

static FVNetCSOutputFormat  g_format        = FVNetCSOutputFormat_PCM_Signed;
static double               g_bytesPerSec   = 0.0;
static int                  g_sampleFreq    = 16000;
static int                  g_bitResolution = 16;

static bool usage()
{
    std::cout 
        << "Usage: FVNetServerTest [options] <port> <channels> <language list>\n"
        << "\n"
        << "  -sf {8|11|16|22}    Sampling frequency: 8, 11, 16 or 22 kHz. Default: 16\n"         
        << "  -br {8|16}          Bit resolution: 8, 16. Default: 16\n"         
        << "  -pcm_signed         Specifies the otuput format as signed PCM\n"
        << "  -pcm_unsigned       Specifies the otuput format as unsigned PCM\n"
        << "                      The default output format is signed PCM for 16 bit \n"
        << "                      output and unsigned PCM for 8 bit output.\n"
        << "  -ULAW               Specifies the otuput format as ULAW\n"
        << "  -ALAW               Specifies the otuput format as ALAW\n"
        << "  -VOX                Specifies the otuput format as VOX\n"
        << "  -VIS                Specifies the otuput format as VIS\n"
        << "  <port>              the port where the server should listen\n"
        << "  <channels>          the max count of the channes.\n"
        << "  <language list>     space separated language identifiers (eng hun malay)\n"
        << "                      at least one language should be specified\n"
        ;
    return false;
}

static bool processCommandLine( int argc, char* argv[]
                              , int& port, int& channels, std::vector<std::string>& languagelist )
{
    if( argc < 4 ) return usage();

    int param = 1;
    for( int i=1; i < argc; ++i )
    {
        if( strcmp( argv[i], "-pcm_signed" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_PCM_Signed;
        }
        else
        if( strcmp( argv[i], "-pcm_unsigned" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_PCM_Unsigned;
        }
        else
        if( strcmp( argv[i], "-ULAW" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_ULAW;
        }
        else
        if( strcmp( argv[i], "-ALAW" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_ALAW;
        }
        else
        if( strcmp( argv[i], "-VOX" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_VOX;
        }
        else
        if( strcmp( argv[i], "-VIS" ) == 0 )
        {
            g_format = FVNetCSOutputFormat_VIS;
        }
        else
        if( strcmp( argv[i], "-sf" ) == 0 )
        {
            if( ++i >= argc ) return usage();

            int freq = atoi( argv[i] );
            switch( freq )
            {
            case 8:  g_sampleFreq = 8000 ; break;
            case 11: g_sampleFreq = 11025; break;
            case 16: g_sampleFreq = 16000; break;
            case 22: g_sampleFreq = 22050; break;
            default:
                std::cout << "Invalid sampling frequency!" << std::endl;
                return false;
            }
        }
        else
        if( strcmp( argv[i], "-br" ) == 0 )
        {
            if( ++i >= argc ) return usage();

            int br = atoi( argv[i] );
            switch( br )
            {
            case 8:  g_bitResolution = 8 ; break;
            case 16: g_bitResolution = 16; break;
            default:
                std::cout << "Invalid bit resolution!" << std::endl;
                return false;
            }
        }
        else
        {
            switch( param++ )
            {
            case 1: 
                port = atoi( argv[i] ); 
                break;
            case 2: 
                channels = atoi( argv[i] ); 
                break;
            default:
                languagelist.push_back(argv[i]);
                break;
            }
        }
    }
    return true;
}

bool error( int err )
{
    switch( err )
    {
    case FVNetServerError_CannotInitEngine:
        std::cout << "Cannot initialize engine!\n";
        break;
    case FVNetServerError_CannotOpenPort:
        std::cout << "Cannot use port. Please select another!\n";
        break;
    }
    return false;
}

int main(int argc, char* argv[])
{
    int port;
    int channels;

    std::vector<std::string>    languagelist;

    if( !processCommandLine( argc, argv, port, channels, languagelist ) )
        return 0;

    unsigned long handle = FVNetServerCreate();
    if( handle == 0 )
    {
        printf( "Can't create server!\n" );
        return 0;
    }
    
    int err = FVNetServerError_OK;
    
    std::cout << "Initializing...\n";
    err = FVNetServerInitialize( handle, NULL, port, channels, g_format, g_sampleFreq, g_bitResolution );
    if( err != FVNetServerError_OK ) return error( err );

    for( int i=0; i < languagelist.size(); ++i )
    {
        std::cout << "Loading language:" << languagelist[i].c_str();
        err = FVNetServerLoadLanguage( handle, languagelist[i].c_str() );
        if( err != FVNetServerError_OK ) return error( err );
    }
    
    err = FVNetServerStartListening( handle );

    std::cout << "ready...\n";
    std::cout << "Press <Enter> to exit!\n";

    char ch;
    std::cin >> std::noskipws >> ch;

    std::cout << "Waiting for living channels...\n";
    FVNetServerDestroy( handle );
    
    std::cout << "Done.\n";
    return 0;
}
