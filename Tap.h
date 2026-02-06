#if !defined(AFX_TAP_H__E21A8FEE_37B1_4573_A45D_42D4A64A3382__INCLUDED_)
#define AFX_TAP_H__E21A8FEE_37B1_4573_A45D_42D4A64A3382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tap.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CTap dialog

class CTap : public CTio
{
// Construction
public:
    CTap(UINT IDD,DB *pdf,int Idx = 0,void(*p)() = NULL,CWnd* pParent = NULL);   // standard constructor
    virtual void OnSetWild();
    virtual void OnPrint();
    void OnSaveOR();
    double balance;

// Dialog Data
	//{{AFX_DATA(CTap)
	enum { IDD = IDD_TAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTap)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAP_H__E21A8FEE_37B1_4573_A45D_42D4A64A3382__INCLUDED_)
