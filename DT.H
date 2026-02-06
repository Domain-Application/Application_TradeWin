//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_DT_H__8C27C1E0_0432_11D6_A4E7_0080C8EA65C0__INCLUDED_)
#define AFX_DT_H__8C27C1E0_0432_11D6_A4E7_0080C8EA65C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DT.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CDt dialog

class CDt : public CParent
{
// Construction
public:
    CDt(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void ShowInformation();
	virtual void ShowInitFrame();
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
	virtual BOOL IsCannotEdit();
	virtual void SetAttribute(int flag,int fld);
		
	CString strSql;

// Dialog Data
    //{{AFX_DATA(CDt)
    enum { IDD = IDD_DT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CDt)
	afx_msg void OnTransferToMain();
	afx_msg void OnButton2();
	afx_msg void OnUpdatePrice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DT_H__8C27C1E0_0432_11D6_A4E7_0080C8EA65C0__INCLUDED_)
