#if !defined(AFX_WEIGHING_H__9CC10001_11C5_11D3_B36C_85716FC7922E__INCLUDED_)
#define AFX_WEIGHING_H__9CC10001_11C5_11D3_B36C_85716FC7922E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Weighing.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CWeighing dialog

class CWeighing : public CStaff
{
// Construction
public:
    CWeighing(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    ~CWeighing();
    virtual int GetRecord();
    virtual void OnDirectSave();
    virtual int OnTellMe(UINT nChar,int &id,int &fld);
    virtual int GetPreviousField(int fld);
    virtual int GetNextField(int fld);
    virtual void SetFieldEnable(int *fld);
    virtual void OnSave();
    virtual void OnAddedit();
    virtual void SaveSecondWeightTicket(DB *fdes,DB *mfile);
    virtual void OnPrintTicket();
    virtual void W1Start(long weight);
    virtual void W2Start(long weight);
    virtual BOOL ChangeToEditMode(int fld);
    virtual void W1Saved();
    virtual void W2Saved();
    virtual void OnDeleteFirstWeightRecord();
    virtual void OnNew();
    virtual BOOL IsLastField(int fld);
	virtual void CheckMaxWeight();
    int     *w1_array,*w2_array,*m_array,vehicle_fld;
    DB      *m_mfile;                   //main ticket file

// Dialog Data
	//{{AFX_DATA(CWeighing)
	enum { IDD = IDD_ASK_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeighing)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWeighing)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEIGHING_H__9CC10001_11C5_11D3_B36C_85716FC7922E__INCLUDED_)
