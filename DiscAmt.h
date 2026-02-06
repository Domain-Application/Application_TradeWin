#if !defined(AFX_DISCAMT_H__407A1E73_A048_4935_8E2F_A9198775C423__INCLUDED_)
#define AFX_DISCAMT_H__407A1E73_A048_4935_8E2F_A9198775C423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiscAmt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiscAmt dialog

class CDiscAmt : public CDialog
{
// Construction
public:
	CDiscAmt(CWnd* pParent = NULL);   // standard constructor
    DB *df;
    double oamt;
// Dialog Data
	//{{AFX_DATA(CDiscAmt)
	enum { IDD = IDD_DISC_AMT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiscAmt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiscAmt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISCAMT_H__407A1E73_A048_4935_8E2F_A9198775C423__INCLUDED_)
