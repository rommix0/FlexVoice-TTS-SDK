// SpeakTestDialog.cpp : implementation file
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "stdafx.h"
#include "MultiSpeakTest.h"
#include "SpeakTestDialog.h"
#include <TTSAPI/Engine.h>
#include <TTSAPI/UserDictionary.h>
#include <string>
#include <sstream>

/////////////////////////////////////////////////////////////////////////////
// SpeakTestDialog dialog

using namespace MM_TTSAPI;

SpeakTestDialog::SpeakTestDialog( EngineFactory& factory
                                , Language       language
                                , const Speaker& speaker
                                , const SpeakerMap& speakerMap
                                , IAudio* audio
                                , const char * text
                                , CWnd* pParent /*=NULL*/ )
	           : CDialog(SpeakTestDialog::IDD, pParent)
               , m_factory( factory )
               , m_speakerMap( speakerMap )
               , m_outputFormat( 16000, 16, MM_TTSAPI::WaveOutputFormat::WC_PCM_SIGNED )
               , m_output( m_outputFormat, audio )
{
    m_engine = m_factory.createEngine( &m_output, speaker, language );

    m_output.registerNotify ( this
                            , MM_TTSAPI::BM_TEXT_BEGIN
                            | MM_TTSAPI::BM_TEXT_END
                            | MM_TTSAPI::BM_PARAGRAPH_BEGIN
                            | MM_TTSAPI::BM_PARAGRAPH_END  
                            | MM_TTSAPI::BM_SENTENCE_BEGIN 
                            | MM_TTSAPI::BM_SENTENCE_END   
                            | MM_TTSAPI::BM_ITEM_BEGIN     
                            | MM_TTSAPI::BM_ITEM_END       
                            | MM_TTSAPI::BM_WORD_BEGIN     
                            | MM_TTSAPI::BM_WORD_END       
                            | MM_TTSAPI::BM_PHONEME_BEGIN  
                            | MM_TTSAPI::BM_PHONEME_END    
                            | MM_TTSAPI::BM_IPAPHONEME_BEGIN  
                            | MM_TTSAPI::BM_IPAPHONEME_END    
                            | MM_TTSAPI::BM_SPEAKERCHANGED  
                            | MM_TTSAPI::BM_PARAMCHANGED
                            | MM_TTSAPI::BM_PAUSE
                            | MM_TTSAPI::BM_RESUME
                            | MM_TTSAPI::BM_START
                            | MM_TTSAPI::BM_STOP
                            | MM_TTSAPI::BM_REPEAT
                            | MM_TTSAPI::BM_PAUSE
                            | MM_TTSAPI::BM_RESUME
                            );
    m_output.open( m_outputFormat );

    for( MM_TTSAPI::SpeakerMap::const_iterator it = m_speakerMap.begin(); it != m_speakerMap.end(); ++it )
    {
        m_engine->addSpeaker( it.getKey(), *it, MM_TTSAPI::SCS_LOAD_IMMEDIATELY_DO_NOT_DELETE );
    }
    
    m_speaker = m_engine->getSpeaker();

    m_activeRequests = 0;
    m_paused   = false;
	//{{AFX_DATA_INIT(SpeakTestDialog)
	m_eventsParagraphDone = FALSE;
	m_eventsParagraphStart = FALSE;
	m_eventsPhonemeDone = FALSE;
	m_eventsPhonemeStart = FALSE;
	m_eventsSentenceDone = FALSE;
	m_eventsSentenceStart = FALSE;
	m_eventsTextDone = FALSE;
	m_eventsTextStart = FALSE;
	m_eventsTokenDone = FALSE;
	m_eventsTokenStart = FALSE;
	m_eventsWordDone = FALSE;
	m_eventsWordStart = FALSE;
	m_repeatCount = 1;
	m_text = text;
	m_eventsText = _T("");
	m_eventsPhraseDone = FALSE;
	m_eventsPhraseStart = FALSE;
	m_prepare = FALSE;
	m_bookmark = TRUE;
	m_skipCount = 1;
	m_speechRate      = 0;
	m_volume          = 0;
	m_defaultPitch    = 0;
	m_minPitch        = 0;
	m_maxPitch        = 0;
	m_pitchRate       = 0;
	m_intonationLevel = 0;
	m_headsize        = 0;
	m_richness        = 0;
	m_fricationRate   = 0;
	m_smoothness      = 0;
	m_speakerIndex    = 0;
	//}}AFX_DATA_INIT
}

