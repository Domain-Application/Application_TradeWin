#if !defined(AFX_EDITCOMBO_H__112AE161_812D_11D2_B1F7_D89D769CCD1F__INCLUDED_)
#define AFX_EDITCOMBO_H__112AE161_812D_11D2_B1F7_D89D769CCD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditCombo window

class CEditCombo : public CComboBox
{
// Construction
public:
	CEditCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditCombo)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOMBO_H__112AE161_812D_11D2_B1F7_D89D769CCD1F__INCLUDED_)
