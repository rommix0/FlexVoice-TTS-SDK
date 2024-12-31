/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.04.17.                                                  *
*                                                                        *
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TTSAPI/FVFarmServer.h>
#include "WaveFile.h"

#define SAMPLE_FREQ     16000
#define BIT_RESOLUTION  16

/** Prints the usage of this executable.
*/
void printUsage()
{
	printf("\nFVServerTest [options]<input text> [output file]\n\n");
	printf("options:\n\n");
	printf("<input text>    text to say (default is \"hello world\")\n");
	printf("<output file>   output file name (default is \".\\sample.wav\")\n");
	printf("-lang           language: <eng,hun,malay> (REQUIRED)\n");
	printf("-sf             sampling frequency\n");
	printf("-ulaw           use MU-LAW encoding instead of PCM\n");
	printf("-alaw           use A-LAW encoding instead of PCM\n");
	printf("-vox            use VOX (ADPCM16) encoding instead of PCM\n");
	printf("-vis            use VIS encoding instead of PCM\n");
}


/** Performs command line processing.
	@param argc count of command line parameters
	@param argv array of command line parameters
	@param format it is updated with the required format of output wave.
	@param inputtext it is updated with text to speech
	@param outputfile it is updated with the name of output file
	Returns TRUE if the arguments were valid, otherwise FALSE.
*/
int  processCommandLine( int argc, char* argv[]
                       , const char**    language
                       , unsigned short* waveformat
                       , int*            dataformat
                       , int*            sampleFreq
                       , char*           inputtext
                       , char*           outputfile
                       )
{
	int state       = 0;
    int currentArg  = 1;
    int stop        = 0;
    int ok          = 1;

	*language   = NULL;
    *waveformat = 1;	/* default is PCM */
    *dataformat = FVServerOutputFormat_PCM_Signed;
    strcpy( inputtext, "hello world" );
    *sampleFreq = SAMPLE_FREQ;

    if( ok )
    {
        while( currentArg < argc && !stop)
        {
            const char *arg = argv[currentArg];
            if( arg[0] == '-' || arg[0] == '/' )
            {
                const char *flag = arg+1;
                if( strcmp( flag, "sf" ) == 0 )
                {
                    if( ++currentArg < argc )
                    {
                        int sf = atoi( argv[currentArg] );
                        ++currentArg;
                        switch( sf )
                        {
                        case 8:  *sampleFreq = 8000 ; break;
                        case 11: *sampleFreq = 11025; break;
                        case 16: *sampleFreq = 16000; break;
                        case 22: *sampleFreq = 22050; break;
                        default: 
                            ok   = 0; 
                            stop = 1;
                        }
                    }
                    else
                    {
                        ok   = 0;
                        stop = 1;
                    }
                }
                else if( strcmp( flag, "pcm_signed" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_PCM_Signed;
                    *waveformat = 1;
                }
                else if( strcmp( flag, "pcm_unsigned" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_PCM_Unsigned;
                    *waveformat = 1;
                }
                else if( strcmp( flag, "ulaw" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_ULAW;
                    *waveformat = 7;
                }
                else if( strcmp( flag, "alaw" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_ALAW;
                    *waveformat = 6;
                }
                else if( strcmp( flag, "vox" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_VOX;
                    *waveformat = 0;
                }
                else if( strcmp( flag, "vis" ) == 0 )
                {
                    ++currentArg;
                    *dataformat = FVServerOutputFormat_VIS;
                    *waveformat = 0;
                }
                else if( strcmp( flag, "lang" ) == 0 )
                {
                    if( ++currentArg < argc )
                    {
                        *language = argv[currentArg];
                        ++currentArg;
                    }
                    else
                    {
                        ok   = 0;     // unknown flag
                        stop = 1;
                    }
                }
                else
                {
                    ok   = 0;     // unknown flag
                    stop = 1;
                }
            }
            else
            {
                switch( state )
                {
                case 0: strncpy( inputtext , arg, 1000 ), inputtext [1000] = '\0'; break;
                case 1: strncpy( outputfile, arg, 1000 ), outputfile[1000] = '\0'; break;
                }
                ++currentArg;
                ++state;
            }    
        }
    }

    if( *language == NULL )
    {
        printf( "-lang parameter is required!\n" );
        ok = 0;
    }
    
    if( !ok )
    {
        printUsage();
        return 0;
    }

    switch( *dataformat )
    {
    case FVServerOutputFormat_VOX: strcpy( outputfile, ".\\sample.vox" ); break;
    case FVServerOutputFormat_VIS: strcpy( outputfile, ".\\sample.vis" ); break;
    default                      : strcpy( outputfile, ".\\sample.wav" ); break;
    }

	return 1;
}


/*******************************************************************************/

/** Callback function passed to FlexVoice Server.
*/
int put( void * param, const unsigned char * data, int size )
{
    wavewrite( data, size );
    return 1;
}

/** Callback function passed to FlexVoice Server (for RAW formats).
*/
int put_RAW( void * param, const unsigned char * data, int size )
{
    FILE * file = (FILE*)param;
    fwrite( data, size, 1, file );
    return 1;
}



/** Errors messages from FlexVoice Server.
*/
int error( int code )
{
    switch( code )
    {
    case FVServerError_AlreadyInitialized: printf( "Already initialized!\n"); return 1;
    case FVServerError_NotInitialized    : printf( "Not initialized!\n"    ); return 1;
    case FVServerError_CannotInitialize  : printf( "Can't initialize!\n"   ); return 1;
    case FVServerError_CannotUninitialize: printf( "Can't uninitialize!\n" ); return 1;
    case FVServerError_LanguageNotLoaded : printf( "Language not loaded!\n"); return 1;
    case FVServerError_CannotLoadLanguage: printf( "Can't load language!\n"); return 1;
    case FVServerError_CannotSpeak       : printf( "Can't speak!\n"        ); return 1;
    }
    return 0;
}


/** Program starts here.
*/
int main(int argc, char* argv[])
{
	char inputtext[1000+1], outputfile[1000+1];
    int err=0;

	unsigned short wformat = 1;
    const char* language;
	int dformat = FVServerOutputFormat_PCM_Unsigned;
    int sampleFreq;
    if( processCommandLine(argc, argv, &language, &wformat, &dformat, &sampleFreq, inputtext, outputfile ) )
	{
		err = FVServerInitialize( NULL );
        if( err == FVServerError_OK )
		    err = FVServerLoadLanguage( language, 1, dformat, sampleFreq, BIT_RESOLUTION );
        if( err == FVServerError_OK )
        {
		    if(  dformat == FVServerOutputFormat_VOX 
              || dformat == FVServerOutputFormat_VIS
              )
            {
        	    FILE * file = fopen( outputfile, "wb" );
                err = FVServerSpeak( inputtext, language, put_RAW, (void*)file );
                if( err == FVServerError_OK )
		            err = FVServerWait();
                fclose( file );
            }
            else
            {
                waveopen( outputfile, wformat, sampleFreq, BIT_RESOLUTION );
                err = FVServerSpeak( inputtext, language, put, NULL );
                if( err == FVServerError_OK )
		            err = FVServerWait();
                waveclose();
            }

            if( err == FVServerError_OK )
		        err = FVServerUninitialize();
        }
	}
    error(err);
    return 0;
}

