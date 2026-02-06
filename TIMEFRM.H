//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_TimeFrm_H__38AA26D2_7D89_4989_904F_92B7CA8FCCA0__INCLUDED_)
#define AFX_TimeFrm_H__38AA26D2_7D89_4989_904F_92B7CA8FCCA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeFrm.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTimeFrm dialog

class CTimeFrm : public CDialog
{
// Construction
public:
    CTimeFrm(CWnd* pParent = NULL); // standard constructor
    CString strVehicle,msg;
    long confirmWeight;
    int  type,key;
    int  maxWt;
    void ShowChart();

// Dialog Data
    //{{AFX_DATA(CTimeFrm)
	enum { IDD = IDD_TIMEFRM };
	CMSChart	m_chart;
	CString	m_wrange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTimeFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CTimeFrm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TimeFrm_H__38AA26D2_7D89_4989_904F_92B7CA8FCCA0__INCLUDED_)
