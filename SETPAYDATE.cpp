// SETPAYDATE.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "SETPAYDATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATE dialog


CSETPAYDATE::CSETPAYDATE(CWnd* pParent /*=NULL*/)
	: CDialog(CSETPAYDATE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSETPAYDATE)
	m_d1 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CSETPAYDATE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSETPAYDATE)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSETPAYDATE, CDialog)
	//{{AFX_MSG_MAP(CSETPAYDATE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATE message handlers

void CSETPAYDATE::OnOK() 
{	
	lsetblank(getpay);
	lset(getpay->dat[1],m_ticket);
	lset(getpay->dat[2],m_d1);
	getpay->AddNew();
	
	AfxMessageBox("RECORD COMPLETE");

	CDialog::OnOK();
}
