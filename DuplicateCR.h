#if !defined(AFX_DUPLICATECR_H__88312695_D63A_4701_99A9_4E5893C20770__INCLUDED_)
#define AFX_DUPLICATECR_H__88312695_D63A_4701_99A9_4E5893C20770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DuplicateCR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDuplicateCR dialog

class CDuplicateCR : public CDialog
{
// Construction
public:
	CDuplicateCR(CWnd* pParent = NULL);   // standard constructor
    void OnDuplicate();
    void OnDuplicateCopy();
    char szNo[100];
    CString str,strSql;

// Dialog Data
	//{{AFX_DATA(CDuplicateCR)
	enum { IDD = IDD_DUPLICATE_CR };
	CString	m_crno;
	CString	m_nextno;
	CString	m_noofcopy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuplicateCR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDuplicateCR)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUPLICATECR_H__88312695_D63A_4701_99A9_4E5893C20770__INCLUDED_)
