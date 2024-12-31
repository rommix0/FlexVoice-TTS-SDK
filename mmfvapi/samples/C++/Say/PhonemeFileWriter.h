/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#ifndef PHONEME_FILE_WRITER_H
#define PHONEME_FILE_WRITER_H

#include <string>
#include <fstream>
#include "SpeechParams.h"
#include <TTSAPI/Engine.h>
#include <TTSAPI/Speaker.h>
#include <TTSAPI/FileOutputSite.h>
#include <map>


class PhonemeOutputSite : public MM_TTSAPI::FakeAttribute<MM_TTSAPI::IAnnPhonemeOutputSite>
{
public:
// IOutputSite
                        PhonemeOutputSite();
                        ~PhonemeOutputSite();

    virtual const MM_TTSAPI::IOutputFormat&    getOutputFormat() const;

    virtual void        pause();
    virtual void        play();
    virtual void        clear();

    virtual void        setBookmark ( MM_TTSAPI::Bookmark* bookmark );
    virtual void        sendBookmark( MM_TTSAPI::Bookmark& bookmark );

// IAnnPhonemeOutputSite
    virtual void                        put( const MM_TTSAPI::AnnPhoneme& phoneme );

// INotifyDispatcher
    virtual void    registerNotify      ( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types );
    virtual void    unregisterNotify    ( MM_TTSAPI::INotify * notify                                );
    virtual void    addBookmarkTypes    ( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types );
    virtual void    removeBookmarkTypes ( MM_TTSAPI::INotify * notify, const MM_TTSAPI::BookmarkTypeList& types );

// PhonemeOutputSite

    void            open( const char * filename );
    void            close();
    void            setPhonemeCoder( const MM_TTSAPI::IPhonemeCoder& phonemeCoder );

protected:
    void            notifyAll( const MM_TTSAPI::Bookmark& bookmark_, bool prepare_ );

    typedef std::map<MM_TTSAPI::INotify*, MM_TTSAPI::BookmarkTypeList>    NotifyMap;

    std::ofstream       m_os;

    MM_TTSAPI::AnnPhonemeOutputFormat   m_outputFormat;
    NotifyMap                           m_notifyMap;
    MM_TTSAPI::IPhonemeCoder*           m_phonemeCoder;
};

class PhFileWriter 
{
public:
    PhFileWriter( std::string text, std::string outputFile, const SpeechParams & ); 
    ~PhFileWriter();

    bool writeFile();

private:

    bool getEnvSettings();

    bool createFactory();
    void askPatience( std::string );
    bool addSpeakersToFactory();
    
    bool createEngine();
    bool loadDefaultSpeaker();

    bool setParameters();
    bool testOutputFile();
    bool produceSpeech();

    std::string                             m_text;
    std::string                             m_outputFile;
    SpeechParams                            m_speechParams;

    std::auto_ptr<MM_TTSAPI::EngineFactory> m_factory;

    MM_TTSAPI::Speaker                      m_defSpeaker;

    std::auto_ptr<MM_TTSAPI::Engine>        m_engine;
    PhonemeOutputSite                       m_output;
};


#endif
