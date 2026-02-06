#if !defined(AFX_DS_H__7CBF0341_1CF3_11D3_8243_C911F0784B41__INCLUDED_)
#define AFX_DS_H__7CBF0341_1CF3_11D3_8243_C911F0784B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ds.h : header file
//
#include "staff.h"


/////////////////////////////////////////////////////////////////////////////
// CDs dialog

class CDs : public CStaff
{
// Construction
public:
     CDs(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	 virtual BOOL IsCannotEdit();
     virtual void ShowInformation();
	 virtual void SetAttribute(int flag,int fld);
	 virtual void after_save(int ExistFlag);


// Dialog Data
	//{{AFX_DATA(CDs)
	enum { IDD = IDD_DS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDs)
	afx_msg void OnPartyGrading();
	afx_msg void OnSetpaydate();
	afx_msg void OnUnset();
	afx_msg void OnDo();
	afx_msg void OnSetarrdate();
	afx_msg void OnUnsetarr();
	afx_msg void OnDestroy();
	afx_msg void OnChangeUpc8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DS_H__7CBF0341_1CF3_11D3_8243_C911F0784B41__INCLUDED_)
