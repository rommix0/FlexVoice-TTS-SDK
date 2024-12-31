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

#include <stdio.h>

extern int waveopen    ( const char * filename, unsigned short format, int fs, int bitres );
extern int waveclose   ();

extern int wavewrite   ( const unsigned char * data, int size );

#endif // WAVE_FILE_H
