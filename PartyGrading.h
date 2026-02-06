#if !defined(AFX_PARTYGRADING_H__FEAEC4A5_9D76_4D79_98B5_AB9D99FB2F90__INCLUDED_)
#define AFX_PARTYGRADING_H__FEAEC4A5_9D76_4D79_98B5_AB9D99FB2F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartyGrading.h : header file
//

#include "FEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CPartyGrading dialog

class CPartyGrading : public CFEdit
{
// Construction
public:
    CPartyGrading(UINT IDD,CWnd* pParent = NULL);   // standard constructor
    virtual int GetNextField(int fld);
    virtual void ShowInformation();

// Dialog Data
	//{{AFX_DATA(CPartyGrading)
	enum { IDD = IDD_PARTY_GRADING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartyGrading)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPartyGrading)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTYGRADING_H__FEAEC4A5_9D76_4D79_98B5_AB9D99FB2F90__INCLUDED_)
