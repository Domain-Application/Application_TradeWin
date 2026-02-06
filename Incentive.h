#if !defined(AFX_INCENTIVE_H__0BF2E6EE_3D54_4885_845F_8527F2D2DA7B__INCLUDED_)
#define AFX_INCENTIVE_H__0BF2E6EE_3D54_4885_845F_8527F2D2DA7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Incentive.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIncentive dialog

class CIncentive : public CDialog
{
// Construction
public:
	CIncentive(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIncentive)
	enum { IDD = IDD_INCENTIVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIncentive)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIncentive)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCENTIVE_H__0BF2E6EE_3D54_4885_845F_8527F2D2DA7B__INCLUDED_)
