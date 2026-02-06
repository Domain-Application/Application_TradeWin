#if !defined(AFX_ONE_H__70B08D21_0BD3_11D3_B781_9C9638427963__INCLUDED_)
#define AFX_ONE_H__70B08D21_0BD3_11D3_B781_9C9638427963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// One.h : header file
//
#include "Tio.h"
/////////////////////////////////////////////////////////////////////////////
// COne dialog

class COne : public CTio
{
// Construction
public:
        COne(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COne)
	enum { IDD = IDD_ONE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COne)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual void show_tio();
        virtual void OnSetWild();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COne)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONE_H__70B08D21_0BD3_11D3_B781_9C9638427963__INCLUDED_)
