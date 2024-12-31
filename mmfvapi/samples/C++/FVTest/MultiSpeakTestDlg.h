// MultiSpeakTestDlg.h : header file
//

#if !defined(AFX_MULTISPEAKTESTDLG_H__76B138C9_8558_11D4_93F9_0040F6341449__INCLUDED_)
#define AFX_MULTISPEAKTESTDLG_H__76B138C9_8558_11D4_93F9_0040F6341449__INCLUDED_

#include <TTSAPI/Engine.h>

#include <vector>
#include <map>
#include "SpeakTestDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultiSpeakTestDlg dialog

struct LangData
{
    MM_TTSAPI::Speaker          m_defaultSpeaker;
    MM_TTSAPI::SpeakerMap       m_speakerMap;
};

class CMultiSpeakTestDlg : public CDialog
{
// Construction
public:
	CMultiSpeakTestDlg(CWnd* pParent = NULL);	// standard constructor

	void AddSpeakTest( MM_TTSAPI::Language lang, const char * text );
    void loadLanguage( MM_TTSAPI::Language lang );

// Dialog Data
	//{{AFX_DATA(CMultiSpeakTestDlg)
	enum { IDD = IDD_MULTISPEAKTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSpeakTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    
    MM_TTSAPI::EngineFactory    m_factory;
    
    typedef std::map<MM_TTSAPI::Language, LangData> LangMap;
    
    LangMap m_langMap;

	HICON m_hIcon;

    std::vector<SpeakTestDialog*>   m_dialogList;

	// Generated message map functions
	//{{AFX_MSG(CMultiSpeakTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAddHungarian();
	afx_msg void OnButtonAddEnglish();
	afx_msg void OnButtonAddMalay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISPEAKTESTDLG_H__76B138C9_8558_11D4_93F9_0040F6341449__INCLUDED_)
