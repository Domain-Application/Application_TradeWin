//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_CIQ_H__DC214C21_2FDB_11D4_BCB5_A03BBA1C8544__INCLUDED_)
#define AFX_CIQ_H__DC214C21_2FDB_11D4_BCB5_A03BBA1C8544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ciq.h : header file
//
#include "browse.h"
/////////////////////////////////////////////////////////////////////////////
// CCiq dialog

class CCiq : public CBrowse
{
// Construction
public:
    CCiq(UINT IDD = IDD_BROWSE,CWnd* pParent = NULL);   // standard constructor
    virtual void CreateToolBarAndStatusBar();
    virtual void ShowInitFrame();
    virtual void ReplaceGrid(long rec,CMSFlexGrid *m_grid,DB *df,int IdxItem);
    void ViewCsutomerDetail(long rec,int mode);
    long select_records;
    void FindCustomerBalance();
    void OnViewDetails();
    CString str;

// Dialog Data
	//{{AFX_DATA(CCiq)
	enum { IDD = IDD_CIQ };
	CListBox	m_salesman;
	CProgressCtrl	m_progress2;
	CProgressCtrl	m_progress3;
    COleDateTime  m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCiq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCiq)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
    virtual void OnDblClickMsflexgrid1();
    virtual void OnPrinter();
    virtual void OnClickMsflexgrid1();
	afx_msg void OnStatement();
	afx_msg void OnGO();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIQ_H__DC214C21_2FDB_11D4_BCB5_A03BBA1C8544__INCLUDED_)