SpeakTestDialog::~SpeakTestDialog()
{
    if( m_engine.get() != NULL )
        delete m_engine.release();
}

void SpeakTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SpeakTestDialog)
	DDX_Control(pDX, IDOK, m_buttonClose);
	DDX_Control(pDX, IDC_COMBO_SPEAKER, m_comboSpeaker);
	DDX_Control(pDX, IDC_EDIT_SPEECH_RATE            , m_editSpeechRate     );
	DDX_Control(pDX, IDC_EDIT_VOLUME                 , m_editVolume         );
	DDX_Control(pDX, IDC_EDIT_DEFAULT_PITCH          , m_editDefaultPitch   );
	DDX_Control(pDX, IDC_EDIT_MIN_PITCH              , m_editMinPitch       );
	DDX_Control(pDX, IDC_EDIT_MAX_PITCH              , m_editMaxPitch       );
	DDX_Control(pDX, IDC_EDIT_PITCH_RATE             , m_editPitchRate      );
	DDX_Control(pDX, IDC_EDIT_INTONATION_LEVEL       , m_editIntonationLevel);
	DDX_Control(pDX, IDC_EDIT_HEADSIZE               , m_editHeadsize       );
	DDX_Control(pDX, IDC_EDIT_RICHNESS               , m_editRichness       );
	DDX_Control(pDX, IDC_EDIT_FRICATION_RATE         , m_editFricationRate  );
	DDX_Control(pDX, IDC_EDIT_SMOOTHNESS             , m_editSmoothness     );
	DDX_Control(pDX, IDC_SLIDER_SPEECH_RATE          , m_sliderSpeechRate   );
	DDX_Control(pDX, IDC_SLIDER_VOLUME               , m_sliderVolume       );
	DDX_Control(pDX, IDC_SLIDER_DEFAULT_PITCH        , m_sliderDefaultPitch );
	DDX_Control(pDX, IDC_SLIDER_MIN_PITCH            , m_sliderMinPitch     );
	DDX_Control(pDX, IDC_SLIDER_MAX_PITCH            , m_sliderMaxPitch     );
	DDX_Control(pDX, IDC_SLIDER_PITCH_RATE           , m_sliderPitchRate    );
	DDX_Control(pDX, IDC_SLIDER_INTONATION_LEVEL     , m_sliderIntonationLevel);
	DDX_Control(pDX, IDC_SLIDER_HEADSIZE             , m_sliderHeadsize     );
	DDX_Control(pDX, IDC_SLIDER_RICHNESS             , m_sliderRichness     );
	DDX_Control(pDX, IDC_SLIDER_FRICATION_RATE       , m_sliderFricationRate);
	DDX_Control(pDX, IDC_SLIDER_SMOOTHNESS           , m_sliderSmoothness   );
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_BUTTON_PAUSE_RESUME, m_buttonPauseResume);
	DDX_Check(pDX, IDC_CHECK_PARAGRAPH_DONE, m_eventsParagraphDone);
	DDX_Check(pDX, IDC_CHECK_PARAGRAPH_START, m_eventsParagraphStart);
	DDX_Check(pDX, IDC_CHECK_PHONEME_DONE, m_eventsPhonemeDone);
	DDX_Check(pDX, IDC_CHECK_PHONEME_START, m_eventsPhonemeStart);
	DDX_Check(pDX, IDC_CHECK_SENTENCE_DONE, m_eventsSentenceDone);
	DDX_Check(pDX, IDC_CHECK_SENTENCE_START, m_eventsSentenceStart);
	DDX_Check(pDX, IDC_CHECK_TEXT_DONE, m_eventsTextDone);
	DDX_Check(pDX, IDC_CHECK_TEXT_START, m_eventsTextStart);
	DDX_Check(pDX, IDC_CHECK_TOKEN_DONE, m_eventsTokenDone);
	DDX_Check(pDX, IDC_CHECK_TOKEN_START, m_eventsTokenStart);
	DDX_Check(pDX, IDC_CHECK_WORD_DONE, m_eventsWordDone);
	DDX_Check(pDX, IDC_CHECK_WORD_START, m_eventsWordStart);
	DDX_Text(pDX, IDC_EDIT_REPEAT_COUNT, m_repeatCount);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	DDX_Control(pDX, IDC_EDIT_EVENTS, m_editEvents);
	DDX_Text(pDX, IDC_EDIT_EVENTS, m_eventsText);
	DDX_Check(pDX, IDC_CHECK_PHRASE_DONE, m_eventsPhraseDone);
	DDX_Check(pDX, IDC_CHECK_PHRASE_START, m_eventsPhraseStart);
	DDX_Check(pDX, IDC_CHECK_PREPARE, m_prepare);
	DDX_Check(pDX, IDC_CHECK_BOOKMARK, m_bookmark);
	DDX_Text(pDX, IDC_EDIT_SKIP_COUNT, m_skipCount);
	DDX_Text(pDX, IDC_EDIT_SPEECH_RATE            , m_speechRate      );
	DDX_Text(pDX, IDC_EDIT_VOLUME                 , m_volume          );
	DDX_Text(pDX, IDC_EDIT_DEFAULT_PITCH          , m_defaultPitch    );
	DDX_Text(pDX, IDC_EDIT_MIN_PITCH              , m_minPitch        );
	DDX_Text(pDX, IDC_EDIT_MAX_PITCH              , m_maxPitch        );
	DDX_Text(pDX, IDC_EDIT_PITCH_RATE             , m_pitchRate       );
	DDX_Text(pDX, IDC_EDIT_INTONATION_LEVEL       , m_intonationLevel );
	DDX_Text(pDX, IDC_EDIT_HEADSIZE               , m_headsize        );
	DDX_Text(pDX, IDC_EDIT_RICHNESS               , m_richness        );
	DDX_Text(pDX, IDC_EDIT_FRICATION_RATE         , m_fricationRate   );
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS             , m_smoothness      );
	DDX_CBIndex(pDX, IDC_COMBO_SPEAKER            , m_speakerIndex    );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SpeakTestDialog, CDialog)
	//{{AFX_MSG_MAP(SpeakTestDialog)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_RESUME, OnButtonPauseResume)
	ON_BN_CLICKED(IDC_BUTTON_EVENTS_CLEAR, OnButtonEventsClear)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_EVENTS, OnButtonSelectAllEvents)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL_EVENTS, OnButtonDeselectAllEvents)
	ON_BN_CLICKED(IDC_BUTTON_SKIP_SENTENCE_BACK, OnButtonSkipSentenceBack)
	ON_BN_CLICKED(IDC_BUTTON_SKIP_SENTENCE_FORWARD, OnButtonSkipSentenceForward)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SKIP_WORD_BACK, OnButtonSkipWordBack)
	ON_BN_CLICKED(IDC_BUTTON_SKIP_WORD_FORWARD, OnButtonSkipWordForward)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEAKER, OnSelchangeComboSpeaker)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SPEAK, OnButtonSpeak)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SpeakTestDialog message handlers

