/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.06.26.                                                  *
*                                                                        *
*************************************************************************/

#include "WaveFile.h"

int prepareHeader( unsigned short format, int fs, int bitres );
int updateHeader ();

static int      g_datasize = 0;
static FILE*    g_file     = 0;


int waveopen( const char * filename, unsigned short format, int fs, int bitres )
{
	g_file = fopen( filename, "wb" );

    if( g_file == 0 )
        return 0;

    g_datasize = 0;

    return prepareHeader( format, fs, (format == 7) ? 8 : bitres );
}

int waveclose()
{
    int ret = updateHeader();
    fclose( g_file );
    return ret;
}

/** Writes WAV file header. Size datas shuold be updated later !
	@param fs sample frequency
	@param bitres sample resolution
    Returns 1 on success, otherwise 0.
*/
static int prepareHeader( unsigned short format_, int fs, int bitres )
{
    unsigned long  dataLong=0;
	unsigned short format       = format_;   /* 1 = PCM,  7 = MU-LAW */
    unsigned short channels     = 1;	/* Mono */
    unsigned short blockalign   = channels * bitres / 8;
    unsigned long  bytepersec   = blockalign * fs;
/* writting header */
    dataLong = 0x46464952; /* [RIFF] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;
    dataLong = 0;          /* file length */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;
    dataLong = 0x45564157; /* [WAVE] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;
    dataLong = 0x20746D66; /* [fmt ] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;

/* writting wave format */
    dataLong = 5*sizeof(unsigned short)+2*sizeof(unsigned long);/* sizeof( WAVEFORMATEX ); (=18) */
    if( fwrite( &dataLong         , sizeof(unsigned long ), 1, g_file ) != 1 ) return 0;
    
   
    
    if( fwrite( &format     , sizeof(unsigned short), 1, g_file ) != 1 ) return 0;
    if( fwrite( &channels   , sizeof(unsigned short), 1, g_file ) != 1 ) return 0;
    if( fwrite( &fs         , sizeof(unsigned long ), 1, g_file ) != 1 ) return 0;
    if( fwrite( &bytepersec , sizeof(unsigned long ), 1, g_file ) != 1 ) return 0;
    if( fwrite( &blockalign , sizeof(unsigned short), 1, g_file ) != 1 ) return 0;
    if( fwrite( &bitres     , sizeof(unsigned short), 1, g_file ) != 1 ) return 0;
    dataLong = 0;
    if( fwrite( &dataLong         , sizeof(unsigned short), 1, g_file ) != 1 ) return 0;

    dataLong = 0x61746164; /* [data] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;

    dataLong=0;  /* size!!!*/
    if( fwrite( &dataLong, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;

    return 1;
}

/** Performs coding of wave data (if necessary) and writes it to file.
	@data data to write
	@size size of data
	Returns 1 on success.
*/
int wavewrite( const unsigned char * data, int size )
{
	/* write data into file */
	if( fwrite( data, sizeof(unsigned char), size, g_file ) != (size_t)size )
		return 0;

	/* update size counter */
	g_datasize += size;
    return 1;
}


/** Updates the sizes in WAVE file header.
    Returns 1 on success, otherwise 0.
*/
static int updateHeader()
{
    unsigned long size = ftell( g_file );
	int pos = sizeof(unsigned long) /*RIFF*/
            + sizeof(unsigned long) /*length*/
            + sizeof(unsigned long) /*WAVE*/
            + sizeof(unsigned long) /*fmt*/
            + sizeof(unsigned long) /*sizeof(WAVEFORMATEX)*/
            + 5*sizeof(unsigned short)+2*sizeof(unsigned long)/*sizeof(WAVEFORMATEX)*/
            + sizeof(unsigned long) /*data*/
			;
    
			size -= 2*sizeof(unsigned long);
    fseek( g_file, sizeof(unsigned long), SEEK_SET );
    if( fwrite( &size, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;

    
            ;
    fseek( g_file, pos, SEEK_SET );
 
    size = g_datasize;
    if( fwrite( &size, sizeof(unsigned long), 1, g_file ) != 1 ) return 0;

    fseek( g_file, 0, SEEK_END );
    return 1;
}


