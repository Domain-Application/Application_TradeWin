#if !defined(AFX_REMINDER_H__5C8B3F68_953D_494E_A28D_B8C8A070744B__INCLUDED_)
#define AFX_REMINDER_H__5C8B3F68_953D_494E_A28D_B8C8A070744B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Reminder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReminder dialog

class CReminder : public CDialog
{
// Construction
public:
	CReminder(CWnd* pParent = NULL);   // standard constructor
    void SaveFastMode();
    void UpdateFastMode();
    void UpdateWac();
    int WacModule(DB *df,int id);
    long wac_mrec;
    DB *df;

// Dialog Data
	//{{AFX_DATA(CReminder)
	enum { IDD = IDD_REMINDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReminder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReminder)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMINDER_H__5C8B3F68_953D_494E_A28D_B8C8A070744B__INCLUDED_)
