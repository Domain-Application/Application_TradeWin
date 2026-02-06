#if !defined(AFX_EDITTOOLBAR_H__7B829781_7CD5_11D2_88DE_C7AA706D4542__INCLUDED_)
#define AFX_EDITTOOLBAR_H__7B829781_7CD5_11D2_88DE_C7AA706D4542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditToolBar window

class CEditToolBar : public CToolBarCtrl
{
// Construction
public:
	CEditToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditToolBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTOOLBAR_H__7B829781_7CD5_11D2_88DE_C7AA706D4542__INCLUDED_)
