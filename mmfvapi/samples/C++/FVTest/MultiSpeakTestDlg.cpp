// MultiSpeakTestDlg.cpp : implementation file
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "stdafx.h"
#include "MultiSpeakTest.h"
#include "MultiSpeakTestDlg.h"
#include "SpeakTestDialog.h"
#include <TTSAPI/Engine.h>

#include <atlbase.h>
#include <atlcom.h>

#define PATH_SEP_CHAR       '\\'

#include <objbase.h>
#include <initguid.h>
#include <speech.h>

struct __declspec(uuid("f546b340-c743-11cd-80e5-00aa003e4b50")) IAudio;

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestDlg dialog

#ifndef DATAPATH
#define DATAPATH    NULL
#endif

CMultiSpeakTestDlg::CMultiSpeakTestDlg(CWnd* pParent /*=NULL*/)
	              : CDialog(CMultiSpeakTestDlg::IDD, pParent)
                  , m_factory( DATAPATH )
{
	//{{AFX_DATA_INIT(CMultiSpeakTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiSpeakTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSpeakTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiSpeakTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiSpeakTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD_HUNGARIAN, OnButtonAddHungarian)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ENGLISH, OnButtonAddEnglish)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MALAY, OnButtonAddMalay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestDlg message handlers

BOOL CMultiSpeakTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

    CoInitializeEx( NULL, COINIT_MULTITHREADED );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMultiSpeakTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMultiSpeakTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMultiSpeakTestDlg::loadLanguage( MM_TTSAPI::Language lang )
{
    LangMap::iterator lit = m_langMap.find( lang );
    if( lit != m_langMap.end() )
        return;

    LangData& langdata = m_langMap[lang];

    m_factory.loadLanguage( lang );

    MM_TTSAPI::Speaker speaker;

    CString vlStr( m_factory.getLangDataPath( lang ) );
    vlStr += PATH_SEP_CHAR;
    vlStr += "VoiceList.tvl";

    CString defSpStr( m_factory.getLangDataPath( lang ) );
    defSpStr += PATH_SEP_CHAR;
    defSpStr += "Default.tav";

    langdata.m_speakerMap.load( vlStr, m_factory.getLangDataPath( lang ) );
    for( MM_TTSAPI::SpeakerMap::iterator it = langdata.m_speakerMap.begin(); it != langdata.m_speakerMap.end(); ++it )
    {
        m_factory.addSpeaker( lang, *it, MM_TTSAPI::SCS_LOAD_IMMEDIATELY_DO_NOT_DELETE );
    }
    langdata.m_defaultSpeaker.load( defSpStr );
}

void CMultiSpeakTestDlg::AddSpeakTest( MM_TTSAPI::Language lang, const char * text ) 
{
    loadLanguage( lang );

    CComPtr<IAudio> audio;
    audio.CoCreateInstance( L"AudioMixer3.AudioMixer" );

    SpeakTestDialog * dialog = new SpeakTestDialog( m_factory
                                                  , lang
                                                  , m_langMap[lang].m_defaultSpeaker
                                                  , m_langMap[lang].m_speakerMap
                                                  , audio 
                                                  , text
                                                  );
    m_dialogList.push_back(dialog);

    dialog->Create( IDD_DIALOG_SPEAK_TEST, this );
    dialog->ShowWindow( SW_SHOW );
}

void CMultiSpeakTestDlg::OnDestroy() 
{
    for( std::vector<SpeakTestDialog*>::iterator it = m_dialogList.begin(); it != m_dialogList.end(); ++it )
    {
        delete *it, *it = NULL;
    }
    m_dialogList.clear();

    CoUninitialize();

	CDialog::OnDestroy();
}

void CMultiSpeakTestDlg::OnButtonAddHungarian() 
{
    AddSpeakTest( MM_TTSAPI::getLangID("Hungarian"), "Ez egy próbaszöveg!" );
}

void CMultiSpeakTestDlg::OnButtonAddEnglish() 
{
    AddSpeakTest( MM_TTSAPI::getLangID("English:US"), "This is a test message!" );
}

void CMultiSpeakTestDlg::OnButtonAddMalay() 
{
    AddSpeakTest( MM_TTSAPI::getLangID("Malay"), "" );
}