// INotify

void writeEvents( HWND hwnd, const char * text )
{
    int length = GetWindowTextLength( hwnd );
    if( length > 10000 )
    {
        SetWindowText( hwnd, text );
        //m_editEvents.SetWindowText( text );
    }
    else
    {
        SendMessage( hwnd, EM_SETSEL, length, length );
        SendMessage( hwnd, EM_REPLACESEL, FALSE, (LPARAM)text );
    }
}

void bookmarkText( const MM_TTSAPI::Bookmark& bookmark, std::string& text )
{
    switch( bookmark.type )
    {
    case MM_TTSAPI::BM_TEXT_BEGIN     : text += "TextStart"       ; break;
    case MM_TTSAPI::BM_TEXT_END       : text += "TextDone"        ; break;
    case MM_TTSAPI::BM_PARAGRAPH_BEGIN: text += "ParagraphStart"  ; break;
    case MM_TTSAPI::BM_PARAGRAPH_END  : text += "ParagraphDone"   ; break;
    case MM_TTSAPI::BM_SENTENCE_BEGIN : text += "SentenceStart"   ; break;
    case MM_TTSAPI::BM_SENTENCE_END   : text += "SentenceDone"    ; break;
    case MM_TTSAPI::BM_ITEM_BEGIN     : text += "TokenStart"      ; break;
    case MM_TTSAPI::BM_ITEM_END       : text += "TokenDone"       ; break;
    case MM_TTSAPI::BM_WORD_BEGIN     : text += "WordStart"       ; break;
    case MM_TTSAPI::BM_WORD_END       : text += "WordDone"        ; break;
    case MM_TTSAPI::BM_PHONEME_BEGIN  : text += "PhonemeStart"    ; break;
    case MM_TTSAPI::BM_PHONEME_END    : text += "PhonemeDone"     ; break;
    case MM_TTSAPI::BM_IPAPHONEME_BEGIN:text += "IPAPhonemeStart" ; break;
    case MM_TTSAPI::BM_IPAPHONEME_END : text += "IPAPhonemeDone"  ; break;
    case MM_TTSAPI::BM_SPEAKERCHANGED : text += "Speaker changed" ; break;
    case MM_TTSAPI::BM_PARAMCHANGED   : text += "Param changed"   ; break;
    case MM_TTSAPI::BM_START          : text += "Start"           ; break;
    case MM_TTSAPI::BM_STOP           : text += "Stop"            ; break;
    case MM_TTSAPI::BM_REPEAT         : text += "Repeat"          ; break;
    case MM_TTSAPI::BM_PAUSE          : text += "Pause"           ; break;
    case MM_TTSAPI::BM_RESUME         : text += "Resume"          ; break;
    }

    std::stringstream buff;
    buff << bookmark.pos << ',' << bookmark.len;

    text += "(";
    text += buff.str();
    text += ")";
}

