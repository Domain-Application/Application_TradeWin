//{{AFX_INCLUDES()
#include "mscomm.h"
#include "mdctext.h"
#include "gwedit.h"
//}}AFX_INCLUDES
#if !defined(AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_)
#define AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetWt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetWt dialog

class CGetWt : public CDialog
{
// Construction
public:
	CGetWt(CWnd* pParent = NULL);   // standard constructor
    long  weight;
    void GetIndicatorWeight(CString str,char *tmp,int i);
    int  GetIndicatorType(int k = 0);
    int  IndicatorType;
    virtual void OnCancel();
    virtual void OnOK();

// Dialog Data
	//{{AFX_DATA(CGetWt)
	enum { IDD = IDD_GW };
	CGwEdit	m_gwedit;
	CMSComm	m_comm;
	CMdcText	m_wt;
	CString	m_vehicle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetWt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetWt)
	virtual BOOL OnInitDialog();
	afx_msg void OnGw();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickCommandbutton1();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_)
