#if !defined(AFX_WACITEM_H__874A14D9_772C_426D_B4CA_1AAC2CCE93B2__INCLUDED_)
#define AFX_WACITEM_H__874A14D9_772C_426D_B4CA_1AAC2CCE93B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wacitem.h : header file
//
#include "child.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWacitem dialog

class CWacitem : public CChild
{
// Construction
public:
    CWacitem(UINT IDD,DB *psub,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInitFrame();
    virtual void ReplaceChildGrid(long rec);
    virtual void Refresh();
    virtual void ShowChildTotal();
    virtual BOOL IsBatchSaveConditionOK();

// Dialog Data
	//{{AFX_DATA(CWacitem)
	enum { IDD = IDD_WACITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWacitem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWacitem)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WACITEM_H__874A14D9_772C_426D_B4CA_1AAC2CCE93B2__INCLUDED_)
