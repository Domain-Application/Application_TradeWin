// AdjOffset.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "AdjOffset.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjOffset dialog

DB tmpinv;
enum invbal {Inv_DATE=1,Inv_NAME,Inv_BAL};
CAdjOffset::CAdjOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjOffset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdjOffset)
    m_invoiceno = _T("");
	m_balamt = _T("");
	m_offsetamt = _T("");
	//}}AFX_DATA_INIT
    if (!m_ok.LoadBitmaps("OKU", "OKD", "OKF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
    if (!m_cancel.LoadBitmaps("CANCELU", "CANCELD", "CANCELF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
	fontPitch = 6;
}


void CAdjOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjOffset)
	DDX_Control(pDX, IDC_LIST1, m_ListView);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
    DDX_Text(pDX, IDC_EDIT1, m_invoiceno);
	DDX_Text(pDX, IDC_EDIT2, m_balamt);
	DDX_Text(pDX, IDC_EDIT3, m_offsetamt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjOffset, CDialog)
	//{{AFX_MSG_MAP(CAdjOffset)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjOffset message handlers
void CAdjOffset::ShowReportOption(DB *df)
{
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_LEFT;

    for (uint j = 1; j <= df->sumfield; j++) {
        int len = max(df->f[j].len,strlen(df->f[j].name));
        lvc.cx = len * fontPitch + 20;
        lvc.pszText = df->f[j].name;
        lvc.iSubItem = j;
        lvc.fmt = LVCFMT_LEFT;
        if(df->f[j].mode=='N') {
            lvc.fmt = LVCFMT_RIGHT;
        }
        m_ListView.InsertColumn(j-1, &lvc);
    }

    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iSubItem = 0;
	
	char szDate[20];
	CString strDate;
    for (int rec = 1; rec <= df->t->sumrecord; rec++) {
        readrec(rec,df);
        lvi.iItem = rec-1;
        lvi.iImage = rec-1;
        if(df->f[1].mode=='D') {
			strDate = mbdate(df->dat[1]);
			lset(szDate,strDate);
            lvi.pszText = szDate;
        }
        else {
            lvi.pszText = df->dat[1];
        }
        lvi.cchTextMax = df->sumfield - 1;
        m_ListView.InsertItem(&lvi);

        for (uint k = 2; k <= df->sumfield; k++) {
            if(df->f[k].mode=='D') {
                m_ListView.SetItemText(rec-1, k-1, mbdate(df->dat[k]));
            }
            else if(df->f[k].mode=='N') {
                m_ListView.SetItemText(rec-1, k-1, putnum(df->dat[k]));
            }
            else {
                m_ListView.SetItemText(rec-1, k-1, df->dat[k]);
            }
        }
    }

	HCURSOR m_hStdHotCursor;
    m_ListView.SetExtendedStyle(LVS_EX_TRACKSELECT |
        LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT);
    m_ListView.SetHoverTime(1500);
    m_hStdHotCursor = m_ListView.GetHotCursor();
	m_ListView.SetHotCursor(m_hStdHotCursor);
}

BOOL CAdjOffset::OnInitDialog() 
{
    CDialog::OnInitDialog();

    ropen(&report,"invbal.rep");
    InvoiceRealBalance();
    ShowReportOption(&report);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

double CAdjOffset::InvoiceRealBalance()
{
    char tmp[20],text[25];

    lset(inv->dat[1],m_invoiceno);
    strcpy(text,m_invoiceno);
    qd(text,inv);
    INV_total = inv->val(IV_TOTAL);
    INV_pay = 0;
    memstart(&report);
    lset(report.dat[Inv_DATE],inv->dat[IV_DATE]);
    if(STCR == St_CR) {
        lset(report.dat[Inv_NAME],"Sales");
    }
    else {
        lset(report.dat[Inv_NAME],"Purchase");
    }
    dtodb(INV_total,&report,Inv_BAL);
    memwrt(&report);

    DB adoTmp;
    CString strPay;
    if(STCR == St_CR) {
        strPay.Format("select * from [invarpay] where [invoice]='%s'",text);
    }
    else {
        strPay.Format("select * from [invappay] where [invoice]='%s'",text);
    }
    ado(&adoTmp,"",NULL,strPay);
    long last = read_sumindex(1,&adoTmp);
    for(long rec=1;rec<=last;rec++) {
        adoFastRead(rec,&adoTmp);
        double pamt = adoTmp.val(4);
        lset(report.dat[Inv_DATE],adoTmp.dat[2]);
        lset(report.dat[Inv_NAME],adoTmp.dat[3]);
        dtodb(pamt*-1,&report,Inv_BAL);
        memwrt(&report);
        INV_pay += pamt;
    }
    dclose(&adoTmp);

    INV_cr = 0;
    if(STCR == St_CR) {
        strPay.Format("select * from [crarpay] where [invoice]='%s'",text);
    }
    else {
        strPay.Format("select * from [crappay] where [invoice]='%s'",text);
    }
    ado(&adoTmp,"",NULL,strPay);
    last = read_sumindex(1,&adoTmp);
    for(rec=1;rec<=last;rec++) {
        adoFastRead(rec,&adoTmp);
        double pamt = adoTmp.val(4);
        lset(report.dat[Inv_DATE],adoTmp.dat[2]);
        lset(report.dat[Inv_NAME],adoTmp.dat[3]);
        dtodb(pamt*-1,&report,Inv_BAL);
        memwrt(&report);
        INV_cr += pamt;
    }
    dclose(&adoTmp);

    INV_rc = 0;
    if(STCR == St_CR) {
        strPay.Format("select * from [rcarpay] where [invoice]='%s'",text);
    }
    else {
        strPay.Format("select * from [rcappay] where [invoice]='%s'",text);
    }
    ado(&adoTmp,"",NULL,strPay);
    last = read_sumindex(1,&adoTmp);
    for(rec=1;rec<=last;rec++) {
        adoFastRead(rec,&adoTmp);
        double pamt = adoTmp.val(4);
        lset(report.dat[Inv_DATE],adoTmp.dat[2]);
        lset(report.dat[Inv_NAME],adoTmp.dat[3]);
        dtodb(pamt,&report,Inv_BAL);
        memwrt(&report);
        INV_rc += pamt;
    }
    dclose(&adoTmp);
	
    double bal = INV_bal = INV_total-INV_pay-INV_cr+INV_rc;
    dtoa(bal,tmp,17,2);
    m_balamt = tmp;
    UpdateData(FALSE);

    lsetblank(&report);
    memwrt(&report);
    memwrt(&report);

    rset(report.dat[Inv_NAME],"Balance:");
    dtodb(bal,&report,Inv_BAL);
    memwrt(&report);

    memend(&report);
    return atof(tmp);
}

void CAdjOffset::OnCancel() 
{
    dclose(&report);
	CDialog::OnCancel();
}

void CAdjOffset::OnOK() 
{
    dclose(&report);
	CDialog::OnOK();
}

void CAdjOffset::OnTimer(UINT nIDEvent) 
{
	
	CDialog::OnTimer(nIDEvent);
}
