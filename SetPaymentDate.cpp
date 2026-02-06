// SetPaymentDate.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "SetPaymentDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetPaymentDate dialog


CSetPaymentDate::CSetPaymentDate(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPaymentDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetPaymentDate)
	m_d1 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CSetPaymentDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetPaymentDate)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetPaymentDate, CDialog)
	//{{AFX_MSG_MAP(CSetPaymentDate)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetPaymentDate message handlers
DB paymentdate;
void CSetPaymentDate::OnOK()
{
	lsetblank(&paymentdate);
	lset(paymentdate.dat[1],df->dat[1]);
	lset(paymentdate.dat[2],m_d1);
	paymentdate.AddNew();

	AfxMessageBox("RECORD COMPLETE");
}