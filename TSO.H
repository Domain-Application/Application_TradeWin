//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TSO_H__6BD6A161_5ACD_11D3_8F94_A6F7D54E9A59__INCLUDED_)
#define AFX_TSO_H__6BD6A161_5ACD_11D3_8F94_A6F7D54E9A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tso.h : header file
//
#include "Tio.h"
#include "db_ado.h"
#include "state.h"
/////////////////////////////////////////////////////////////////////////////
// CTso dialog

class CTso : public CTio
{
// Construction
public:
    CTso(UINT IDD,DB *pdf,int Idx = 0,void(*p)() = NULL,CWnd* pParent = NULL);   // standard constructor
    ~CTso();
    virtual void SetOneTimeScreen();
    virtual void OnSetWild();
    virtual void show_tio();
    virtual void ShowInformation();
    virtual void OnPrintText();
    virtual void OnPrint();
    virtual void ShowBeginScreen();
    virtual void OnCancel();
    void SaveGridRecord();
    void ReplaceGridRecord();
    void    (*gst)();
    char file[140];
    BOOL bDone;
    void FindBF();
    void FindOutstanding();
    void FindPeriodData();
    void FindAgeing();
    void DoStatement();
    DB adoTmp;
    CString str,strSql;
    double Totalbal,bal;

// Dialog Data
	//{{AFX_DATA(CTso)
	enum { IDD = IDD_TSO };
	CProgressCtrl	m_progress3;
	CProgressCtrl	m_progress2;
	CProgressCtrl	m_progress;
	CMSFlexGrid	m_grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTso)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTso)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSO_H__6BD6A161_5ACD_11D3_8F94_A6F7D54E9A59__INCLUDED_)
