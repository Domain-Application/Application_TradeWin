// ChequeNo.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ChequeNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChequeNo dialog


CChequeNo::CChequeNo(CWnd* pParent /*=NULL*/)
	: CDialog(CChequeNo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChequeNo)
	m_date = COleDateTime::GetCurrentTime();
	m_cheque = _T("");
	//}}AFX_DATA_INIT
}


void CChequeNo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChequeNo)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Text(pDX, IDC_EDIT1, m_cheque);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChequeNo, CDialog)
	//{{AFX_MSG_MAP(CChequeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChequeNo message handlers

BOOL CChequeNo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	struct tm pst;
    sever_date(&pst,one.dat[1]);
    m_date.SetDate(pst.tm_year,pst.tm_mon,pst.tm_mday);

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChequeNo::OnOK() 
{
	CString str;
    str.Format("%4.4i%2.2i%2.2i",m_date.GetYear(),m_date.GetMonth(),m_date.GetDay());
    lset(one.dat[1],str);
	
	CDialog::OnOK();
}
