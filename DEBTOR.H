#if !defined(AFX_DEBTOR_H__2DCE8722_444C_11D4_9E54_00606767E971__INCLUDED_)
#define AFX_DEBTOR_H__2DCE8722_444C_11D4_9E54_00606767E971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Debtor.h : header file
//
#include "Staff.h"

/////////////////////////////////////////////////////////////////////////////
// CDebtor dialog

class CDebtor : public CStaff
{
// Construction
public:
    CDebtor(UINT IDD,DB *pdf,int Idx,CWnd* pParent =NULL);   // standard constructor
    virtual void SetOneTimeScreen();
    void to_st2(DB *rep, DB *df);
    void to_st3(DB *rep, DB *df);
    double BalAmt;

// Dialog Data
	//{{AFX_DATA(CDebtor)
	enum { IDD = IDD_DEBTOR };
	COleDateTime	m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebtor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebtor)
	afx_msg void OnCystomerType();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnStatement();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBTOR_H__2DCE8722_444C_11D4_9E54_00606767E971__INCLUDED_)
