// MultiSpeakTest.cpp : Defines the class behaviors for the application.
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "stdafx.h"
#include "MultiSpeakTest.h"
#include "MultiSpeakTestDlg.h"
#include "TTSAPI/FVVersion.h"

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestApp

BEGIN_MESSAGE_MAP(CMultiSpeakTestApp, CWinApp)
	//{{AFX_MSG_MAP(CMultiSpeakTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestApp construction

CMultiSpeakTestApp::CMultiSpeakTestApp()
{
#ifdef _MSC_VER
    long breakalloc = 0;
    if( breakalloc > 0 )
        _CrtSetBreakAlloc( breakalloc );
#endif // _MSC_VER
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMultiSpeakTestApp object

CMultiSpeakTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestApp initialization

BOOL CMultiSpeakTestApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    if( strcmp( MM_TTSAPI::getVersion(), FLEX_VOICE_VERSION ) != 0 )
    {
        MessageBox( NULL, "Bad engine version!", NULL, MB_OK );
        exit(1);
    }

	CMultiSpeakTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
