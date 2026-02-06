#if !defined(AFX_TRANSFERDIRECTTICKETTOMAINSYSTEM_H__E439C5F1_C5DD_463C_BBA4_6032A3B18DE9__INCLUDED_)
#define AFX_TRANSFERDIRECTTICKETTOMAINSYSTEM_H__E439C5F1_C5DD_463C_BBA4_6032A3B18DE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferDirectTicketToMainSystem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransferDirectTicketToMainSystem dialog

class CTransferDirectTicketToMainSystem : public CDialog
{
// Construction
public:
	CTransferDirectTicketToMainSystem(CWnd* pParent = NULL);   // standard constructor
    DB *df,*sub;

// Dialog Data
	//{{AFX_DATA(CTransferDirectTicketToMainSystem)
	enum { IDD = IDD_TRANSFER_DIRECT_TICKET_TO_MAIN_SYSTEM };
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferDirectTicketToMainSystem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransferDirectTicketToMainSystem)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDIRECTTICKETTOMAINSYSTEM_H__E439C5F1_C5DD_463C_BBA4_6032A3B18DE9__INCLUDED_)
