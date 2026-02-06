// AutoOffsetInvoice.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "AutoOffsetInvoice.h"
#include "Ipay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoOffsetInvoice dialog


CAutoOffsetInvoice::CAutoOffsetInvoice(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoOffsetInvoice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoOffsetInvoice)
	m_d1 = COleDateTime::GetCurrentTime();
	m_d2 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CAutoOffsetInvoice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoOffsetInvoice)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoOffsetInvoice, CDialog)
	//{{AFX_MSG_MAP(CAutoOffsetInvoice)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_COMMAND(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnOffsetOneByOne)
	ON_BN_CLICKED(IDC_BUTTON5, OnOffsetOneByOneTicket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoOffsetInvoice message handlers

void CAutoOffsetInvoice::OnButton1() 
{
    OnButton2();
	CDialog::OnOK();
}

void CAutoOffsetInvoice::OnButton2() 
{
    CIpay *p = (CIpay *) GetParent();
    for(;;) {
        if(receipt.t->sumrecord==0) return;
        CString str;
        str.Format("select sum(total) from [%s] where [%s]= %s",
            sub->m_strTable, sub->fieldName[1], DbText(df,1) );
        double receipt_amt = SqlValue(str);
        double balance = p->PaydatTotal();
        if(balance <=0) break;
        ridx(1,&receipt,0);
        double offset = receipt.val(OFI_IBAL);
        lsetblank(sub);
        lset(sub->dat[1],df->dat[1]);
        lset(sub->dat[PY_INVOICE],receipt.dat[OFI_INVOICE]);
        lset(sub->dat[PY_TOTAL],receipt.dat[OFI_IBAL]);
        if(balance>=offset) {
            dtodb(offset,sub,PY_TOTAL);
        }
        else {
            dtodb(balance,sub,PY_TOTAL);
        }
        qd(sub->dat[PY_INVOICE], inv);
        lset(sub->dat[PY_DATE],df->dat[IP_DATE]);
        if(strcmp(inv->dat[IV_DATE],sub->dat[PY_DATE])>0) {
            lset(sub->dat[PY_DATE],inv->dat[IV_DATE]);
        }
		//OOI
		GetPaydatGainLoss(df,sub);
		//
        sub->AddNew();
        waitflock(&receipt);
        deleterec(1,&receipt);
        unflock(&receipt);
    }
}

BOOL CAutoOffsetInvoice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int nYear = year_no(two.dat[1]);
    int nMonth = month_no(two.dat[1]);
    int nDay = day_no(two.dat[1]);

    if(check_valid_date(nYear,nMonth,nDay)) {
       m_d1.SetDate(nYear,nMonth,nDay);
    }
    nYear = year_no(two.dat[2]);
    nMonth = month_no(two.dat[2]);
    nDay = day_no(two.dat[2]);

    if(check_valid_date(nYear,nMonth,nDay)) {
        m_d2.SetDate(nYear,nMonth,nDay);
    }
    UpdateData(FALSE);
    if(df->val(IP_TOTAL) >0) {
        GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
        GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(FALSE);
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }
    else {
        GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoOffsetInvoice::OnOK() 
{
	UpdateData();
	lset(two.dat[1], m_d1);	
    lset(two.dat[2], m_d2);

    dtodb(999999, df, IP_TOTAL);
    long last = receipt.t->sumrecord;
    for(long rec=last; rec>=1; rec--) {
        readrec(rec, &receipt);
        if(!(realm( two.dat[1], receipt.dat[RE_DATE], two.dat[2])) ) {
            waitflock(&receipt);
            deleterec(rec, &receipt);
            unflock(&receipt);
        }
    }
    OnButton2();
    CString str;
    str.Format("select sum(total) from [%s] where [%s]= %s",
        sub->m_strTable, sub->fieldName[1], DbText(df,1) );
    double receipt_amt = SqlValue(str);
    dtodb(receipt_amt, df, IP_TOTAL);
    df->Update();

	CDialog::OnOK();
}

void CAutoOffsetInvoice::OnOffsetOneByOne() 
{
    CIpay *p = (CIpay *) GetParent();
    p->get_invoice_balance(df,sub,IP_TOTAL,1);
	
	CDialog::OnOK();
}

void CAutoOffsetInvoice::OnOffsetOneByOneTicket() 
{
    CIpay *p = (CIpay *) GetParent();
    p->get_invoice_balance(df,sub,IP_TOTAL,0);
	
	CDialog::OnOK();
	
}
