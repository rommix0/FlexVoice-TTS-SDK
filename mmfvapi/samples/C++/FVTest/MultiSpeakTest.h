// MultiSpeakTest.h : main header file for the MULTISPEAKTEST application
//

#if !defined(AFX_MULTISPEAKTEST_H__76B138C7_8558_11D4_93F9_0040F6341449__INCLUDED_)
#define AFX_MULTISPEAKTEST_H__76B138C7_8558_11D4_93F9_0040F6341449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestApp:
// See MultiSpeakTest.cpp for the implementation of this class
//

class CMultiSpeakTestApp : public CWinApp
{
public:
	CMultiSpeakTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSpeakTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMultiSpeakTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CString g_dataFolder;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISPEAKTEST_H__76B138C7_8558_11D4_93F9_0040F6341449__INCLUDED_)
