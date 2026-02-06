#if !defined(AFX_WAC_H__F8F01B43_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_)
#define AFX_WAC_H__F8F01B43_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wac.h : header file
//
#include "kohfrm.h"
#include "wacitem.h"
/////////////////////////////////////////////////////////////////////////////
// CWac dialog

class CWac : public CKohFrm
{
// Construction
public:
    CWac(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInformation();
    virtual BOOL IsChildDataCompleted(DB *sub);
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void ChildControl(UINT IDD,char *key,DB *psub,int IdxItem);
    virtual void AutoAddInFastMode(DB *wsub);
    virtual void SetOneTimeScreen();
    virtual int FindChildTotalFields();
    virtual void OnDetail();

// Dialog Data
	//{{AFX_DATA(CWac)
	enum { IDD = IDD_WAC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWac)
	afx_msg void OnGotoNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAC_H__F8F01B43_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_)
