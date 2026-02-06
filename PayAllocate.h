//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PAYALLOCATE_H__4D9E1241_59D8_11D4_A635_9CC002B29F67__INCLUDED_)
#define AFX_PAYALLOCATE_H__4D9E1241_59D8_11D4_A635_9CC002B29F67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PayAllocate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPayAllocate dialog

class CPayAllocate : public CDialog
{
// Construction
public:
    CPayAllocate(UINT IDD,CWnd* pParent = NULL);   // standard constructor
    DB *df;
    void Refresh();
    void OnInitialPaymentScheme();
    void SetGridTitle();
    void ReplaceGrid(long rec);
    double BalanceAllocated,obal;
    void OnAutoAdjust(long rec,long last);
    BOOL bClearFlag,bScheme;
    char customer_code[20],paydate[10];
    double FindPaymentSchemeDiscount();
    void OnCompute(long rec,long last);

// Dialog Data
	//{{AFX_DATA(CPayAllocate)
	enum { IDD = IDD_PAY_ALLOCATE };
	CProgressCtrl	m_progress;
	CListBox	m_list;
	CComboBox	m_combo;
	CMSFlexGrid	m_grid;
	CString	m_BalanceAllocated;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPayAllocate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPayAllocate)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnAutoMatch();
	afx_msg void OnDblclkList1();
	afx_msg void OnActivePaymentScheme();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAYALLOCATE_H__4D9E1241_59D8_11D4_A635_9CC002B29F67__INCLUDED_)
