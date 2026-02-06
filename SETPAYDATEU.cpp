// SETPAYDATEU.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "SETPAYDATEU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATEU dialog


CSETPAYDATEU::CSETPAYDATEU(CWnd* pParent /*=NULL*/)
	: CDialog(CSETPAYDATEU::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSETPAYDATEU)
	m_b1 = COleDateTime::GetCurrentTime();
	ispaydate = TRUE;
	//}}AFX_DATA_INIT
}


void CSETPAYDATEU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSETPAYDATEU)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_b1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSETPAYDATEU, CDialog)
	//{{AFX_MSG_MAP(CSETPAYDATEU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSETPAYDATEU message handlers

void CSETPAYDATEU::OnOK() 
{
	UpdateData(TRUE);
//	long Srec = updatepay->Search(b_ticket,1,TRUE);
	long Srec = qd(main->dat[1],updatepay);
	if(Srec){
		lset(updatepay->dat[2],m_b1);
		updatepay->Update();
		AfxMessageBox("RECORD COMPLETE");
	}
	else{
		lsetblank(updatepay);
		lset(updatepay->dat[1],main->dat[1]);
		lset(updatepay->dat[2],m_b1);
		updatepay->AddNew();
		AfxMessageBox("RECORD COMPLETE");
	}
	CDialog::OnOK();
}

BOOL CSETPAYDATEU::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (ispaydate==FALSE){
		SetWindowText("Set Arrived Date");
	}
	else SetWindowText("Set Payment Date");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
