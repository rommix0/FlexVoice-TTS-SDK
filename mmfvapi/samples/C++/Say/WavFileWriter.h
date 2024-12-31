/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#ifndef WAV_FILE_WRITER_H
#define WAV_FILE_WRITER_H

#include <string>
#include "SpeechParams.h"
#include <TTSAPI/Engine.h>
#include <TTSAPI/Speaker.h>
#include <TTSAPI/FileOutputSite.h>

class WavFileWriter {
public:
    WavFileWriter( std::string text, std::string outputFile, const SpeechParams & ); 
    ~WavFileWriter();

    bool writeFile();

private:

//     bool getEnvSettings();

    bool loadSpeakerList();
    
    bool createFactory();
    void askPatience( std::string );
    bool addSpeakersToFactory();
    
    bool createEngine();
    bool loadDefaultSpeaker();
    bool addSpeakersToEngine();

    bool setParameters();
    bool testOutputFile();
    bool produceSpeech();

    std::string                             m_text;
    std::string                             m_outputFile;
    SpeechParams                            m_speechParams;

    std::auto_ptr<MM_TTSAPI::EngineFactory> m_factory;
    std::auto_ptr<MM_TTSAPI::Engine>        m_engine;

    MM_TTSAPI::WaveOutputFormat                 m_outputFormat;
    MM_TTSAPI::WaveFileOutputSite::FileFormat   m_fileFormat;
    MM_TTSAPI::WaveFileOutputSite               m_output;

    MM_TTSAPI::SpeakerMap                   m_speakerList;
    MM_TTSAPI::Speaker                      m_defSpeaker;
};


#endif
