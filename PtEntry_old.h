#if !defined(AFX_PTENTRY_H__E246263F_EB14_4FCD_9794_01933AEA6027__INCLUDED_)
#define AFX_PTENTRY_H__E246263F_EB14_4FCD_9794_01933AEA6027__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PtEntry.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CPtEntry dialog

class CPtEntry : public CStaff
{
// Construction
public:
	CPtEntry(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPtEntry)
	enum { IDD = IDD_PT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPtEntry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPtEntry)
	afx_msg void OnViewStockFlow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTENTRY_H__E246263F_EB14_4FCD_9794_01933AEA6027__INCLUDED_)
