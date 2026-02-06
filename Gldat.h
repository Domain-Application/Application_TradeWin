//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_GLDAT_H__8310D881_5B69_11D4_A635_B30F8A7DB967__INCLUDED_)
#define AFX_GLDAT_H__8310D881_5B69_11D4_A635_B30F8A7DB967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Gldat.h : header file
//

#include "Child.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CGldat dialog

class CGldat : public CChild
{
// Construction
public:
    CGldat(UINT IDD,DB *psub,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInitFrame();
    virtual void ReplaceChildGrid(long rec);
    virtual void ShowChildTotal();
    virtual void SetOneTimeScreen();

// Dialog Data
	//{{AFX_DATA(CGldat)
	enum { IDD = IDD_GLDAT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGldat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGldat)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLDAT_H__8310D881_5B69_11D4_A635_B30F8A7DB967__INCLUDED_)
