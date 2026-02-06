//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "labelcontrol.h"
#include "mdccheckbox.h"
//}}AFX_INCLUDES
#if !defined(AFX_CR_H__9274F1E4_5A1F_11D3_8F94_92541F98A35F__INCLUDED_)
#define AFX_CR_H__9274F1E4_5A1F_11D3_8F94_92541F98A35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cr.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CCr dialog

class CCr : public CParent
{
// Construction
public:
    CCr(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    void ShowOutstandingInvoice();
    virtual void SetOneTimeScreen();
    void RefreshBalanceInvoiceGrid();
    void SetBalanceGridInitFrame(long rec);
    virtual int SavePayData(double *amt,DB *sub);
    void ShowGldataInitFrame();
    void ReplaceGldataSubGrid(long rec);
    void ReplaceBalanceGrid(long rec);
    virtual BOOL IsChildDataCompleted(DB *sub);
    virtual void ShowInitFrame();
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void ShowDetail();
    virtual void ShowInformation();
    virtual void SetAttribute(int flag,int fld);
    void get_invoice_balance(DB *df,DB *data,int total_field);
    virtual void OnDetail();
//    void GetInvoiceInformation(DB* df);
    int SaveGiData(double *amt,DB *data);
    double TotalGlAmount;
    CWinThread *CrThread;

// Dialog Data
	//{{AFX_DATA(CCr)
	enum { IDD = IDD_CR };
	CProgressCtrl	m_progress;
	CString	m_customer;
	CMSFlexGrid	m_glgrid;
	CString	m_salesman;
	CMSFlexGrid	m_bgrid;
    CMdcCheckBox m_bal_inv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCr)
	virtual BOOL OnInitDialog();
	afx_msg void OnShow();
	afx_msg void OnDblClickBgrid();
	afx_msg void OnChangeCheckbox1();
	afx_msg void OnDuplicate();
	afx_msg void OnButton2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CR_H__9274F1E4_5A1F_11D3_8F94_92541F98A35F__INCLUDED_)
