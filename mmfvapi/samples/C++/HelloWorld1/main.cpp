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
    using namespace MM_TTSAPI;

    try
    {
        EngineFactory factory( NULL );

        factory.loadLanguage( MM_TTSAPI::LNG_ENGLISH );
        std::string dataDir = factory.getLangDataPath( MM_TTSAPI::LNG_ENGLISH );

        std::string speakerFile     = dataDir + "/Default.tav";
        std::string speakerListFile = dataDir + "/VoiceList.tvl";

        SpeakerMap speakerList;
        speakerList.load( speakerListFile.c_str(), dataDir.c_str() );
        SpeakerMap::iterator it;
        for( it = speakerList.begin(); it != speakerList.end(); ++it )
        {
            factory.addSpeaker( MM_TTSAPI::LNG_ENGLISH, *it, SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE );
        }

        Speaker defSpeaker;
        defSpeaker.load( speakerFile.c_str() );

        WaveOutputFormat    outputFormat( 16000, 16, WaveOutputFormat::WC_PCM_SIGNED );
        WaveFileOutputSite  output( outputFormat );

        output.open( "hello.wav", MM_TTSAPI::WaveFileOutputSite::FF_WAV );

        std::auto_ptr<Engine> engine = factory.createEngine( &output, defSpeaker, LNG_ENGLISH );

        for( it = speakerList.begin(); it != speakerList.end(); ++it )
        {
            engine->addSpeaker( it.getKey(), *it, SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE );
        }

        engine->speakRequest( "\\vce=speaker=\"Ben\"\\ Hello World!", 1 );
        engine->wait();

        output.close();
    }
    catch(GenericException& e)
    {
        std::cout << e.what   () << '\n';
        std::cout << e.details() << '\n';
        return 1;
    }
    return 0;
}
