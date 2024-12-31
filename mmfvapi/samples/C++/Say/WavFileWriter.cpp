/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#ifdef _MSC_VER
# pragma warning ( disable: 4786 )
#endif //_MSC_VER

#ifdef _MSC_VER
#include <windows.h>
#endif
#include "WavFileWriter.h"
#include "Tools.h"
#include <TTSAPI/FVVersion.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

WavFileWriter::WavFileWriter ( std::string text
                             , std::string outputFile
                             , const SpeechParams & params 
                             )
             : m_text        ( text )
             , m_outputFile  ( outputFile )
             , m_speechParams( params )
             , m_outputFormat( params.samplingFreq, params.bitResolution, params.coding )
             , m_fileFormat  ( params.fileformat )
             , m_output      ( m_outputFormat )
{
//    assert( !m_text.empty() && !m_outputFile.empty() );
}

WavFileWriter::~WavFileWriter()
{
    if( m_factory.get() != 0 )
        askPatience( "Shutting down" );
    delete m_engine.release();
    delete m_factory.release();
}

bool WavFileWriter::writeFile()
{
    bool ok = true;

    if( ok )
    {
        ok = createFactory();
    }

    if( ok )
    {
        ok = loadSpeakerList();
    }

    if( ok )
    {
        ok = addSpeakersToFactory();
    }

    if( ok )
    {
        ok = loadDefaultSpeaker();
    }

    if( ok )
    {
        ok = createEngine();
    }

    if( ok )
    {
        ok = addSpeakersToEngine();
    }

    if( ok )
    {
        ok = setParameters();
    }

    if( ok )
    {
        ok = testOutputFile();
    }
    
    if( ok )
    {
        ok = produceSpeech();
    }

    return ok;
}

void WavFileWriter::askPatience( std::string action )
{
    std::cout << action << " TTS engine factory, please wait...\n";
}

bool WavFileWriter::loadSpeakerList()
{
    const char * datapath = m_factory->getLangDataPath( m_speechParams.language );
    std::string speakerListFileName;
    speakerListFileName = datapath;
    speakerListFileName += "/VoiceList.tvl";

    try 
    {
        m_speakerList.load( speakerListFileName.c_str(), datapath );
    }
    catch( ... )
    {
        std::cout << "\nFatal error: cannot load speaker list from file " 
                  << speakerListFileName << std::endl;
        return false;
    }

    return true;
}

bool WavFileWriter::createFactory()
{
    askPatience("Initializing");

    try 
    {
        m_factory = std::auto_ptr<MM_TTSAPI::EngineFactory> (
            checkedptr( new MM_TTSAPI::EngineFactory( NULL ) ));
        m_factory->loadLanguage( m_speechParams.language );
    }
    catch( MM_TTSAPI::GenericException )
    {
        std::cout << "\nFatal error: cannot create TTS engine factory\n";
        return false;
    }

    return true;
}

bool WavFileWriter::addSpeakersToFactory()
{
    assert( m_factory.get() != 0 );

    bool ok = true;

    for( MM_TTSAPI::SpeakerMap::iterator it = m_speakerList.begin(); 
         it != m_speakerList.end(); ++it )
    {
        m_factory->addSpeaker( m_speechParams.language, *it, MM_TTSAPI::SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE );
    }

    return ok;
}

bool WavFileWriter::loadDefaultSpeaker()
{
    const char * datapath = m_factory->getLangDataPath( m_speechParams.language );
    std::string defSpeakerFileName = datapath;
    defSpeakerFileName += "/Default.tav";

    try
    {
        m_defSpeaker.load( defSpeakerFileName.c_str() );
    }
    catch( ... )
    {
        std::cout << "\nFatal error: cannot load default speaker from file " 
                  << defSpeakerFileName << std::endl;
        return false;
    }

    return true;
}

bool WavFileWriter::createEngine()
{
    try 
    {
        m_engine = m_factory->createEngine( &m_output, m_defSpeaker, 
                                            m_speechParams.language );
    }
    catch( MM_TTSAPI::GenericException ) 
    {
        std::cout << "\nFatal error: could not create TTS engine\n";
        return false;
    }

    assert( m_engine.get() );
    return true;
}

bool WavFileWriter::addSpeakersToEngine()
{
    assert( m_engine.get() != 0 );

    for( MM_TTSAPI::SpeakerMap::iterator it = m_speakerList.begin(); 
         it != m_speakerList.end(); ++it )
    {
        m_engine->addSpeaker( it.getKey(), *it
                            , MM_TTSAPI::SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE );
    }

    return true;
}


bool WavFileWriter::setParameters()
{
    std::string command;

    if( !m_speechParams.speaker.empty() )
    {
        command += "\\vce=speaker=\"";
        command += m_speechParams.speaker;
        command += "\"\\ ";
    }

    if( m_speechParams.speechRate > 0 )
    {
        std::stringstream strm;
        strm << m_speechParams.speechRate;
        command += "\\rspd=";
        command += strm.str();
        command += "\\ ";
    }

    if( m_speechParams.pitch > 0 )
    {
        std::stringstream strm;
        strm << m_speechParams.pitch;
        command += "\\pit=";
        command += strm.str();
        command += "\\ ";
    }

    if( m_speechParams.pitchRate > 0 )
    {
        std::stringstream strm;
        strm << m_speechParams.pitchRate;
        command += "\\rpit=";
        command += strm.str();
        command += "\\ ";
    }

    m_text.insert( 0, command );

    return true;
}
    
bool WavFileWriter::testOutputFile()
{
    std::ofstream file( m_outputFile.c_str(), std::ios::app );
    if( !file.good() )
    {
        std::cout << "\nFatal error: cannot open file " << m_outputFile << " for output\n";
        return false;
    }   
    return true;
}

bool WavFileWriter::produceSpeech()
{
    assert( m_engine.get() );

    m_output.open( m_outputFile.c_str(), m_fileFormat );

    std::cout << "Recording speech in file " << m_outputFile << " ...";
#ifndef _DEBUG
    try
#endif //!_DEBUG
    {
        m_engine->speakRequest( m_text.c_str(), 1 );
        m_engine->wait();
    }
#ifndef _DEBUG
    catch(...)
    {
        std::cout << "error\n";
        return false;
    }
#endif //!_DEBUG
    std::cout << "done\n";

    m_output.close();

    return true;
}

