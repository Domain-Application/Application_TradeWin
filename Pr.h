#if !defined(AFX_PR_H__6F27A305_A6FA_4906_B6AC_90B75E206798__INCLUDED_)
#define AFX_PR_H__6F27A305_A6FA_4906_B6AC_90B75E206798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pr.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CPr dialog

class CPr : public CStaff
{
// Construction
public:
	CPr(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void ShowInformation();
	virtual void SetAttribute(int flag,int fld);

// Dialog Data
	//{{AFX_DATA(CPr)
	enum { IDD = IDD_PR_NEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPr)
	afx_msg void OnFirst();
	afx_msg void OnSecond();
	afx_msg void OnDestroy();
	afx_msg void OnInitMenu(CMenu* pMenu);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR_H__6F27A305_A6FA_4906_B6AC_90B75E206798__INCLUDED_)
