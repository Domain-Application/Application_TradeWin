#if !defined(AFX_TNG_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_TNG_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tng.h : header file
//

#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CTng dialog

class CTng : public CStaff
{
// Construction
public:
    CTng(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInformation();
	CString str;
    BOOL busy;
    CString m_tuid;

// Dialog Data
    //{{AFX_DATA(CTng)
    enum { IDD = IDD_TNG };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTng)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTng)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_TNG_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
