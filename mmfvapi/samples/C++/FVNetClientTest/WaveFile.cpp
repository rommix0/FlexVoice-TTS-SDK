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

WaveFile::WaveFile()
        : m_datasize( 0 )
        , m_file    ( 0 )
{
}

bool WaveFile::open( const char * filename, int format, int fs, int bitres )
{
	m_file = fopen( filename, "wb" );

    if( m_file == 0 )
        return false;

    m_datasize = 0;

    return prepareHeader( format, fs, (format == 7) ? 8 : bitres );
}

bool WaveFile::close()
{
    bool ret = updateHeader();
    fclose( m_file );
    return ret;
}

/** Writes WAV file header. Size datas shuold be updated later !
	@param fs sample frequency
	@param bitres sample resolution
    Returns true on success, otherwise false.
*/
bool WaveFile::prepareHeader( unsigned short format_, int fs, int bitres )
{
    unsigned long  dataLong=0;
	unsigned short format       = format_;   /* 1 = PCM,  7 = MU-LAW */
    unsigned short channels     = 1;	/* Mono */
    unsigned short blockalign   = channels * bitres / 8;
    unsigned long  bytepersec   = blockalign * fs;
/* writting header */
    dataLong = 0x46464952; /* [RIFF] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;
    dataLong = 0;          /* file length */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;
    dataLong = 0x45564157; /* [WAVE] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;
    dataLong = 0x20746D66; /* [fmt ] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;

/* writting wave format */
    dataLong = 5*sizeof(unsigned short)+2*sizeof(unsigned long);/* sizeof( WAVEFORMATEX ); (=18) */
    if( fwrite( &dataLong         , sizeof(unsigned long ), 1, m_file ) != 1 ) return false;
    
   
    
    if( fwrite( &format     , sizeof(unsigned short), 1, m_file ) != 1 ) return false;
    if( fwrite( &channels   , sizeof(unsigned short), 1, m_file ) != 1 ) return false;
    if( fwrite( &fs         , sizeof(unsigned long ), 1, m_file ) != 1 ) return false;
    if( fwrite( &bytepersec , sizeof(unsigned long ), 1, m_file ) != 1 ) return false;
    if( fwrite( &blockalign , sizeof(unsigned short), 1, m_file ) != 1 ) return false;
    if( fwrite( &bitres     , sizeof(unsigned short), 1, m_file ) != 1 ) return false;
    dataLong = 0;
    if( fwrite( &dataLong         , sizeof(unsigned short), 1, m_file ) != 1 ) return false;

    dataLong = 0x61746164; /* [data] */
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;

    dataLong=0;  /* size!!!*/
    if( fwrite( &dataLong, sizeof(unsigned long), 1, m_file ) != 1 ) return false;

    return true;
}

/** Performs coding of wave data (if necessary) and writes it to file.
	@data data to write
	@size size of data
	Returns true on success.
*/
bool WaveFile::writeData( const unsigned char * data, int size )
{
	/* write data into file */
	if( fwrite( data, sizeof(unsigned char), size, m_file ) != (size_t)size )
		return false;

	/* update size counter */
	m_datasize += size;
    return true;
}


/** Updates the sizes in WAVE file header.
    Returns true on success, otherwise false.
*/
bool WaveFile::updateHeader()
{
    unsigned long size = ftell( m_file );
	int pos = sizeof(unsigned long) /*RIFF*/
            + sizeof(unsigned long) /*length*/
            + sizeof(unsigned long) /*WAVE*/
            + sizeof(unsigned long) /*fmt*/
            + sizeof(unsigned long) /*sizeof(WAVEFORMATEX)*/
            + 5*sizeof(unsigned short)+2*sizeof(unsigned long)/*sizeof(WAVEFORMATEX)*/
            + sizeof(unsigned long) /*data*/
			;
    
			size -= 2*sizeof(unsigned long);
    fseek( m_file, sizeof(unsigned long), SEEK_SET );
    if( fwrite( &size, sizeof(unsigned long), 1, m_file ) != 1 ) return false;

    
            ;
    fseek( m_file, pos, SEEK_SET );
 
    size = m_datasize;
    if( fwrite( &size, sizeof(unsigned long), 1, m_file ) != 1 ) return false;

    fseek( m_file, 0, SEEK_END );
    return true;
}


