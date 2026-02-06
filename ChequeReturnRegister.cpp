// ChequeReturnRegister.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ChequeReturnRegister.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChequeReturnRegister dialog


CChequeReturnRegister::CChequeReturnRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CChequeReturnRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChequeReturnRegister)
	m_cheque = _T("");
	m_or = _T("");
	m_accode = _T("");
	m_date = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CChequeReturnRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChequeReturnRegister)
	DDX_Control(pDX, IDC_COMBO1, m_cb);
	DDX_Text(pDX, IDC_EDIT1, m_cheque);
	DDX_Text(pDX, IDC_EDIT3, m_or);
	DDX_CBString(pDX, IDC_COMBO1, m_accode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChequeReturnRegister, CDialog)
	//{{AFX_MSG_MAP(CChequeReturnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChequeReturnRegister message handlers

#include "openrs.h"
BOOL CChequeReturnRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CString str;
    if( STCR == St_CR) {
        str.Format("select code from [debtor] where glcode is not null order by code");
    }
    else {
        str.Format("select code from [supplier] order by code");
    }
    OpenRs p1(str);
    if(p1.rows) {
         p1.ToComboBox(m_cb,1);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChequeReturnRegister::IsDataOK()
{
    CString str;

    if( STCR == St_CR) {
        str.Format("select * from [aripay] where [refno]='%s' and [cheque]='%s' and [debtor]='%s'",
            m_or, m_cheque, m_accode);
    }
    else {
        str.Format("select * from [apipay] where [refno]='%s' and [cheque]='%s' and [debtor]='%s'",
            m_or, m_cheque, m_accode);
    }
    BOOL flag = FALSE;
    OpenRs p1(str);
    if(p1.rows) {
         flag = TRUE;
    }
    return flag;
}

void CChequeReturnRegister::MoveToReturnCheque()
{
    CString str;


    if( STCR == St_CR) {
        str.Format("insert into [arrc] select * from [aripay] where [refno]='%s'",
            m_or);
        RunSql(str);

        str.Format("update [arrc] set [date]='%s' where [refno]='%s'",
            DateStr(one.dat[1]),m_or);
        RunSql(str);
/* OOI
        str.Format("insert into [arrcdat] select * from [arpaydat] where [refno]='%s'",
            m_or);
        RunSql(str);
*/
		str.Format("insert into [arrcdat]([REFNO],[SQNO],[INVOICE],[DATE],[TOTAL],[GAINLOSS]) select [REFNO],[SQNO],[INVOICE],[DATE],[TOTAL],[GAINLOSS] from [arpaydat] where [refno]='%s'",
            m_or);
		RunSql(str);
			
        lset(aripay.dat[1], m_or);
        aripay.Search(aripay.dat[1],1,TRUE);
        lset(aripay.dat[IP_RETURN], one.dat[1]);
        lset(aripay.dat[IP_BANKIN], one.dat[1]);
        lset(aripay.dat[IP_REMARKS],"Cheque Return");
        aripay.Update();

        lset(arrc.dat[1], m_or);
        arrc.Search(arrc.dat[1],1,TRUE);
        lset(arrc.dat[IP_DATE], one.dat[1]);
        lset(arrc.dat[IP_BANKIN], one.dat[1]);
        lset(arrc.dat[IP_REMARKS],"Cheque Return");
        arrc.Update();
    }
    else {
        str.Format("insert into [aprc] select * from [apipay] where [refno]='%s'",
            m_or);
        RunSql(str);

        str.Format("update [aprc] set [date]='%s' where [refno]='%s'",
            DateStr(one.dat[1]),m_or);
        RunSql(str);

        //str.Format("insert into [aprcdat] select * from [appaydat] where [refno]='%s'",		
		//m_or);
		str.Format("insert into [aprcdat]([REFNO],[SQNO],[INVOICE],[DATE],[TOTAL],[GAINLOSS]) select [REFNO],[SQNO],[INVOICE],[DATE],[TOTAL],[GAINLOSS] from [appaydat] where [refno]='%s'",
            m_or);
        RunSql(str);

        lset(apipay.dat[1], m_or);
        apipay.Search(apipay.dat[1],1,TRUE);
        lset(apipay.dat[IP_RETURN], one.dat[1]);
        lset(apipay.dat[IP_BANKIN], one.dat[1]);
        apipay.Update();

        lset(aprc.dat[1], m_or);
        aprc.Search(aprc.dat[1],1,TRUE);
        lset(aprc.dat[IP_DATE], one.dat[1]);
        lset(aprc.dat[IP_BANKIN], one.dat[1]);
        lset(aprc.dat[IP_REMARKS],"Cheque Return");
        aprc.Update();
    }
}

void CChequeReturnRegister::OnOK() 
{
    UpdateData();

    lset(one.dat[1],m_date);
    if(IsDataOK()==FALSE) {
        MessageBox("Data incomplete","error message",MB_ICONSTOP);
        CDialog::OnCancel();
        return;
    }
    MoveToReturnCheque();
    MessageBox("Data complete");
    CDialog::OnOK();
}
