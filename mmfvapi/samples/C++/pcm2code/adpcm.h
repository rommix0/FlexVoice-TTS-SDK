/***********************************************************************
* Copyright (c) 2001 Mindmaker Ltd.  All rights reserved.              *
*                                                                      *
* Implementation file of 16 bit ADPCM coding                           *
*                                                                      *
************************************************************************
* File name    : adpcm.h                                               *
* Environment  : Win32                                                 *
* Compiler     : MSVC++ 6.0                                            *
************************************************************************/

/***********************************************************************  
 Description: Routines to convert 16 bit linear samples to the
 Dialogic ADPCM coding format (.vox).
 Reference: the book "PC Telephony - The
 complete guide to designing, building and programming systems
 using Dialogic and Related Hardware" by Bob Edgar. pg 272-276.
/***********************************************************************/

#ifndef _ADPCM_H_
#define _ADPCM_H_

#include <vector>

namespace Mindmaker
{

struct ADPCMStatus 
{
    short last;
    short step_index;
};

static short step_size[49] = { 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41,
     45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173,
     190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
     724, 796, 876, 963, 1060, 1166, 1282, 1408, 1552 };

inline void linear2adpcm(std::vector<short>& inputSamples, std::vector<unsigned char>& outputSamples, 
                         ADPCMStatus* coder_stat);
inline short adpcm_decode( char, ADPCMStatus *);
inline short step_adjust ( char );

//Initialze the data used by the coder. 
inline void adpcm_init( ADPCMStatus *stat ) 
{
  stat->last = 0;
  stat->step_index = 0;
  return;
}

//Encode linear to ADPCM
inline char adpcm_encode( short samp, ADPCMStatus *stat ) 
{
  short code;
  short diff, E, SS;

  SS = step_size[stat->step_index];
  code = 0x00;
  if( (diff = samp - stat->last) < 0 )
      code = 0x08;
  E = diff < 0 ? -diff : diff;
  if( E >= SS ) {
      code = code | 0x04;
      E -= SS;
  }
  if( E >= SS/2 ) {
      code = code | 0x02;
      E -= SS/2;
  }
  if( E >= SS/4 ) {
      code = code | 0x01;
  }

  //Use the decoder to set the estimate of last sample.
  //It also will adjust the step_index for us.
  stat->last = adpcm_decode((char) code, stat);
  return( (char) code );
}

// Decode Linear from ADPCM
inline short adpcm_decode( char code, ADPCMStatus *stat ) 
{  
  short diff, E, SS, samp;

  SS = step_size[stat->step_index];
  E = SS/8;
  if ( code & 0x01 )
      E += SS/4;
  if ( code & 0x02 )
      E += SS/2;
  if ( code & 0x04 )
      E += SS;
  diff = (code & 0x08) ? -E : E;
  samp = stat->last + diff;

  //Clip the values to +/- 2^11 (supposed to be 12 bits)
  if( samp > 2048 ) samp = 2048;
  if( samp < -2048 ) samp = -2048;

  stat->last = samp;
  stat->step_index += step_adjust( code );
  if( stat->step_index < 0 ) stat->step_index = 0;
  if( stat->step_index > 48 ) stat->step_index = 48;

  return( samp );
}

//adjust the step for use on the next sample.
inline short step_adjust ( char code ) 
{
  switch( code & 0x07 ) 
  {
    case 0x00:
        return(-1);
        break;
    case 0x01:
        return(-1);
        break;
    case 0x02:
        return(-1);
        break;
    case 0x03:
        return(-1);
        break;
    case 0x04:
        return(2);
        break;
    case 0x05:
        return(4);
        break;
    case 0x06:
        return(6);
        break;
    case 0x07:
        return(8);
        break;
    default:
        return(0); //Dummy: this is just to avoid compiler warnings. Never get here.
  }
}

//program to read and convert data to 12 bit format.
inline int read12(std::vector<short>& inputSamples, short *buffer12,int buffer_size)
{
    int i, n;

    //Read the next block of data;
    //Can use the 12-bit buffer to read the 16 bit data.
    n = buffer_size > inputSamples.size() ? inputSamples.size() : buffer_size;
    for(i=0; i < n; ++i)
    {
      buffer12[i] = (short) inputSamples[i];
    }
    inputSamples.erase(inputSamples.begin(), inputSamples.begin() + n);

    //Convert the 16 bit samples to 12 bit.
    //Note that n is the number of bytes read, which is twice
    //the number samples read because sizeof(short) == 2.
    for (i=0; i<n; i++) {
        buffer12[i] /= 16;
    }

    return(n);
}

/** Converts series of samples from linear to 16 bit adpcm (Dialogic .vox format).
    @param begin iterator pointing to the beginning of linear samples to be converted.
    @param end iterator pointing to the end of linear samples to be converted.
    @param out iterator pointing to the beginning of adpcm coded samples.
    Note that the calling procedure must ensure that enough place is available.
*/
inline void linear2adpcm(std::vector<short>& inputSamples, std::vector<unsigned char>& outputSamples, 
                         ADPCMStatus* coder_stat)
{
  const int buffer_size = 1024;
  int n;
  int i, j;

  //Allocate memory for the buffer of 12 bit data.
  std::vector<short> buffer12(buffer_size);
    
  //Allocate memory for the buffer of ADPCM samples.
  std::vector<char> adpcm(buffer_size/2);
  
  outputSamples.clear();
  while ((n=read12(inputSamples, &buffer12[0], buffer_size))>0 ) 
  {
      //Note that two ADPCM samples are stored in (8 bit) char,
      //because the ADPCM samples are only 4 bits.
      j = 0;
      for(i=0; i<n/2; i++) {
          adpcm[i] = adpcm_encode( buffer12[j++], coder_stat )<<4;
          if( j > n )  /* only true for last sample when n is odd */
              adpcm[i] |= adpcm_encode(0, coder_stat);
          else
              adpcm[i] |= adpcm_encode(buffer12[j++], coder_stat);
      }

      n /= 2;
      for(i=0; i < n; ++i)
      {
        outputSamples.push_back(adpcm[i]);
      }
  }
}

} //end of namespace Mindmaker

#endif //_ADPCM_H_