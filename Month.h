#if !defined(AFX_MONTH_H__185A15A1_0C6A_11D3_8243_8670CE518258__INCLUDED_)
#define AFX_MONTH_H__185A15A1_0C6A_11D3_8243_8670CE518258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Month.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CMonth dialog

class CMonth : public CTio
{
// Construction
public:
        CMonth(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonth)
	enum { IDD = IDD_MONTH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonth)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTH_H__185A15A1_0C6A_11D3_8243_8670CE518258__INCLUDED_)
