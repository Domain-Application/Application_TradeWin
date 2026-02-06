#if !defined(AFX_WDES_H__C81ABDA1_9500_11D2_B1F7_9895AAC24F0A__INCLUDED_)
#define AFX_WDES_H__C81ABDA1_9500_11D2_B1F7_9895AAC24F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// wdes.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// Cwdes dialog

class Cwdes : public CStaff
{
// Construction
public:
    Cwdes(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void OnSave();
    virtual void ShowInformation();
    virtual void OnAddedit();
    virtual int GetPreviousField(int fld);
    virtual int GetNextField(int fld);
    virtual void SetFieldEnable(int *fld);
    int     *m_array;
    DB      *m_mfile;

// Dialog Data
	//{{AFX_DATA(Cwdes)
	enum { IDD = IDD_CUSTOMER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cwdes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Cwdes)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDES_H__C81ABDA1_9500_11D2_B1F7_9895AAC24F0A__INCLUDED_)