void SpeakTestDialog::bookmarkProcess( const MM_TTSAPI::Bookmark& bookmark )
{
    std::string specInfo;
    
    switch( bookmark.type )
    {
    case MM_TTSAPI::BM_START:
        {
            ++m_activeRequests;
        }
        break;
    case MM_TTSAPI::BM_STOP:
        {
            --m_activeRequests;
        }
        break;
    case MM_TTSAPI::BM_PHONEME_BEGIN:
        {
            const MM_TTSAPI::BookmarkPhoneme &phonemeBookmark = dynamic_cast<const MM_TTSAPI::BookmarkPhoneme&>(bookmark);
            int phonemeCode     = phonemeBookmark.phonemeCode;
            int nextPhonemeCode = phonemeBookmark.nextPhonemeCode;
            std::ostringstream   ss;
            ss << " " << phonemeCode << "," << nextPhonemeCode;
            specInfo = ss.str();
        }
        break;
    case MM_TTSAPI::BM_IPAPHONEME_BEGIN:
        {
            const MM_TTSAPI::BookmarkIPA& ipaBookmark = dynamic_cast<const MM_TTSAPI::BookmarkIPA&>(bookmark);
            std::ostringstream   ss;
            const IPAPhoneme&   ipa1 = ipaBookmark.ipa;
            const IPAPhoneme&   ipa2 = ipaBookmark.nextipa;
            ss << " " << ipa1.m_type;
            switch( ipa1.m_type )
            {
            case IPAPhoneme::PTY_CONSONANT  : 
                ss << ',' << ipa1.m_consonantType;
                ss << ',' << ipa1.m_consonantPos;
                ss << ',' << ipa1.m_voicingState;
                break;
            case IPAPhoneme::PTY_VOWEL      :
                ss << ',' << ipa1.m_mouthPos;
                ss << ',' << ipa1.m_tonguePos;
                ss << ',' << ipa1.m_lipState;
                break;
            }
            ss << " " << ipa2.m_type;
            switch( ipa2.m_type )
            {
            case IPAPhoneme::PTY_CONSONANT  : 
                ss << ',' << ipa2.m_consonantType;
                ss << ',' << ipa2.m_consonantPos;
                ss << ',' << ipa2.m_voicingState;
                break;
            case IPAPhoneme::PTY_VOWEL      :
                ss << ',' << ipa2.m_mouthPos;
                ss << ',' << ipa2.m_tonguePos;
                ss << ',' << ipa2.m_lipState;
                break;
            }
            specInfo = ss.str();
        }
        break;
    case MM_TTSAPI::BM_SPEAKERCHANGED:
        {
            const MM_TTSAPI::BookmarkSpeakerChanged& bookmarkSpeaker
                = dynamic_cast<const MM_TTSAPI::BookmarkSpeakerChanged&>(bookmark);
            int i=0;
            ++i;
        }
        break;
    }

    if( !m_bookmark ) return;

    std::string text = "bookmark:";
    bookmarkText( bookmark, text );
    text += specInfo;
    text += "\r\n";

    writeEvents( m_editEvents, text.c_str() );

}

