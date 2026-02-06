#if !defined(AFX_DP_H__4E911362_18C9_11D3_8243_B50274934C4B__INCLUDED_)
#define AFX_DP_H__4E911362_18C9_11D3_8243_B50274934C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dp.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CDp dialog

class CDp : public CStaff
{
// Construction
public:
        CDp(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
		virtual void SetAttribute(int flag,int fld);
		virtual void ShowInformation();

// Dialog Data
	//{{AFX_DATA(CDp)
	enum { IDD = IDD_DP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDp)
	afx_msg void OnPartyGrading();
	afx_msg void OnDo();
	afx_msg void OnSetpaydate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DP_H__4E911362_18C9_11D3_8243_B50274934C4B__INCLUDED_)
