#if !defined(AFX_WPR_H__14269B03_16B9_4B8B_8AD5_E867DC9340D9__INCLUDED_)
#define AFX_WPR_H__14269B03_16B9_4B8B_8AD5_E867DC9340D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wpr.h : header file
//
#include "weighing.h"
/////////////////////////////////////////////////////////////////////////////
// CWpr dialog

class CWpr : public CWeighing
{
// Construction
public:
	CWpr(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void SaveSecondWeightTicket(DB *fdes,DB *mfile);
	virtual void W1Start(long weight);
	virtual void W2Start(long weight);
	virtual void ShowInformation();
	virtual void W2Saved();
	virtual void OnSave();

// Dialog Data
	//{{AFX_DATA(CWpr)
	enum { IDD = IDD_ACTYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWpr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWpr)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WPR_H__14269B03_16B9_4B8B_8AD5_E867DC9340D9__INCLUDED_)