void SpeakTestDialog::prepareProcess( const MM_TTSAPI::Bookmark& bookmark )
{
    if( !m_prepare ) return;

    std::string text = "prepare :";
    bookmarkText( bookmark, text );
    text += "\r\n";
    writeEvents( m_editEvents, text.c_str() );
}

// CDialog

void SpeakTestDialog::setNotifyTypes()
{
    UpdateData( TRUE );
    OnButtonEventsClear();

    MM_TTSAPI::BookmarkTypeList  types;

    if( m_eventsTextStart       ) types.set( MM_TTSAPI::BM_TEXT_BEGIN      );
    if( m_eventsTextDone        ) types.set( MM_TTSAPI::BM_TEXT_END        );
    if( m_eventsParagraphStart  ) types.set( MM_TTSAPI::BM_PARAGRAPH_BEGIN );
    if( m_eventsParagraphDone   ) types.set( MM_TTSAPI::BM_PARAGRAPH_END   );
    if( m_eventsSentenceStart   ) types.set( MM_TTSAPI::BM_SENTENCE_BEGIN  );
    if( m_eventsSentenceDone    ) types.set( MM_TTSAPI::BM_SENTENCE_END    );
    if( m_eventsTokenStart      ) types.set( MM_TTSAPI::BM_ITEM_BEGIN      );
    if( m_eventsTokenDone       ) types.set( MM_TTSAPI::BM_ITEM_END        );
    if( m_eventsWordStart       ) types.set( MM_TTSAPI::BM_WORD_BEGIN      );
    if( m_eventsWordDone        ) types.set( MM_TTSAPI::BM_WORD_END        );
    if( m_eventsPhonemeStart    ) types.set( MM_TTSAPI::BM_PHONEME_BEGIN   );
    if( m_eventsPhonemeDone     ) types.set( MM_TTSAPI::BM_PHONEME_END     );
    if( m_eventsPhonemeStart    ) types.set( MM_TTSAPI::BM_IPAPHONEME_BEGIN);
    if( m_eventsPhonemeDone     ) types.set( MM_TTSAPI::BM_IPAPHONEME_END  );

    types.set( MM_TTSAPI::BM_START  );
    types.set( MM_TTSAPI::BM_STOP   );
    types.set( MM_TTSAPI::BM_REPEAT );
    types.set( MM_TTSAPI::BM_PAUSE  );
    types.set( MM_TTSAPI::BM_RESUME );
    types.set( MM_TTSAPI::BM_SPEAKERCHANGED );
    types.set( MM_TTSAPI::BM_PARAMCHANGED   );

    MM_TTSAPI::BookmarkTypeList  alltypes;
    alltypes.flip();

    m_output.removeBookmarkTypes( this, alltypes );
    m_output.addBookmarkTypes   ( this, types );
}

