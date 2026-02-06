#if !defined(AFX_WELCOME_H__AD61C97C_688E_4817_8572_A6369301664F__INCLUDED_)
#define AFX_WELCOME_H__AD61C97C_688E_4817_8572_A6369301664F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Welcome.h : header file
//

#include "ClockST.h"
/////////////////////////////////////////////////////////////////////////////
// CWelcome dialog

class CWelcome : public CDialog
{
// Construction
public:
	CWelcome(CWnd* pParent = NULL);   // standard constructor
    void SaveFirstWeight();
    void ShowFundNoEnough();
    void OnWaitingFire();
    BOOL WaitingFire,busy;
    double amt;
    CString m_tuid;
    CString str;
    void OnOK();
    long current_weight,iCounter,ExistRecordNo;
    DB *df,*wb;

// Dialog Data
	//{{AFX_DATA(CWelcome)
	enum { IDD = IDD_WELCOME };
    CClockST m_Clock;
	CString	m_pt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcome)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWelcome)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOME_H__AD61C97C_688E_4817_8572_A6369301664F__INCLUDED_)
