//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "labelcontrol.h"
#include "mdccheckbox.h"
//}}AFX_INCLUDES
#if !defined(AFX_IPAY_H__6D7C4EC1_62A7_11D3_8F94_8D39C9F3F155__INCLUDED_)
#define AFX_IPAY_H__6D7C4EC1_62A7_11D3_8F94_8D39C9F3F155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ipay.h : header file
//
#include "Parent.h"
/////////////////////////////////////////////////////////////////////////////
// CIpay dialog

class CIpay : public CParent
{
// Construction
public:
    CIpay(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    double PaydatTotal();
    void ShowOutstandingInvoice();
    virtual void ShowInitFrame();
    virtual void SetBalanceGridInitFrame(long rec);
    virtual void RefreshBalanceInvoiceGrid();
    virtual void SetAttribute(int flag,int fld);
    void ReplaceBalanceGrid(long rec);
    virtual void AddOffsetInvoice();
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void ShowDetail();
    virtual void ShowInformation();
    virtual void OnDetail();
    virtual void AutoCreateCreditNote();
    virtual void SetOneTimeScreen();
    int SavePayData(double *amt,DB *sub);
    void get_invoice_balance(DB *df,DB *data,int total_field, int OrderBy=1);
    int SaveGiData(double *amt,DB *data);
    BOOL bScheme;
    CWinThread *pPartNoThread;

// Dialog Data
	//{{AFX_DATA(CIpay)
	enum { IDD = IDD_IPAY };
	CProgressCtrl	m_progress;
	CButton	m_show;
	CString	m_customer;
	CTime	m_return;
	CString	m_salesman;
	CMSFlexGrid	m_bgrid;
	CLabelControl	m_total;
	CMdcCheckBox	m_bal_inv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIpay)
	virtual BOOL OnInitDialog();
	afx_msg void OnGotoNext();
	afx_msg void OnClickBalanceInvoice();
	afx_msg void OnChangeCheckbox1();
	afx_msg void OnChequeReturnRegister();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPAY_H__6D7C4EC1_62A7_11D3_8F94_8D39C9F3F155__INCLUDED_)