void SpeakTestDialog::OnButtonSpeak() 
{
    UpdateData( TRUE );
    setNotifyTypes();
    m_engine->speakRequest( m_text, m_repeatCount );
}

void SpeakTestDialog::OnButtonStop() 
{
    UpdateData( TRUE );

    m_paused = false;
    m_buttonPauseResume.SetWindowText( "Pause" );

    m_engine->stop();
}

void SpeakTestDialog::OnButtonPauseResume() 
{
    if( m_paused )
    {
        m_engine->resume();
        m_paused = false;
        m_buttonPauseResume.SetWindowText( "Pause" );
    }
    else
    {
        m_engine->pause();
        m_paused = true;
        m_buttonPauseResume.SetWindowText( "Resume" );
    }
}

void SpeakTestDialog::OnButtonEventsClear()
{
    m_editEvents.SetWindowText( "" );
}

void SpeakTestDialog::OnOK() 
{
    m_engine->stop();
    m_engine->wait();

    delete m_engine.release();

    m_output.close();
    
    stopNotifyLoop( false );

	CDialog::OnOK();
}

void setSliderRange( int& var, CSliderCtrl& slider, int pos, int min, int max  )
{
    var = pos;
    slider.SetRangeMin( min );
    slider.SetRangeMax( max );
    slider.SetPos     ( pos );
}

void SpeakTestDialog::setSliders( const MM_TTSAPI::Speaker& speaker )
{
    
    double  speechRate      = 1.0;
    double  volume          = 1.0;
    int     defaultPitch    = -1;
    int     pitchMin        = -1;
    int     pitchMax        = -1;
    double  pitchRate       = 1.0;
    double  intonationLevel = 1.0;
    double  headsize        = 1.0;
    double  richness        = 1.0;
    double  tilt            = 1.0;
    double  fricationRate   = 1.0;
    double  smoothness      = 1.0;

    speaker.get( "speechRate"     , speechRate      );
    speaker.get( "volume"         , volume          );
    speaker.get( "defaultPitch"   , defaultPitch    );
    speaker.get( "pitchMin"       , pitchMin        );
    speaker.get( "pitchMax"       , pitchMax        );
    speaker.get( "pitchRate"      , pitchRate       );
    speaker.get( "intonationLevel", intonationLevel );
    speaker.get( "headsize"       , headsize        );
    speaker.get( "richness"       , richness        );
    speaker.get( "tilt"           , tilt            );
    speaker.get( "fricationRate"  , fricationRate   );
    speaker.get( "smoothness"     , smoothness      );

    setSliderRange( m_speechRate    , m_sliderSpeechRate     , speechRate   * 100 ,  10, 400 );
    setSliderRange( m_volume        , m_sliderVolume         , volume       * 100 ,  10, 400 );
    setSliderRange( m_defaultPitch  , m_sliderDefaultPitch   , defaultPitch       ,  50, 500 );
    setSliderRange( m_minPitch      , m_sliderMinPitch       , pitchMin           ,  50, 500 );
    setSliderRange( m_maxPitch      , m_sliderMaxPitch       , pitchMax           ,  50, 500 );
    setSliderRange( m_pitchRate     , m_sliderPitchRate      , pitchRate    * 100 ,  10, 400 );
    setSliderRange( m_intonationLevel,m_sliderIntonationLevel, intonationLevel*100,  10, 400 );
    setSliderRange( m_headsize      , m_sliderHeadsize       , headsize     * 100 ,  30, 300 );
    setSliderRange( m_richness      , m_sliderRichness       , richness     * 100 ,   1, 100 );
    setSliderRange( m_fricationRate , m_sliderFricationRate  , fricationRate* 100 ,   0, 300 );
    setSliderRange( m_smoothness    , m_sliderSmoothness     , smoothness   * 100 ,   0, 100 );
}

