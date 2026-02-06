#if !defined(AFX_SCALE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_SCALE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scale.h : header file
//

#include "Parent.h"
/////////////////////////////////////////////////////////////////////////////
// CScale dialog

class CScale : public CParent
{
// Construction
public:
    CScale(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual BOOL IsCannotEdit();
    virtual void ShowDetail();
    virtual void OnDetail();
    virtual void ShowInformation();
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
	virtual void SetAttribute(int flag,int fld);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
	virtual void after_save(int ExistFlag);
	virtual void OnReqitemInitDialog(CDialog *p,int status);
    virtual void OnPrint();

    double amt, rounding_adj, final_amt, subtotal_amt, subtotal_rounding_adj, subtotal_final_amt;

// Dialog Data
    //{{AFX_DATA(CScale)
    enum { IDD = IDD_SCALE };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScale)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CScale)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetpaydate();
	afx_msg void OnUnset();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
