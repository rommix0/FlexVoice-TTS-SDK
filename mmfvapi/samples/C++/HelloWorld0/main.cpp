/*************************************************************************
*                                                                        *
*  (C) Copyright 2001 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.02.07.                                                  *
*                                                                        *
*************************************************************************/
#ifdef _MSC_VER
# pragma warning ( disable: 4786 )
#endif //_MSC_VER

#include <string>
#include <iostream>
#include <TTSAPI/Engine.h>
#include <TTSAPI/Speaker.h>
#include <TTSAPI/FileOutputSite.h>

int main()
{
    MM_TTSAPI::EngineFactory factory( NULL );

    factory.loadLanguage( MM_TTSAPI::LNG_ENGLISH );
    std::string speakerFile = factory.getLangDataPath( MM_TTSAPI::LNG_ENGLISH );
    speakerFile += "/Default.tav";

    MM_TTSAPI::WaveOutputFormat    outputFormat( 16000, 16, MM_TTSAPI::WaveOutputFormat::WC_PCM_SIGNED );
    MM_TTSAPI::WaveFileOutputSite  output( outputFormat );

    MM_TTSAPI::Speaker defSpeaker;
    defSpeaker.load( speakerFile.c_str() );

    factory.addSpeaker( MM_TTSAPI::LNG_ENGLISH, defSpeaker, MM_TTSAPI::SCS_LOAD_IMMEDIATELY_DO_NOT_DELETE );

    output.open( "hello.wav", MM_TTSAPI::WaveFileOutputSite::FF_WAV );

    std::auto_ptr<MM_TTSAPI::Engine> engine 
        = factory.createEngine( &output, defSpeaker, MM_TTSAPI::LNG_ENGLISH );

    engine->speakRequest( "Hello World!", 1 );
    engine->wait();

    output.close();
    return 0;
}
