// ResetPassword.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ResetPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResetPassword dialog


CResetPassword::CResetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CResetPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetPassword)
	m_password = _T("");
	//}}AFX_DATA_INIT
}

void CResetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetPassword)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_EDIT1, m_password);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResetPassword, CDialog)
	//{{AFX_MSG_MAP(CResetPassword)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetPassword message handlers

void CResetPassword::RunSql(CString str)
{
    m_progress.StepIt();
    ::RunSql(str);
}

void CResetPassword::OnOK() 
{
    UpdateData();


	CString str;
    str.Format("DOMAIN-%s",today);
    if(m_password.Compare(str)) {
        MessageBox("Incorret password");
        return;
    }

    m_progress.SetRange(1,20);
    str = "truncate table [aroinv]";
	RunSql(str);

    str = "delete from [arinv]";
	RunSql(str);

    str = "truncate table [apoinv]";
	RunSql(str);

    str = "delete from [apinv]";
	RunSql(str);

    str = "truncate table [wds]";
	RunSql(str);

    str = "truncate table [wdp]";
	RunSql(str);

    str = "truncate table [ds]";
	RunSql(str);

    str = "truncate table [dp]";
	RunSql(str);

    str = "truncate table [poitem]";
	RunSql(str);

    str = "delete from [po]";
	RunSql(str);

    str = "truncate table [soitem]";
	RunSql(str);

    str = "delete from [so]";
	RunSql(str);

    str = "truncate table [dtitem]";
	RunSql(str);

    str = "truncate table [dt]";
	RunSql(str);

    str = "truncate table [fc_dp]";
	RunSql(str);

    str = "truncate table [fc_ds]";
	RunSql(str);

    str = "truncate table [ic]";
	RunSql(str);

    str = "truncate table [rptlog]";
	RunSql(str);
/*
    str = "truncate table [pt]";
	RunSql(str);

    str = "delete from [category]";
	RunSql(str);

  str = "delete from [ptgroup]";
  RunSql(str);
*/
    str = "truncate table [spitem]";
	RunSql(str);

    str = "DELETE FROM [accode]";
	RunSql(str);

    str = "truncate table [lorry]";
	RunSql(str);
    
	str = "truncate table [transp]";
	RunSql(str);

    str = "truncate table [grade]";
	RunSql(str);

    str = "truncate table [staff]";
	RunSql(str);

    str = "truncate table [ReportFilterDescription]";
	RunSql(str);

    str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'ds'";
	RunSql(str);

    str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'dp'";
	RunSql(str);

    str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'wds'";
	RunSql(str);

    str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'wdp'";
	RunSql(str);

	str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'dt'";
	RunSql(str);
	
	str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'arinv'";
	RunSql(str);
	
	str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'apinv'";
	RunSql(str);

	str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'so'";
	RunSql(str);

	str = "update [ticketno] set [NEXTNO] = 1 where [recordset] = 'po'";
	RunSql(str);

    MessageBox("Completed");

	CDialog::OnOK();
}

void CleanDatabase()
{
    CResetPassword h1;

    h1.DoModal();
}

void CResetPassword::OnButton1() 
{
    UpdateData();

	CString str;

    m_progress.SetRange(1,20);

    str = "truncate table [rptlog]";
	RunSql(str);

    MessageBox("Completed");

	CDialog::OnOK();
	
}
