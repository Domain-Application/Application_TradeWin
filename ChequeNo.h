#if !defined(AFX_CHEQUENO_H__05626996_1A65_4104_8524_E8AFE2D7C370__INCLUDED_)
#define AFX_CHEQUENO_H__05626996_1A65_4104_8524_E8AFE2D7C370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChequeNo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChequeNo dialog

class CChequeNo : public CDialog
{
// Construction
public:
	CChequeNo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChequeNo)
	enum { IDD = IDD_CHEQUE_NO };
	COleDateTime	m_date;
	CString	m_cheque;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChequeNo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChequeNo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHEQUENO_H__05626996_1A65_4104_8524_E8AFE2D7C370__INCLUDED_)
