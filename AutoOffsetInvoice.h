#if !defined(AFX_AUTOOFFSETINVOICE_H__710799C0_1020_48CB_8FFA_033DAB151998__INCLUDED_)
#define AFX_AUTOOFFSETINVOICE_H__710799C0_1020_48CB_8FFA_033DAB151998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoOffsetInvoice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoOffsetInvoice dialog

class CAutoOffsetInvoice : public CDialog
{
// Construction
public:
	CAutoOffsetInvoice(CWnd* pParent = NULL);   // standard constructor
    DB *df, *sub;

// Dialog Data
	//{{AFX_DATA(CAutoOffsetInvoice)
	enum { IDD = IDD_AUTO_OFFSET_INV };
	COleDateTime	m_d1;
	COleDateTime	m_d2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoOffsetInvoice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoOffsetInvoice)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnOffsetOneByOne();
	afx_msg void OnOffsetOneByOneTicket();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOOFFSETINVOICE_H__710799C0_1020_48CB_8FFA_033DAB151998__INCLUDED_)
