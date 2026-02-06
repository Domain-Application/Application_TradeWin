#if !defined(AFX_ENQBANK_H__135D01E1_640A_11D4_A636_A0A1DBF46565__INCLUDED_)
#define AFX_ENQBANK_H__135D01E1_640A_11D4_A636_A0A1DBF46565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnqBank.h : header file
//

#include "browse.h"
/////////////////////////////////////////////////////////////////////////////
// CEnqBank dialog

class CEnqBank : public CBrowse
{
// Construction
public:
    CEnqBank(UINT IDD = IDD_BROWSE,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInitFrame();
    virtual void ReplaceGrid(long rec,CMSFlexGrid *m_grid,DB *df,int IdxItem);
    void put_blist(DB *df,int mode);
    virtual void SetBrowseText();
    double put_bankp(DB *df);

// Dialog Data
	//{{AFX_DATA(CEnqBank)
	enum { IDD = IDD_ENQBANK };
	CProgressCtrl	m_progress;
	COleDateTime	m_d2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnqBank)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnqBank)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	afx_msg void OnPrint();
	afx_msg void OnClickMsflexgrid1();
        afx_msg void OnEditDate();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENQBANK_H__135D01E1_640A_11D4_A636_A0A1DBF46565__INCLUDED_)
