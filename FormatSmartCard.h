#if !defined(AFX_FORMATSMARTCARD_H__F30CEA7B_D6F7_438A_8A63_9A9160D25D2C__INCLUDED_)
#define AFX_FORMATSMARTCARD_H__F30CEA7B_D6F7_438A_8A63_9A9160D25D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatSmartCard.h : header file
//

#include "ClockST.h"
/////////////////////////////////////////////////////////////////////////////
// CFormatSmartCard dialog

class CFormatSmartCard : public CDialog
{
// Construction
public:
    CFormatSmartCard(CWnd* pParent = NULL);   // standard constructor
    BOOL DoFormat();
    BOOL DoSector(int sector);
    BOOL busy;
    int  key;
    CString m_tuid,str;

// Dialog Data
	//{{AFX_DATA(CFormatSmartCard)
	enum { IDD = IDD_FORMATCARD };
	CProgressCtrl	m_progress;
    CClockST m_Clock;
	CString	m_msg;
	CString	m_cardno;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormatSmartCard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormatSmartCard)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATSMARTCARD_H__F30CEA7B_D6F7_438A_8A63_9A9160D25D2C__INCLUDED_)
