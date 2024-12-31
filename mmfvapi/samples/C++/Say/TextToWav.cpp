/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.28.                                                  *
*                                                                        *
*************************************************************************/
#ifdef _MSC_VER
# pragma warning ( disable: 4786 )
#endif //_MSC_VER

#include "TextToWav.h"
#include "WavFileWriter.h"
#include "PhonemeFileWriter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

const int   TextToWav::s_defSamplingFreq = 16000; 
const int   TextToWav::s_bitResolution   = 16; 

TextToWav::TextToWav( int argc, char** argv )
:   m_argc( argc ),
    m_argv( argv ),
    m_currentArg( 0 ),
    m_outputMode( WAVE )
{
    assert( argc >= 1 && argv != 0 );
}

TextToWav::~TextToWav()
{
}

bool TextToWav::parseCmdLine()
{
    bool ok = true;

    if( ok )
    {
        ok = getPrgName();
    }

    if( ok )
    {
        m_currentArg = 1;
        ok = parseOptions();
    }

    if( ok )
    {
        ok = parseInput();
    }

    if( ok )
    {
        ok = parseOutputFile();
    }

    if( !ok )
        showUsage();
    
    return ok;
}

bool TextToWav::getPrgName()
{
    bool ok = true;

    m_prgName = m_argv[0];
    stripPath( m_prgName );
    stripExtension( m_prgName );

    return ok;
}

void TextToWav::stripPath( std::string& s )
{
    static std::string pathSeparators( "\\/" );
    std::string::size_type lastSep = s.find_last_of( pathSeparators );
    if( lastSep != std::string::npos )
    {
        s.erase( 0, lastSep+1 );
    }    
}

void TextToWav::stripExtension( std::string& s )
{
    std::string::size_type lastDot = s.find_last_of( '.' );
    if( lastDot != std::string::npos )
    {
        s.erase( lastDot );
    }    
}


