#if !defined(AFX_M_ANIMATION_H__ECC82F49_0E93_11D3_8243_DF2125695E5B__INCLUDED_)
#define AFX_M_ANIMATION_H__ECC82F49_0E93_11D3_8243_DF2125695E5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// M_ANIMATION.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// M_ANIMATION dialog

class M_ANIMATION : public CDialog
{
// Construction
public:
	M_ANIMATION(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(M_ANIMATION)
	enum { IDD = IDD_WB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(M_ANIMATION)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(M_ANIMATION)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(M_ANIMATION)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M_ANIMATION_H__ECC82F49_0E93_11D3_8243_DF2125695E5B__INCLUDED_)
