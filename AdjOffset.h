#if !defined(AFX_ADJOFFSET_H__B3DEB1C1_6115_11D3_8F94_BBAB4B66A563__INCLUDED_)
#define AFX_ADJOFFSET_H__B3DEB1C1_6115_11D3_8F94_BBAB4B66A563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjOffset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjOffset dialog

class CAdjOffset : public CDialog
{
// Construction
public:
	CAdjOffset(CWnd* pParent = NULL);   // standard constructor
    void ShowReportOption(DB *df);
	int fontPitch;
    double InvoiceRealBalance();

// Dialog Data
	//{{AFX_DATA(CAdjOffset)
	enum { IDD = IDD_ADJ_OFFSET };
	CListCtrl	m_ListView;
    CBitmapButton m_ok;
    CBitmapButton m_cancel;
	CString	m_invoiceno;
	CString	m_balamt;
	CString	m_offsetamt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjOffset)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJOFFSET_H__B3DEB1C1_6115_11D3_8F94_BBAB4B66A563__INCLUDED_)
