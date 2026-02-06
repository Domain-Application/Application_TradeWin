#if !defined(AFX_WDP_H__90601D41_2409_11D3_8243_97827C3DE441__INCLUDED_)
#define AFX_WDP_H__90601D41_2409_11D3_8243_97827C3DE441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wdp.h : header file
//
#include "weighing.h"

/////////////////////////////////////////////////////////////////////////////
// CWdp dialog

class CWdp : public CWeighing
{
// Construction
public:
        CWdp(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
        virtual void W1Start(long weight);
        virtual void W2Start(long weight);
        virtual void ShowInformation();
		virtual void W2Saved();

// Dialog Data
	//{{AFX_DATA(CWdp)
       enum { IDD = IDD_DP };
//	enum { IDD = IDD_ASK_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWdp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWdp)
	afx_msg void OnPartyGrading();
	afx_msg void OnSetpaydate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDP_H__90601D41_2409_11D3_8243_97827C3DE441__INCLUDED_)
