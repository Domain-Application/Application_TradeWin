#if !defined(AFX_OFFSETAMT_H__4D9E1242_59D8_11D4_A635_9CC002B29F67__INCLUDED_)
#define AFX_OFFSETAMT_H__4D9E1242_59D8_11D4_A635_9CC002B29F67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OffsetAmt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COffsetAmt dialog

class COffsetAmt : public CDialog
{
// Construction
public:
	COffsetAmt(CWnd* pParent = NULL);   // standard constructor
    DB *df;
    double oamt;
// Dialog Data
	//{{AFX_DATA(COffsetAmt)
	enum { IDD = IDD_OFFSETAMT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COffsetAmt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COffsetAmt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OFFSETAMT_H__4D9E1242_59D8_11D4_A635_9CC002B29F67__INCLUDED_)
