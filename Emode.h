#if !defined(AFX_EMODE_H__E2B0473D_EC4C_4EB1_8BC0_8343BDEE7621__INCLUDED_)
#define AFX_EMODE_H__E2B0473D_EC4C_4EB1_8BC0_8343BDEE7621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Emode.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CEmode dialog

class CEmode : public CStaff
{
// Construction
public:
    CEmode(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInformation();
    virtual void ReplaceRecord();

// Dialog Data
	//{{AFX_DATA(CEmode)
	enum { IDD = IDD_EMODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmode)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
    afx_msg void OnRadio6();
	afx_msg void OnGotoNextRunningNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMODE_H__E2B0473D_EC4C_4EB1_8BC0_8343BDEE7621__INCLUDED_)
