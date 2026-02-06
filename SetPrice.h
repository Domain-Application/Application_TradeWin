#if !defined(AFX_SETPRICE_H__68557CB2_BEE4_4402_946A_6C46B73F428E__INCLUDED_)
#define AFX_SETPRICE_H__68557CB2_BEE4_4402_946A_6C46B73F428E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetPrice.h : header file
//
#include "staff.h"

/////////////////////////////////////////////////////////////////////////////
// CSetPrice dialog

class CSetPrice : public CStaff
{
// Construction
public:
	CSetPrice(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void ShowInformation();
	virtual void after_save(int ExistFlag);
	virtual BOOL IsCannotEdit();

// Dialog Data
	//{{AFX_DATA(CSetPrice)
	enum { IDD = IDD_SETPRICE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetPrice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetPrice)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPRICE_H__68557CB2_BEE4_4402_946A_6C46B73F428E__INCLUDED_)
