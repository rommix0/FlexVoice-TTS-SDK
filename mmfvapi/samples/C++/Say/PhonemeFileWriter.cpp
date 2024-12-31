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
#include "PhonemeFileWriter.h"
#include "Tools.h"
#include <TTSAPI/FVVersion.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

/**********************************************************************************/

PhonemeOutputSite::PhonemeOutputSite()
                 : m_phonemeCoder( NULL )
{
}

PhonemeOutputSite::~PhonemeOutputSite()
{
    if( m_phonemeCoder != NULL )
        delete m_phonemeCoder, m_phonemeCoder = NULL;
}

const MM_TTSAPI::IOutputFormat& PhonemeOutputSite::getOutputFormat() const
{
    return m_outputFormat;
}

void PhonemeOutputSite::pause()
{
}

void PhonemeOutputSite::play()
{
}

void PhonemeOutputSite::clear()
{
}

void PhonemeOutputSite::setBookmark( MM_TTSAPI::Bookmark* bookmark )
{
    notifyAll( *bookmark, true );
    notifyAll( *bookmark, false );
}

void PhonemeOutputSite::sendBookmark( MM_TTSAPI::Bookmark& bookmark )
{
    notifyAll( bookmark, false );
}

void PhonemeOutputSite::put( const MM_TTSAPI::AnnPhoneme& phoneme )
{
    if( m_phonemeCoder == NULL )
        return;
    
    m_os << m_phonemeCoder->getSymbol( phoneme.code );
    m_os << '\t' << '(' << phoneme.code << ')';
    m_os << '\t' << phoneme.duration;
    int i;
    m_os << '\t';
    for( i=0; i < (int)phoneme.pitchContour.size(); ++i )
    {
        m_os << " <" << phoneme.pitchContour[i].first << ',' << phoneme.pitchContour[i].second << '>';
    }
    m_os << '\t';
    for( i=0; i < (int)phoneme.loudnessContour.size(); ++i )
    {
        m_os << " <" << phoneme.loudnessContour[i].first << ',' << phoneme.loudnessContour[i].second << '>';
    }
    m_os << '\n';
}

void PhonemeOutputSite::registerNotify( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types )
{
    m_notifyMap.insert( NotifyMap::value_type( notify, types ) );
}

void PhonemeOutputSite::unregisterNotify( MM_TTSAPI::INotify * notify )
{
    NotifyMap::iterator it = m_notifyMap.find( notify );
    if( it != m_notifyMap.end() )
    {
        m_notifyMap.erase( it );
    }
}

void PhonemeOutputSite::addBookmarkTypes( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types )
{
    NotifyMap::iterator it = m_notifyMap.find( notify );
    if( it != m_notifyMap.end() )
    {
        it->second |= types;
    }
}

void PhonemeOutputSite::removeBookmarkTypes( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types )
{
    NotifyMap::iterator it = m_notifyMap.find( notify );
    if( it != m_notifyMap.end() )
    {
        it->second &= ~types;
    }
}

void PhonemeOutputSite::open( const char * filename )
{
    m_os.open( filename );
}

void PhonemeOutputSite::close()
{
    m_os.close();
}

void PhonemeOutputSite::setPhonemeCoder( const MM_TTSAPI::IPhonemeCoder& phonemeCoder )
{
    m_phonemeCoder = phonemeCoder.clone();
}

void PhonemeOutputSite::notifyAll( const MM_TTSAPI::Bookmark& bookmark_, bool prepare_ )
{
    for( NotifyMap::iterator it = m_notifyMap.begin(); it != m_notifyMap.end(); ++it )
    {
        if( it->second.test( bookmark_.type ) )
        {
#ifndef _DEBUG
            try
#endif //!_DEBUG
            {
                if( prepare_ )
                    it->first->prepare( bookmark_ );
                else
                    it->first->bookmark( bookmark_ );
            }
#ifndef _DEBUG
            catch(...)
            {
            }
#endif //!_DEBUG
        }
    }
}

/**********************************************************************************/


PhFileWriter::PhFileWriter( std::string text, std::string outputFile
                          , const SpeechParams & params )
            : m_text        ( text )
            , m_outputFile  ( outputFile )
            , m_speechParams( params )
            , m_output      ()
{
    assert( !m_text.empty() && !m_outputFile.empty() );
}

PhFileWriter::~PhFileWriter()
{
    if( m_factory.get() != 0 )
        askPatience( "Shutting down" );
    delete m_engine.release();
    delete m_factory.release();
}

bool PhFileWriter::writeFile()
{
    bool ok = true;

    if( ok )
    {
        ok = createFactory();
    }

    if( ok )
    {
        ok = loadDefaultSpeaker();
    }

    if( ok )
    {
        ok = addSpeakersToFactory();
    }

    if( ok )
    {
        ok = createEngine();
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

void PhFileWriter::askPatience( std::string action )
{
    std::cout << action << " TTS engine factory, please wait...\n";
}

bool PhFileWriter::createFactory()
{
    askPatience("Initializing");

    try 
    {
        m_factory = std::auto_ptr<MM_TTSAPI::EngineFactory> (
            checkedptr( new MM_TTSAPI::EngineFactory( NULL )) );
        m_factory->loadLanguage( m_speechParams.language );
    }
    catch( MM_TTSAPI::GenericException )
    {
        std::cout << "\nFatal error: cannot create TTS engine factory\n";
        return false;
    }

    return true;
}

bool PhFileWriter::addSpeakersToFactory()
{
    assert( m_factory.get() != 0 );

    m_factory->addSpeaker( m_speechParams.language, m_defSpeaker, MM_TTSAPI::SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE );

    return true;
}


bool PhFileWriter::createEngine()
{
    try 
    {
        m_engine = m_factory->createEngine( &m_output, m_defSpeaker, 
                                            m_speechParams.language );
        m_output.setPhonemeCoder( m_engine->getPhonemeCoder() );
    }
    catch( MM_TTSAPI::GenericException ) 
    {
        std::cout << "\nFatal error: could not create TTS engine\n";
        return false;
    }

    assert( m_engine.get());
    return true;
}

bool PhFileWriter::loadDefaultSpeaker()
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

bool PhFileWriter::setParameters()
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
    
bool PhFileWriter::testOutputFile()
{
    std::ofstream file( m_outputFile.c_str(), std::ios::app );
    if( !file.good() )
    {
        std::cout << "\nFatal error: cannot open file " << m_outputFile << " for output\n";
        return false;
    }   

    return true;
}

bool PhFileWriter::produceSpeech()
{
    assert( m_engine.get() );

    m_output.open( m_outputFile.c_str() );

    std::cout << "Recording speech in file " << m_outputFile << " ...";
    m_engine->speakRequest( m_text.c_str(), 1 );
    m_engine->wait();
    std::cout << "done\n";

    m_output.close();

    return true;
}

