#if !defined(AFX_TWO_H__18E80BC1_942F_11D2_B77D_F58344E04534__INCLUDED_)
#define AFX_TWO_H__18E80BC1_942F_11D2_B77D_F58344E04534__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Two.h : header file
//
#include "Tio.h"
/////////////////////////////////////////////////////////////////////////////
// CTwo dialog

class CTwo : public CTio
{
// Construction
public:
        CTwo(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTwo)
	enum { IDD = IDD_TWO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual void show_tio();
        virtual void OnSetWild();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTwo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWO_H__18E80BC1_942F_11D2_B77D_F58344E04534__INCLUDED_)
