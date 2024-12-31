/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.06.26.                                                  *
*                                                                        *
*************************************************************************/

#ifndef WAVE_FILE_H
#define WAVE_FILE_H

#include <cstdio>

class WaveFile
{
public:
            WaveFile();
    
    bool    open        ( const char * filename, int format, int fs, int bitres );
    bool    close       ();

    bool    writeData   ( const unsigned char * data, int size );

protected:

    bool    prepareHeader   ( unsigned short format, int fs, int bitres );
    bool    updateHeader    ();

    int             m_datasize;
    FILE*           m_file;
};

#endif // WAVE_FILE_H
