/*************************************************************************
*                                                                        *
*  (C) Copyright 2001 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.02.07.                                                  *
*                                                                        *
*************************************************************************/
#ifndef SPEECH_PARAMS_H
#define SPEECH_PARAMS_H

class SpeechParams {
public:
    SpeechParams();

    std::string     speaker;
    size_t          samplingFreq;
    size_t          speechRate;   // percent value
    size_t          pitch;
    size_t          pitchRate;    // percent value
};

inline SpeechParams::SpeechParams()
:   samplingFreq( 0 ),
    speechRate( 0 ),
    pitch( 0 ),
    pitchRate( 0 )
{
}

#endif
