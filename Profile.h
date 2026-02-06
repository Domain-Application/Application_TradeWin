#if !defined(AFX_PROFILE1_H__439152A1_F2B7_11D2_99C1_F85CA8F9917A__INCLUDED_)
#define AFX_PROFILE1_H__439152A1_F2B7_11D2_99C1_F85CA8F9917A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Profile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProfile dialog

class CProfile : public CDialog
{
// Construction
public:
	CProfile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProfile)
	enum { IDD = IDD_PROFILE };
	CString	m_e1;
	CString	m_e2;
	CString	m_e3;
	CString	m_e4;
	CString	m_e5;
	CString	m_e6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILE1_H__439152A1_F2B7_11D2_99C1_F85CA8F9917A__INCLUDED_)
