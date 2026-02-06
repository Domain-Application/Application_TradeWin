#if !defined(AFX_WDS_H__0AFBCC81_1DB0_11D3_8243_AE3DAA2AC047__INCLUDED_)
#define AFX_WDS_H__0AFBCC81_1DB0_11D3_8243_AE3DAA2AC047__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wds.h : header file
//

#include "weighing.h"
/////////////////////////////////////////////////////////////////////////////
// CWds dialog

class CWds : public CWeighing
{
// Construction
public:
        CWds(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
        virtual void W1Start(long weight);
        virtual void W2Start(long weight);
        virtual void ShowInformation();
        virtual void W2Saved();

// Dialog Data
	//{{AFX_DATA(CWds)
	enum { IDD = IDD_DS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWds)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWds)
	afx_msg void OnPartyGrading();
	afx_msg void OnSetpaydate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDS_H__0AFBCC81_1DB0_11D3_8243_AE3DAA2AC047__INCLUDED_)
