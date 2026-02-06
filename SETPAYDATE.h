#if !defined(AFX_SETPAYDATE_H__FCDF2BF9_DF89_4CF0_8A04_E11BB248E3A6__INCLUDED_)
#define AFX_SETPAYDATE_H__FCDF2BF9_DF89_4CF0_8A04_E11BB248E3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SETPAYDATE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATE dialog

class CSETPAYDATE : public CDialog
{
// Construction
public:
	CSETPAYDATE(CWnd* pParent = NULL);   // standard constructor
	DB *getpay;

// Dialog Data
	//{{AFX_DATA(CSETPAYDATE)
	enum { IDD = IDD_SETPAYDATE };
	COleDateTime	m_d1;
	CString m_ticket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSETPAYDATE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSETPAYDATE)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPAYDATE_H__FCDF2BF9_DF89_4CF0_8A04_E11BB248E3A6__INCLUDED_)
