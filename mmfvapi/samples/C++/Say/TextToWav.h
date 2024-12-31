/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#ifndef TEXT_TO_WAV_H
#define TEXT_TO_WAV_H

#include <string>
#include "SpeechParams.h"

class TextToWav {
public:
    
    enum OutputMode { WAVE, PHSTR, TEXT };

    TextToWav( int argc, char** argv ); 
    ~TextToWav();

    bool parseCmdLine();
    bool prepareText();
    bool writeWav();

private:
    bool getPrgName();
    void stripPath( std::string& );
    void stripExtension( std::string& );

    bool parseOptions();
    
    bool parseInput();
    
    bool parseOutputFile();
    
    void showUsage();
    void readCmdLineInt   ( const char *, int & );

    int                     m_argc;
    char**                  m_argv;
    int                     m_currentArg;

    std::string             m_prgName;  // needed by showUsage() because the customer 
                                        // gets this .exe under a different name...
                            
    OutputMode              m_outputMode;
    std::string             m_text;
    std::string             m_inputFile;
    std::string             m_outputFile;
                            
    SpeechParams            m_speechParams;
    static const int        s_defSamplingFreq; 
    static const int        s_bitResolution; 
};

#endif