bool TextToWav::parseOptions()
{
    bool ok = true;
    bool hasLanguage = false;

    if( ok )
    {
        bool stop = false;
        while( m_currentArg < m_argc && !stop)
        {
            const char *arg = m_argv[m_currentArg];
            if( arg[0] == '-' || arg[0] == '/' )
            {
                const char *flag = arg+1;
                if( strcmp( flag, "lang" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        m_speechParams.language = MM_TTSAPI::getLangID( m_argv[m_currentArg] );
                        if( m_speechParams.language == MM_TTSAPI::LNG_INVALID )
                        {
                            ok = false;
                            stop = true;
                        }
                        else
                        {
                            hasLanguage = true;
                        }
                        ++m_currentArg;
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "phout" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        m_outputMode = PHSTR;
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "speaker" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        m_speechParams.speaker = m_argv[m_currentArg];
                        ++m_currentArg;
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "sf" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        int sf = 0;
                        readCmdLineInt( m_argv[m_currentArg], sf );
                        ++m_currentArg;
                        switch( sf )
                        {
                        case 8:  m_speechParams.samplingFreq = 8000; break;
                        case 11: m_speechParams.samplingFreq = 11025; break;
                        case 16: m_speechParams.samplingFreq = 16000; break;
                        case 22: m_speechParams.samplingFreq = 22050; break;
                        default: 
                            ok = false; 
                            stop = true;
                        }
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "br" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        int br = 0;
                        readCmdLineInt( m_argv[m_currentArg], br );
                        ++m_currentArg;
                        switch( br )
                        {
                        case 8:  m_speechParams.bitResolution = br; break;
                        case 16: m_speechParams.bitResolution = br; break;
                        default:
                            ok = false; 
                            stop = true;
                        }
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "raw" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.fileformat = MM_TTSAPI::WaveFileOutputSite::FF_RAW;
                }
                else if( strcmp( flag, "pcm_signed" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_PCM_SIGNED;
                }
                else if( strcmp( flag, "pcm_unsigned" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_PCM_UNSIGNED;
                }
                else if( strcmp( flag, "ulaw" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_ULAW;
                }
                else if( strcmp( flag, "alaw" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_ALAW;
                }
                else if( strcmp( flag, "vox" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_VOX;
                    m_speechParams.fileformat = MM_TTSAPI::WaveFileOutputSite::FF_RAW;
                }
                else if( strcmp( flag, "vis" ) == 0 )
                {
                    ++m_currentArg;
                    m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_VIS;
                    m_speechParams.fileformat = MM_TTSAPI::WaveFileOutputSite::FF_RAW;
                }
                else if( strcmp( flag, "speechrate" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        int sr = 0;
                        readCmdLineInt( m_argv[m_currentArg], sr );
                        ++m_currentArg;
                        if( 0 < sr )
                        {
                            m_speechParams.speechRate = sr;
                        }
                        else
                        {
                            ok = false;
                            stop = true;
                        }
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "pitch" ) == 0 )
                {
                    if( ++m_currentArg < m_argc )
                    {
                        int p = 0;
                        readCmdLineInt( m_argv[m_currentArg], p );
                        ++m_currentArg;
                        if( p > 0 )
                        {
                            m_speechParams.pitch = p;
                        }
                        else
                        {
                            ok = false;
                            stop = true;
                        }
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "pitchrate" ) == 0 ) 
                {
                    if( ++m_currentArg < m_argc )
                    {
                        int pr;
                        readCmdLineInt( m_argv[m_currentArg], pr );
                        ++m_currentArg;
                        if( 0 < pr )
                        {
                            m_speechParams.pitchRate = pr;
                        }
                        else
                        {
                            ok = false;
                            stop = true;
                        }
                    }
                    else
                    {
                        ok = false;
                        stop = true;
                    }
                }
                else if( strcmp( flag, "f" ) == 0 )
                {
                    stop = true;    // leave for next parsing phase
                }
                else
                {
                    ok = false;     // unknown flag
                    stop = true;
                }
            }
            else
            {
                stop = true;
            }    
        }
    }

    if( !hasLanguage )
        ok = false;

    if( m_speechParams.coding == MM_TTSAPI::WaveOutputFormat::WC_INVALID )
    {
        m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_PCM_SIGNED;
        if( m_speechParams.bitResolution == 8 )
        {
            m_speechParams.coding = MM_TTSAPI::WaveOutputFormat::WC_PCM_UNSIGNED;
        }
    }

    return ok;
}

bool TextToWav::parseInput()
{
    bool ok = true;

    if( m_currentArg >= m_argc )
        ok = false;

    if( ok )
    {
        const char *arg = m_argv[m_currentArg];
        if( arg[0] == '-' || arg[0] == '/' )
        {
            const char *flag = arg+1;
            if( strcmp( flag, "f" ) == 0 )
            {
                if( ++m_currentArg < m_argc )
                {
                    m_inputFile = m_argv[m_currentArg];
                    ++m_currentArg;
                }
                else
                {
                    ok = false;
                }
            }
            else
            {
                ok = false;
            }
        }
        else
        {
            m_text = m_argv[m_currentArg];        
            ++m_currentArg;
        }
    }

    return ok;
}

bool TextToWav::parseOutputFile()
{
    bool ok = true;

    if( m_currentArg < m_argc )
    {
        m_outputFile = m_argv[m_currentArg];
        if( ++m_currentArg < m_argc )
        {
            ok = false;     // nothing should follow the output
        }
    }
    else
    {
      if (m_outputMode == PHSTR)
        m_outputFile = "phout.txt";
      else 
      {
        switch( m_speechParams.fileformat )
        {
        case MM_TTSAPI::WaveFileOutputSite::FF_WAV:
            m_outputFile = "sound.wav";
            break;
        default:
            switch( m_speechParams.coding )
            {
            case MM_TTSAPI::WaveOutputFormat::WC_VOX:
                m_outputFile = "sound.vox"; 
                break;
            case MM_TTSAPI::WaveOutputFormat::WC_VIS:
                m_outputFile = "sound.vis"; 
                break;
            default: 
                m_outputFile = "sound.pcm"; 
                break;
            }
            break;
        }
      }
    }

    return ok;
}

void TextToWav::showUsage()
{
    std::cout 
        << "Creates wave file or annotated phoneme string from given text\n"
        << "Usage: " << m_prgName << " -lang language [options] { text | -f input_file } [output_file]\n"
        << "\n"
        << "  Options:\n"
        << "    -phout              Create phoneme string output\n"         
        << "    -speaker name       Specifies the speaker\n"         
        << "    -sf {8|11|16|22}    Sampling frequency: 8, 11, 16 or 22 kHz. Default: 16\n"         
        << "    -br {8|16}          Bit resolution: 8, 16. Default: 16\n"         
        << "    -raw                Specifies the file format as raw (without header)\n"
        << "    -pcm_signed         Specifies the otuput format as signed PCM\n"
        << "    -pcm_unsigned       Specifies the otuput format as unsigned PCM\n"
        << "    -ulaw               Specifies the otuput format as ULAW\n"
        << "    -alaw               Specifies the otuput format as ALAW\n"
        << "    -vox                Specifies the otuput format as VOX\n"
        << "    -vis                Specifies the otuput format as VIS\n"
        << "                        The default output format is signed PCM for 16 bit \n"
        << "                        output and unsigned PCM for 8 bit output.\n"
        << "    -speechrate value   Speech rate in percents\n"         
        << "    -pitch value        Default pitch of speech in Hz\n"         
        << "    -pitchrate value    Pitch rate in percents\n"         
        << "\n"
        << "  language: one of \"eng\", \"hun\", or \"malay\"\n"
        << "\n"
        << "  text: a single word or text in quotes\n"
        << "\n"
        << "  If output_file is not specified, it defaults to sound.wav\n";
}

void TextToWav::readCmdLineInt( const char * intext, int & num )
{
    std::stringstream strm( intext );

    strm >> num;
}

bool TextToWav::prepareText()
{
    bool ok = true;

    if( ok )
    {
        if( !m_inputFile.empty() )
        {
            std::ifstream file( m_inputFile.c_str() );
            if( file.good() ) 
            {
                m_text.erase();
                for( unsigned char ch = file.get(); file.good() && !file.eof(); ch = file.get() )
                    m_text += ch;
            }
            else 
            {
                ok = false;
                std::cout << "Fatal error: cannot open input file " << m_inputFile << "\n";
            }
        }
    }

    return ok;
}

bool TextToWav::writeWav()
{
    bool ok = true;

    if( m_speechParams.samplingFreq  == 0 ) 
        m_speechParams.samplingFreq = s_defSamplingFreq;
    if( m_speechParams.bitResolution == 0 ) 
        m_speechParams.bitResolution = s_bitResolution;

    switch( m_outputMode )
    {
    case WAVE:
        {
#ifndef _DEBUG
            try
#endif //!_DEBUG
            {
                WavFileWriter wavWriter( m_text, m_outputFile, m_speechParams );
                ok = wavWriter.writeFile();
            }
#ifndef _DEBUG
            catch(...)
            {
                std::cout << "error\n";
            }
#endif //!_DEBUG
        }
        break;
    case PHSTR:
        {
            PhFileWriter phWriter( m_text, m_outputFile, m_speechParams );
            ok = phWriter.writeFile();
        }
        break;
    }
    
    return ok;
}