BOOL SpeakTestDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int i=0;
    int act = 0;
    MM_TTSAPI::SpeakerMap::const_iterator it;
    m_speakerIndex = 0;
    for( it = m_speakerMap.begin(); it != m_speakerMap.end(); ++it, i++ )
    {
        m_comboSpeaker.AddString( it.getKey() );
        if( it.getKey() == "Default" )
        {
            m_speakerIndex = i;
        }
    }
	UpdateData( FALSE );

    it = m_speakerMap.find( "Default" );
    if( it == m_speakerMap.end() )
        it = m_speakerMap.begin();

    m_speaker = *it;
    m_engine->setSpeaker( m_speaker, true );

    setSliders( m_speaker );

	UpdateData( FALSE );
    return TRUE;
}

void SpeakTestDialog::OnDestroy() 
{
	CDialog::OnDestroy();
}

void SpeakTestDialog::OnClose() 
{
    m_engine->stop();
    m_engine->wait();

    delete m_engine.release();

    m_output.close();
    
    stopNotifyLoop( false );

	CDialog::OnClose();
}

void SpeakTestDialog::OnButtonSelectAllEvents() 
{
    UpdateData( TRUE );
	m_eventsParagraphDone   = TRUE;
	m_eventsParagraphStart  = TRUE;
	m_eventsPhonemeDone     = TRUE;
	m_eventsPhonemeStart    = TRUE;
	m_eventsSentenceDone    = TRUE;
	m_eventsSentenceStart   = TRUE;
	m_eventsTextDone        = TRUE;
	m_eventsTextStart       = TRUE;
	m_eventsTokenDone       = TRUE;
	m_eventsTokenStart      = TRUE;
	m_eventsWordDone        = TRUE;
	m_eventsWordStart       = TRUE;
    UpdateData( FALSE );
}

void SpeakTestDialog::OnButtonDeselectAllEvents() 
{
    UpdateData( TRUE );
	m_eventsParagraphDone   = FALSE;
	m_eventsParagraphStart  = FALSE;
	m_eventsPhonemeDone     = FALSE;
	m_eventsPhonemeStart    = FALSE;
	m_eventsSentenceDone    = FALSE;
	m_eventsSentenceStart   = FALSE;
	m_eventsTextDone        = FALSE;
	m_eventsTextStart       = FALSE;
	m_eventsTokenDone       = FALSE;
	m_eventsTokenStart      = FALSE;
	m_eventsWordDone        = FALSE;
	m_eventsWordStart       = FALSE;
    UpdateData( FALSE );
}

void SpeakTestDialog::OnButtonSkipSentenceBack() 
{
    UpdateData( TRUE );
    m_engine->skip( DU_SENTENCE, -m_skipCount );
}

void SpeakTestDialog::OnButtonSkipSentenceForward() 
{
    UpdateData( TRUE );
    m_engine->skip( DU_SENTENCE, m_skipCount );
}

void SpeakTestDialog::OnButtonSkipWordBack() 
{
    UpdateData( TRUE );
    m_engine->skip( DU_WORD, -m_skipCount );
}

void SpeakTestDialog::OnButtonSkipWordForward() 
{
    UpdateData( TRUE );
    m_engine->skip( DU_WORD, m_skipCount );
}

