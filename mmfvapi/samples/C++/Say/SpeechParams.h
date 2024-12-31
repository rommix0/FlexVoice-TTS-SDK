/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#ifndef SPEECH_PARAMS_H
#define SPEECH_PARAMS_H

#include <TTSAPI/Engine.h>
#include <TTSAPI/FileOutputSite.h>

class SpeechParams 
{
public:
    SpeechParams();

    MM_TTSAPI::Language language;
    std::string     speaker;
    int             samplingFreq;
    int             bitResolution;
    int             speechRate;   // percent value
    int             pitch;
    int             pitchRate;    // percent value
    
    MM_TTSAPI::WaveOutputFormat::WaveCoding     coding;
    MM_TTSAPI::WaveFileOutputSite::FileFormat   fileformat;
};

inline SpeechParams::SpeechParams()
                   : samplingFreq   ( 0 )
                   , bitResolution  ( 0 )
                   , speechRate     ( 0 )
                   , pitch          ( 0 )
                   , pitchRate      ( 0 )
                   , coding         ( MM_TTSAPI::WaveOutputFormat::WC_INVALID )
                   , fileformat     ( MM_TTSAPI::WaveFileOutputSite::FF_WAV   )
{
}

#endif
