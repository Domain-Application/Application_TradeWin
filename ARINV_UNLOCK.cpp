// ARINV_UNLOCK.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ARINV_UNLOCK.h"
#include "db_ado.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CARINV_UNLOCK dialog


CARINV_UNLOCK::CARINV_UNLOCK(CWnd* pParent /*=NULL*/)
	: CDialog(CARINV_UNLOCK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CARINV_UNLOCK)
	m_d1 = COleDateTime::GetCurrentTime();
	m_d2 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CARINV_UNLOCK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CARINV_UNLOCK)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CARINV_UNLOCK, CDialog)
	//{{AFX_MSG_MAP(CARINV_UNLOCK)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARINV_UNLOCK message handlers

void CARINV_UNLOCK::OnOK() 
{
    UpdateData();
    lset(two.dat[1], m_d1);
    lset(two.dat[2], m_d2);
	
    CString d1 = DbText(&two,1);
    CString d2 = DbText(&two,2);
    CString str;
	if (Purchase_Flag){
		str.Format("Update APINV_WB set [rec_status] = 0 where [date]>= %s and [date]<= %s", d1, d2);
	}
	else{
		str.Format("Update ARINV_WB set [rec_status] = 0 where [date]>= %s and [date]<= %s", d1, d2);
	}
    RunSql(str);
	
	CDialog::OnOK();
}