void SpeakTestDialog::setSpeechRate( int pos )
{
    m_speechRate = pos;
    m_sliderSpeechRate.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "speechRate", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setVolume( int pos )
{
    m_volume = pos;
    m_sliderVolume.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "volume", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setDefaultPitch( int pos )
{
    m_defaultPitch = pos;
    m_sliderDefaultPitch.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "defaultPitch", pos );
    if( m_defaultPitch < m_minPitch )
    {
        m_minPitch = pos;
        m_sliderMinPitch.SetPos( pos );
        m_speaker.set( "pitchMin", pos );
    }
    if( m_defaultPitch > m_maxPitch )
    {
        m_maxPitch = pos;
        m_sliderMaxPitch.SetPos( pos );
        m_speaker.set( "pitchMax", pos );
    }
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setMinPitch( int pos )
{
    m_minPitch = pos;
    m_sliderMinPitch.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "pitchMin", pos );
    if( m_minPitch > m_defaultPitch )
    {
        m_defaultPitch = pos;
        m_sliderDefaultPitch.SetPos( pos );
        m_speaker.set( "defaultPitch", pos );
    }
    if( m_minPitch > m_maxPitch )
    {
        m_maxPitch = pos;
        m_sliderMaxPitch.SetPos( pos );
        m_speaker.set( "pitchMax", pos );
    }
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setMaxPitch( int pos )
{
    m_maxPitch = pos;
    m_sliderMaxPitch.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "pitchMax", pos );
    if( m_maxPitch < m_defaultPitch )
    {
        m_defaultPitch = pos;
        m_sliderDefaultPitch.SetPos( pos );
        m_speaker.set( "defaultPitch", pos );
    }
    if( m_maxPitch < m_minPitch )
    {
        m_minPitch = pos;
        m_sliderMinPitch.SetPos( pos );
        m_speaker.set( "pitchMin", pos );
    }
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setPitchRate( int pos )
{
    m_pitchRate = pos;
    m_sliderPitchRate.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "pitchRate", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setIntonationLevel( int pos )
{
    m_intonationLevel = pos;
    m_sliderIntonationLevel.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "intonationLevel", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setHeadsize( int pos )
{
    m_headsize = pos;
    m_sliderHeadsize.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "headsize", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setRichness( int pos )
{
    m_richness = pos;
    m_sliderRichness.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "richness", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setFricationRate( int pos )
{
    m_fricationRate = pos;
    m_sliderFricationRate.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "fricationRate", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::setSmoothness( int pos )
{
    m_smoothness = pos;
    m_sliderSmoothness.SetPos( pos );
    m_speaker = m_engine->getSpeaker();
    m_speaker.set( "smoothness", pos/100.0 );
    m_engine->setSpeaker( m_speaker, true );
    UpdateData( FALSE );
}

void SpeakTestDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    int             pos=0;
    CSliderCtrl   * slider = (CSliderCtrl*)pScrollBar;

    pos = slider->GetPos();
    
	if( slider == &m_sliderSpeechRate      ) setSpeechRate      ( pos );
	if( slider == &m_sliderVolume          ) setVolume          ( pos );
	if( slider == &m_sliderDefaultPitch    ) setDefaultPitch    ( pos );
	if( slider == &m_sliderMinPitch        ) setMinPitch        ( pos );
	if( slider == &m_sliderMaxPitch        ) setMaxPitch        ( pos );
	if( slider == &m_sliderPitchRate       ) setPitchRate       ( pos );
	if( slider == &m_sliderIntonationLevel ) setIntonationLevel ( pos );
	if( slider == &m_sliderHeadsize        ) setHeadsize        ( pos );
	if( slider == &m_sliderRichness        ) setRichness        ( pos );
	if( slider == &m_sliderFricationRate   ) setFricationRate   ( pos );
	if( slider == &m_sliderSmoothness      ) setSmoothness      ( pos );

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void SpeakTestDialog::OnSelchangeComboSpeaker() 
{
    UpdateData( TRUE );
    
    char    buffer[100+1];
    m_comboSpeaker.GetLBText( m_speakerIndex, buffer );
    MM_TTSAPI::SpeakerMap::const_iterator it = m_speakerMap.find( buffer );
    if( it == m_speakerMap.end() )
        return;

    m_speaker = *it;
    m_speakerName = it.getKey();
    m_engine->setSpeaker( m_speaker, true );
    setSliders( *it );
    UpdateData( FALSE );
}

