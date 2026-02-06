#if !defined(AFX_SETPAYMENTDATE_H__581C1648_8963_4BD1_8A95_4CAB43C1448A__INCLUDED_)
#define AFX_SETPAYMENTDATE_H__581C1648_8963_4BD1_8A95_4CAB43C1448A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetPaymentDate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetPaymentDate dialog

class CSetPaymentDate : public CDialog
{
// Construction
public:
	CSetPaymentDate(CWnd* pParent = NULL);   // standard constructor
	DB *df;

// Dialog Data
	//{{AFX_DATA(CSetPaymentDate)
	enum { IDD = IDD_SETPAYMENTDATE };
	COleDateTime	m_d1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetPaymentDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetPaymentDate)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPAYMENTDATE_H__581C1648_8963_4BD1_8A95_4CAB43C1448A__INCLUDED_)
