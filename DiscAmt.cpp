// DiscAmt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "DiscAmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiscAmt dialog


CDiscAmt::CDiscAmt(CWnd* pParent /*=NULL*/)
	: CDialog(CDiscAmt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiscAmt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDiscAmt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiscAmt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiscAmt, CDialog)
	//{{AFX_MSG_MAP(CDiscAmt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiscAmt message handlers

BOOL CDiscAmt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    SetDlgItemText(IDC_EDIT1,df->dat[OFI_DISCOUNT]);
	SetDlgItemText(IDC_INVNO,df->dat[1]);
    oamt = df->val(OFI_DISCOUNT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiscAmt::OnOK() 
{
    CString str;
    GetDlgItemText(IDC_EDIT1,str);
    oamt = atof(str);
	
	CDialog::OnOK();
}
