//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_Arinv_H__F8F01B42_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_)
#define AFX_Arinv_H__F8F01B42_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Arinv.h : header file
//
#include "parent.h"

extern void AutoPayment(double bal_total);
/////////////////////////////////////////////////////////////////////////////
// CArinv dialog

class CArinv : public CParent
{
// Construction
public:
    CArinv(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual int  FindChildTotalFields();
    virtual void SetOneTimeScreen();
    void ShowAdjustmentDetail();
    void ShowPaymentDetail();
    virtual int IsSaveConditionOk(DB *df);
    void ShowCashbillBox(int nCmdShow);
    void ShowPaymentGrids(int nCmdShow);
    virtual BOOL IsChildDataCompleted(DB *sub);
    virtual void ShowInformation();
    virtual void ShowDetail();
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *df);
    virtual void OnDetail();
    virtual void ShowAdjGrid();
    virtual void ShowPayGrid();
    virtual void ShowRCGrid();
    virtual void ReplaceAdjGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *data,int IdxItem);
    virtual void ReplacePayGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *data,int IdxItem);
    virtual void ReplaceRCGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *data,int IdxItem);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void OnSetFocus(int fld);
    virtual void OnKillFocus(int fld);
    void ShowRCDetail();
    double pay_total,adj_total,RC_total,bal_total;
    long Srec,irec;
    BOOL   ShowFrameFlag;
    char str[40];

// Dialog Data
    //{{AFX_DATA(CArinv)
	enum { IDD = IDD_ARINV };
	CString	m_customer;
	CString	m_salesman;
	CMSFlexGrid	m_agrid;
    CMSFlexGrid m_pgrid;
	CString	m_invno;
	CLabelControl	m_balance;
	CMSFlexGrid	m_cgrid;
	CMSFlexGrid	m_rgrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CArinv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CArinv)
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Arinv_H__F8F01B42_5C84_11D3_8F94_C4D5F0EAD457__INCLUDED_)
