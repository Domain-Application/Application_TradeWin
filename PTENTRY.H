#if !defined(AFX_PTENTRY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_PTENTRY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ptentry.h : header file
//

#include "Parent.h"
/////////////////////////////////////////////////////////////////////////////
// CPtentry dialog

class CPtentry : public CParent
{
// Construction
public:
    CPtentry(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowDetail();
    virtual void OnDetail();
    virtual void ShowInformation();
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
	virtual void SetAttribute(int flag,int fld);

// Dialog Data
    //{{AFX_DATA(CPtentry)
	enum { IDD = IDD_PT };
	double	m_cadj;
	double	m_sadj;
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPtentry)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPtentry)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewStockFlow();
	afx_msg void OnProcess();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTENTRY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
