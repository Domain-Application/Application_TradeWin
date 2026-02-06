#if !defined(AFX_VIEWINV_H__81F9F0E2_7CA7_11D3_8F94_D9844E353745__INCLUDED_)
#define AFX_VIEWINV_H__81F9F0E2_7CA7_11D3_8F94_D9844E353745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewInv.h : header file
//
#include "browse.h"
/////////////////////////////////////////////////////////////////////////////
// CViewInv dialog

class CViewInv : public CBrowse
{
// Construction
public:
    CViewInv(UINT IDD = IDD_BROWSE,CWnd* pParent = NULL);   // standard constructor
    virtual void SetBrowseText();
    void ComputeOverFourMonthAgeing();

// Dialog Data
    //{{AFX_DATA(CViewInv)
    enum { IDD = IDD_VIEWINV };
	CString	m_m4;
	CString	m_due;
	CString	m_m1;
	CString	m_m2;
	CString	m_m3;
	CString	m_current;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewInv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CViewInv)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnViewDetailTransaction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWINV_H__81F9F0E2_7CA7_11D3_8F94_D9844E353745__INCLUDED_)
