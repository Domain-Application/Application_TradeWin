#if !defined(AFX_SETPAYDATEU_H__3B1E0FD4_0088_43C7_92C9_2E45CFD7D633__INCLUDED_)
#define AFX_SETPAYDATEU_H__3B1E0FD4_0088_43C7_92C9_2E45CFD7D633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SETPAYDATEU.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATEU dialog

class CSETPAYDATEU : public CDialog
{
// Construction
public:
	CSETPAYDATEU(CWnd* pParent = NULL);   // standard constructor
	DB *updatepay,*main;
	BOOL ispaydate;

// Dialog Data
	//{{AFX_DATA(CSETPAYDATEU)
	enum { IDD = IDD_SETPAYDATEU };
	COleDateTime	m_b1;
	CString b_ticket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSETPAYDATEU)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSETPAYDATEU)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPAYDATEU_H__3B1E0FD4_0088_43C7_92C9_2E45CFD7D633__INCLUDED_)
