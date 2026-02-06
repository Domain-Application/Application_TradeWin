// OffsetAmt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "OffsetAmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COffsetAmt dialog


COffsetAmt::COffsetAmt(CWnd* pParent /*=NULL*/)
	: CDialog(COffsetAmt::IDD, pParent)
{
	//{{AFX_DATA_INIT(COffsetAmt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COffsetAmt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COffsetAmt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COffsetAmt, CDialog)
	//{{AFX_MSG_MAP(COffsetAmt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COffsetAmt message handlers

BOOL COffsetAmt::OnInitDialog() 
{
	CDialog::OnInitDialog();

    oamt = 0;
    SetDlgItemText(IDC_EDIT1,df->dat[OFI_PAY]);
	SetDlgItemText(IDC_INVNO,df->dat[1]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COffsetAmt::OnOK() 
{
    CString str;
    GetDlgItemText(IDC_EDIT1,str);
    if(df->val(OFI_IBAL) < atof(str)) {
        MessageBox("can not over pay");
        GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
        return;
    }
    oamt = atof(str);
	CDialog::OnOK();
}

void COffsetAmt::OnCancel() 
{
    oamt = 0;
	CDialog::OnCancel();
}
