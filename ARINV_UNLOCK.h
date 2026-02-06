#if !defined(AFX_ARINV_UNLOCK_H__4853C79B_325C_4FA6_8CCA_87295AADD602__INCLUDED_)
#define AFX_ARINV_UNLOCK_H__4853C79B_325C_4FA6_8CCA_87295AADD602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ARINV_UNLOCK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CARINV_UNLOCK dialog

class CARINV_UNLOCK : public CDialog
{
// Construction
public:
	CARINV_UNLOCK(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CARINV_UNLOCK)
	enum { IDD = IDD_UnlockP };
	COleDateTime	m_d1;
	COleDateTime	m_d2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CARINV_UNLOCK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CARINV_UNLOCK)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARINV_UNLOCK_H__4853C79B_325C_4FA6_8CCA_87295AADD602__INCLUDED_)
