#if !defined(AFX_SCAN_H__D3C864C3_9F0B_4B26_800F_81DB8AD710AC__INCLUDED_)
#define AFX_SCAN_H__D3C864C3_9F0B_4B26_800F_81DB8AD710AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scan.h : header file

/////////////////////////////////////////////////////////////////////////////
// CScan dialog

class CScan : public CDialog
{
// Construction
public:
	CScan(CWnd* pParent = NULL);   // standard constructor
	CString str;
    BOOL busy;
    CString m_tuid;
	DB *df;

// Dialog Data
	//{{AFX_DATA(CScan)
	enum { IDD = IDD_SCAN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScan)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCAN_H__D3C864C3_9F0B_4B26_800F_81DB8AD710AC__INCLUDED_)
