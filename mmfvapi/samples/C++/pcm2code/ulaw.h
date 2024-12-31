/***********************************************************************
* Copyright (c) 2000 Mindmaker Ltd.  All rights reserved.              *
*                                                                      *
* header file of ulaw coding                                           *
*                                                                      *
************************************************************************
* File name    : ulaw.h                                                *
* Environment  : Win32                                                 *
* Compiler     : MSVC++ 6.0                                            *
************************************************************************/

#ifndef _ULAW_H
#define _ULAW_H

namespace Mindmaker
{

#define BIAS 0x84   /* define the add-in bias for 16 bit samples */
#define CLIP 32635

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4309 4305 )
#endif //_MSC_VER

/** This is the codebook for the ulaw -> signed 16-bit conversion.
*/
static short ulaw_to_s16[] = {
        0x8284, 0x8684, 0x8a84, 0x8e84, 0x9284, 0x9684, 0x9a84, 0x9e84,
        0xa284, 0xa684, 0xaa84, 0xae84, 0xb284, 0xb684, 0xba84, 0xbe84,
        0xc184, 0xc384, 0xc584, 0xc784, 0xc984, 0xcb84, 0xcd84, 0xcf84,
        0xd184, 0xd384, 0xd584, 0xd784, 0xd984, 0xdb84, 0xdd84, 0xdf84,
        0xe104, 0xe204, 0xe304, 0xe404, 0xe504, 0xe604, 0xe704, 0xe804,
        0xe904, 0xea04, 0xeb04, 0xec04, 0xed04, 0xee04, 0xef04, 0xf004,
        0xf0c4, 0xf144, 0xf1c4, 0xf244, 0xf2c4, 0xf344, 0xf3c4, 0xf444,
        0xf4c4, 0xf544, 0xf5c4, 0xf644, 0xf6c4, 0xf744, 0xf7c4, 0xf844,
        0xf8a4, 0xf8e4, 0xf924, 0xf964, 0xf9a4, 0xf9e4, 0xfa24, 0xfa64,
        0xfaa4, 0xfae4, 0xfb24, 0xfb64, 0xfba4, 0xfbe4, 0xfc24, 0xfc64,
        0xfc94, 0xfcb4, 0xfcd4, 0xfcf4, 0xfd14, 0xfd34, 0xfd54, 0xfd74,
        0xfd94, 0xfdb4, 0xfdd4, 0xfdf4, 0xfe14, 0xfe34, 0xfe54, 0xfe74,
        0xfe8c, 0xfe9c, 0xfeac, 0xfebc, 0xfecc, 0xfedc, 0xfeec, 0xfefc,
        0xff0c, 0xff1c, 0xff2c, 0xff3c, 0xff4c, 0xff5c, 0xff6c, 0xff7c,
        0xff88, 0xff90, 0xff98, 0xffa0, 0xffa8, 0xffb0, 0xffb8, 0xffc0,
        0xffc8, 0xffd0, 0xffd8, 0xffe0, 0xffe8, 0xfff0, 0xfff8, 0x0000,
        0x7d7c, 0x797c, 0x757c, 0x717c, 0x6d7c, 0x697c, 0x657c, 0x617c,
        0x5d7c, 0x597c, 0x557c, 0x517c, 0x4d7c, 0x497c, 0x457c, 0x417c,
        0x3e7c, 0x3c7c, 0x3a7c, 0x387c, 0x367c, 0x347c, 0x327c, 0x307c,
        0x2e7c, 0x2c7c, 0x2a7c, 0x287c, 0x267c, 0x247c, 0x227c, 0x207c,
        0x1efc, 0x1dfc, 0x1cfc, 0x1bfc, 0x1afc, 0x19fc, 0x18fc, 0x17fc,
        0x16fc, 0x15fc, 0x14fc, 0x13fc, 0x12fc, 0x11fc, 0x10fc, 0x0ffc,
        0x0f3c, 0x0ebc, 0x0e3c, 0x0dbc, 0x0d3c, 0x0cbc, 0x0c3c, 0x0bbc,
        0x0b3c, 0x0abc, 0x0a3c, 0x09bc, 0x093c, 0x08bc, 0x083c, 0x07bc,
        0x075c, 0x071c, 0x06dc, 0x069c, 0x065c, 0x061c, 0x05dc, 0x059c,
        0x055c, 0x051c, 0x04dc, 0x049c, 0x045c, 0x041c, 0x03dc, 0x039c,
        0x036c, 0x034c, 0x032c, 0x030c, 0x02ec, 0x02cc, 0x02ac, 0x028c,
        0x026c, 0x024c, 0x022c, 0x020c, 0x01ec, 0x01cc, 0x01ac, 0x018c,
        0x0174, 0x0164, 0x0154, 0x0144, 0x0134, 0x0124, 0x0114, 0x0104,
        0x00f4, 0x00e4, 0x00d4, 0x00c4, 0x00b4, 0x00a4, 0x0094, 0x0084,
        0x0078, 0x0070, 0x0068, 0x0060, 0x0058, 0x0050, 0x0048, 0x0040,
        0x0038, 0x0030, 0x0028, 0x0020, 0x0018, 0x0010, 0x0008, 0x0000
};
#ifdef _MSC_VER
#pragma warning( pop )
#endif //_MSC_VER

/** Performs coding form linear to ulaw.
    @param sample Input value of a linear, 16 bit sample.
    Returns the ulaw-coded sample.
*/
inline unsigned char linear2ulaw(int sample) {
        static int exp_lut[256] = {
                0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
                4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
        };
        int sign, exponent, mantissa;
        unsigned char ulawbyte;

        /* Get the sample into sign-magnitude. */
        sign = (sample >> 8) & 0x80;      /* set aside the sign  */
        if(sign != 0) sample = -sample;   /* get magnitude       */
        if(sample > CLIP) sample = CLIP;  /* clip the magnitude  */

        /* Convert from 16 bit linear to ulaw. */
        sample = sample + BIAS;
        exponent = exp_lut[( sample >> 7 ) & 0xFF];
        mantissa = (sample >> (exponent + 3)) & 0x0F;
        ulawbyte = ~(sign | (exponent << 4) | mantissa);
        return(ulawbyte);
}

/** Converts series of samples from linear to ulaw.
    @param begin iterator pointing to the beginning of linear samples to be converted.
    @param end iterator pointing to the end of linear samples to be converted.
    @param out iterator pointing to the beginning of ulaw coded samples.
    Note that the calling procedure must ensure that enough place is available.
*/
template<typename ShortIn, typename CharOut>
inline void linear2ulaw(ShortIn begin, ShortIn end, CharOut out)
{
  while(begin != end)
  {
    *out = linear2ulaw(*begin);
    begin++;
    out++;
  }
}

/** Performs ulaw to linear coding.
    @param ulaw Input value of ulaw coded sample.
    Returns the linear scales sample value.
*/
inline short ulaw2linear(unsigned char ulaw)
{
  return(ulaw_to_s16[ulaw]);
}


/** Converts series of samples from ulaw to linear.
    @param begin iterator pointing to the beginning of ulaw samples to be converted.
    @param end iterator pointing to the end of ulaw samples to be converted.
    @param out iterator pointing to the beginning of linear coded samples.
    Note that the calling procedure must ensure that enough place is available.
*/
template<typename CharIn, typename ShortOut>
inline void ulaw2linear(CharIn begin, CharIn end, ShortOut out)
{
  while(begin != end)
  {
    *out = ulaw2linear(*begin);
    begin++;
    out++;
  }
}

} //end of namespace Mindmaker

#endif //_ULAW_H
