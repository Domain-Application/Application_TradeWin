#if !defined(AFX_CHEQUERETURNREGISTER_H__51E18A80_6D68_4857_8D07_3C51F9448FA3__INCLUDED_)
#define AFX_CHEQUERETURNREGISTER_H__51E18A80_6D68_4857_8D07_3C51F9448FA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChequeReturnRegister.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChequeReturnRegister dialog

class CChequeReturnRegister : public CDialog
{
// Construction
public:
	CChequeReturnRegister(CWnd* pParent = NULL);   // standard constructor
    BOOL IsDataOK();
    void MoveToReturnCheque();
    DB *df,*sub;

// Dialog Data
	//{{AFX_DATA(CChequeReturnRegister)
	enum { IDD = IDD_RETURN_CHQ };
	CComboBox	m_cb;
	CString	m_cheque;
	CString	m_or;
	CString	m_accode;
	COleDateTime	m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChequeReturnRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChequeReturnRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHEQUERETURNREGISTER_H__51E18A80_6D68_4857_8D07_3C51F9448FA3__INCLUDED_)
