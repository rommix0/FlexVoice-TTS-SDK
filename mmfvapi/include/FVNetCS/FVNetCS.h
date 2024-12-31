/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2002.02.04.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_FVNETCS_H
#define MM_FVNETCS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @name FVNetCS.h
  */
//@{

/** The supported output formats.
  */
enum FVNetCSOutputFormat{ /** Invalid output format.
                            * Used wen the format is not yet specified.
                            */
                          FVNetCSOutputFormat_Invalid,
                          /** Signed PCM.
                            * The output data continuously contains the PCM samples.
                            * The sample size depends on the bit resolution
                            * The samples should be interpreted as singed values
                            */
                          FVNetCSOutputFormat_PCM_Signed,
                          /** Unsigned PCM.
                            * The same as \Ref{FVNetCSOutputFormat_PCM_Signed} but
                            * the samples are unsigned values. In case of 8 bit output
                            * this format is often used.
                            */
                          FVNetCSOutputFormat_PCM_Unsigned,
                          /** ULAW.
                            * The output data contains the samples in a coded format.
                            * The bit resolution should be set to 16 that will be coded
                            * to 8 bit.
                            */
                          FVNetCSOutputFormat_ULAW,
                          /** ALAW.
                            * The output data contains the samples in a coded format.
                            * The bit resolution should be set to 16 that will be coded
                            * to 8 bit.
                            */
                          FVNetCSOutputFormat_ALAW, 
                          /** VOX.
                            * The output data contains the samples in a coded format.
                            * The bit resolution should be set to 16 that will be coded
                            * to 8 bit.
                            */
                          FVNetCSOutputFormat_VOX,
                          /** VIS.
                            * The output data contains the samples in a coded format.
                            * The bit resolution should be set to 16
                            */
                          FVNetCSOutputFormat_VIS
                        };

//@}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MM_FVNETCS_H
