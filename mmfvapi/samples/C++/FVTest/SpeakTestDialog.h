#if !defined(AFX_SPEAKTESTDIALOG_H__76B138D1_8558_11D4_93F9_0040F6341449__INCLUDED_)
#define AFX_SPEAKTESTDIALOG_H__76B138D1_8558_11D4_93F9_0040F6341449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeakTestDialog.h : header file
//

#include <TTSAPI/Engine.h>
#include <TTSAPI/PlayOutputSite.h>
#include <TTSAPI/PlayOutputSiteEx.h>
#include <TTSAPI/Speaker.h>

/////////////////////////////////////////////////////////////////////////////
// SpeakTestDialog dialog

struct IAudio;

class SpeakTestDialog : public CDialog
                      , public MM_TTSAPI::NotifyTransfer
{
// Construction
public:
	SpeakTestDialog ( MM_TTSAPI::EngineFactory& factory
                    , MM_TTSAPI::Language       language
                    , const MM_TTSAPI::Speaker& speaker
                    , const MM_TTSAPI::SpeakerMap& speakerMap
                    , IAudio* audio
                    , const char * text
                    , CWnd* pParent = NULL );   // standard constructor
    ~SpeakTestDialog();

// INotifyTransfer
    virtual void        bookmarkProcess( const MM_TTSAPI::Bookmark& bookmark );
    virtual void        prepareProcess ( const MM_TTSAPI::Bookmark& bookmark );
    
// SpeakTestDialog
    void                setNotifyTypes();

// Dialog Data
	//{{AFX_DATA(SpeakTestDialog)
	enum { IDD = IDD_DIALOG_SPEAK_TEST };
	CButton	m_buttonClose;
	CComboBox	m_comboSpeaker;
	CEdit	m_editSpeechRate     ;
	CEdit	m_editVolume         ;
	CEdit	m_editDefaultPitch   ;
	CEdit	m_editMinPitch       ;
	CEdit	m_editMaxPitch       ;
	CEdit	m_editPitchRate      ;
	CEdit	m_editIntonationLevel;
	CEdit	m_editHeadsize       ;
	CEdit	m_editRichness       ;
	CEdit	m_editFricationRate  ;
	CEdit	m_editSmoothness     ;
	CSliderCtrl	m_sliderSpeechRate   ;
	CSliderCtrl	m_sliderVolume       ;
	CSliderCtrl	m_sliderDefaultPitch ;
	CSliderCtrl	m_sliderMinPitch     ;
	CSliderCtrl	m_sliderMaxPitch     ;
	CSliderCtrl	m_sliderPitchRate    ;
	CSliderCtrl	m_sliderIntonationLevel;
	CSliderCtrl	m_sliderHeadsize     ;
	CSliderCtrl	m_sliderRichness     ;
	CSliderCtrl	m_sliderFricationRate;
	CSliderCtrl	m_sliderSmoothness   ;
	CEdit	m_editText;
	CButton	m_buttonPauseResume;
	BOOL	m_eventsParagraphDone;
	BOOL	m_eventsParagraphStart;
	BOOL	m_eventsPhonemeDone;
	BOOL	m_eventsPhonemeStart;
	BOOL	m_eventsSentenceDone;
	BOOL	m_eventsSentenceStart;
	BOOL	m_eventsTextDone;
	BOOL	m_eventsTextStart;
	BOOL	m_eventsTokenDone;
	BOOL	m_eventsTokenStart;
	BOOL	m_eventsWordDone;
	BOOL	m_eventsWordStart;
	int		m_repeatCount;
	CString	m_text;
	CEdit	m_editEvents;
	CString	m_eventsText;
	BOOL	m_eventsPhraseDone;
	BOOL	m_eventsPhraseStart;
	BOOL	m_prepare;
	BOOL	m_bookmark;
	int		m_skipCount       ;
	int		m_speechRate      ;
	int		m_volume          ;
	int		m_defaultPitch    ;
	int		m_minPitch        ;
	int		m_maxPitch        ;
	int		m_pitchRate       ;
	int		m_intonationLevel ;
	int		m_headsize        ;
	int		m_richness        ;
	int		m_fricationRate   ;
	int		m_smoothness      ;
	int		m_speakerIndex    ;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SpeakTestDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    void    setSliders        ( const MM_TTSAPI::Speaker& speaker );

    void    setSpeechRate     ( int pos );
    void    setVolume         ( int pos );
    void    setDefaultPitch   ( int pos );
    void    setMinPitch       ( int pos );
    void    setMaxPitch       ( int pos );
    void    setPitchRate      ( int pos );
    void    setIntonationLevel( int pos );
    void    setHeadsize       ( int pos );
    void    setRichness       ( int pos );
    void    setFricationRate  ( int pos );
    void    setSmoothness     ( int pos );
    
    MM_TTSAPI::EngineFactory&           m_factory;
    std::auto_ptr<MM_TTSAPI::Engine>    m_engine;
    MM_TTSAPI::Speaker                  m_speaker;
    std::string                         m_speakerName;
    const MM_TTSAPI::SpeakerMap&        m_speakerMap;

    MM_TTSAPI::WaveOutputFormat         m_outputFormat;
    MM_TTSAPI::WavePlayOutputSiteEx     m_output;

    int                                 m_activeRequests;
    bool                                m_paused;

	// Generated message map functions
	//{{AFX_MSG(SpeakTestDialog)
	afx_msg void OnButtonSpeakStop();
	afx_msg void OnButtonPauseResume();
	afx_msg void OnButtonEventsClear();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnButtonSelectAllEvents();
	afx_msg void OnButtonDeselectAllEvents();
	afx_msg void OnButtonSkipSentenceBack();
	afx_msg void OnButtonSkipSentenceForward();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSkipWordBack();
	afx_msg void OnButtonSkipWordForward();
	afx_msg void OnSelchangeComboSpeaker();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonSpeak();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPEAKTESTDIALOG_H__76B138D1_8558_11D4_93F9_0040F6341449__INCLUDED_)
