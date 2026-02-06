#if !defined(AFX_DPDO_H__A4186FFC_7AD7_474D_A2F0_99378C78574A__INCLUDED_)
#define AFX_DPDO_H__A4186FFC_7AD7_474D_A2F0_99378C78574A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DpDo.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CDpDo dialog

class CDpDo : public CStaff
{
// Construction
public:
	CDpDo(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void ShowInformation();
	CString dofrdp,ticketfrdp;

// Dialog Data
	//{{AFX_DATA(CDpDo)
	enum { IDD = IDD_DP_DO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDpDo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDpDo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPDO_H__A4186FFC_7AD7_474D_A2F0_99378C78574A__INCLUDED_)
