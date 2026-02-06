#if !defined(AFX_BLIST_H__B2E23532_787C_4C56_B434_EEE60BA0CA95__INCLUDED_)
#define AFX_BLIST_H__B2E23532_787C_4C56_B434_EEE60BA0CA95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Blist.h : header file
//

#include "browse.h"
/////////////////////////////////////////////////////////////////////////////
// CBlist dialog

class CBlist : public CBrowse
{
// Construction
public:
    CBlist(UINT IDD = IDD_BROWSE,CWnd* pParent = NULL);   // standard constructor
    virtual void SetBrowseText();

// Dialog Data
	//{{AFX_DATA(CBlist)
	enum { IDD = IDD_BLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlist)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlist)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLIST_H__B2E23532_787C_4C56_B434_EEE60BA0CA95__INCLUDED_)
