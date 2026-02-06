// ViewInv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ViewInv.h"
#include "mgrid.h"
#include "db_ado.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewInv dialog


CViewInv::CViewInv(UINT IDD,CWnd* pParent /*=NULL*/)
    : CBrowse(IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewInv)
	m_m4 = _T("");
	m_due = _T("");
	m_m1 = _T("");
	m_m2 = _T("");
	m_m3 = _T("");
	m_current = _T("");
	//}}AFX_DATA_INIT
    bToolBarBottom = FALSE;
}


void CViewInv::DoDataExchange(CDataExchange* pDX)
{
    CBrowse::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewInv)
	DDX_Text(pDX, IDC_M4, m_m4);
	DDX_Text(pDX, IDC_DUE, m_due);
	DDX_Text(pDX, IDC_M1, m_m1);
	DDX_Text(pDX, IDC_M2, m_m2);
	DDX_Text(pDX, IDC_M3, m_m3);
	DDX_Text(pDX, IDC_CURRENT, m_current);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewInv, CBrowse)
    //{{AFX_MSG_MAP(CViewInv)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnViewDetailTransaction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewInv message handlers
void CViewInv::SetBrowseText()
{
    CString text;
    text.Format("Sales Invoice No: %s",df->dat[1]);
    SetDlgItemText(IDC_BROWSE_TEXT,text);
    SetDlgItemText(IDC_BROWSE_TEXT1,mdate(df->dat[2]));
    CBrowse::SetBrowseText();
}

HBRUSH CViewInv::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

BOOL CViewInv::OnInitDialog()
{
    CBrowse::OnInitDialog();
    //bakIdxItem = IdxItem;
    //OnInitFrame();
    ComputeOverFourMonthAgeing();
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewInv::ComputeOverFourMonthAgeing()
{
    long last = read_sumindex(0,df);

    double tm[9];
    memset(tm , 0, sizeof(tm));
    double total=0;
    for(long rec=1; rec<=last; rec++) {
        readrec(rec, df);
        int days = atoi(df->dat[RE_DAY]);
        total += df->val(RE_BAL);
        if(days>=0 && days<=30) {
            tm[0] += df->val(RE_BAL);
        }
        else if(days>30 && days<=60) {
            tm[1] += df->val(RE_BAL);
        }
        else if(days>60 && days<=90) {
            tm[2] += df->val(RE_BAL);
        }
        else if(days>90 && days<=120) {
            tm[3] += df->val(RE_BAL);
        }
        else if(days>120) {
            tm[4] += df->val(RE_BAL);
        }
    }
    char szTmp[20];
    dtoa(total, szTmp, 12, 2);
    m_due = putnum(szTmp);
    for(int i=0; i<=4; i++) {
        dtoa(tm[i], szTmp, 12, 2);
        if(i==0) {
            m_current = putnum(szTmp);
        }
        if(i==1) {
            m_m1 = putnum(szTmp);
        }
        else if(i==2) {
            m_m2 = putnum(szTmp);
        }
        else if(i==3) {
            m_m3 = putnum(szTmp);
        }
        else if(i==4) {
            m_m4 = putnum(szTmp);
        }
    }
}

long viewinv(DB *df,UINT ID)
{
    UINT IDD=ID;

    if(IDD==0) IDD=df->BROWSE_ID;
    CViewInv b1(IDD);
    b1.df       = df;
    b1.IdxItem  = df->f[1].idx & 0x01;
    b1.EditFlag = 0;
    b1.m_SetTitleFlag = TRUE;
    b1.title    = "Oustanding Invoice";

    b1.DoModal();

    df->rec = b1.no;
    ridx(df->rec,df,b1.IdxItem);
    return df->rec;
}

void SQL_Method()
{
    memstart(&st1);
    CString str;
    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, ARINV.AMOUNT AS DEBIT, '2' AS [TYPE] FROM ARINV WHERE DEBTOR = '%s'",customer->dat[1]);

    OpenRs p1(str);
    if(p1.rows) {
         p1.dupcpy(&st1,FALSE);
    }

    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS CREDIT, '4' AS [TYPE] FROM ARCR WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p2(str);
    if(p2.rows) {
         p2.dupcpy(&st1,FALSE);
    }
    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS CREDIT, '3' AS [TYPE] FROM ARIPAY WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p3(str);
    if(p3.rows) {
         p3.dupcpy(&st1,FALSE);
    }

    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS DEBIT, '5' AS [TYPE] FROM ARRC WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p31(str);
    if(p31.rows) {
         p31.dupcpy(&st1,FALSE);
    }

    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS CREDIT, 'A' AS [TYPE] FROM APINV WHERE DEBTOR = '%s'",customer->dat[1]);

    OpenRs p4(str);
    if(p4.rows) {
         p4.dupcpy(&st1,FALSE);
    }

    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS DEBIT, 'C' AS [TYPE] FROM APCR WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p5(str);
    if(p5.rows) {
         p5.dupcpy(&st1,FALSE);
    }
    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS DEBIT, 'B' AS [TYPE] FROM APIPAY WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p6(str);
    if(p6.rows) {
         p6.dupcpy(&st1,FALSE);
    }

    str.Format("SELECT [DATE] as IDATE,[DATE],[REFNO] AS INVNO,[REFNO], PARTICULAR AS NAME, AMOUNT AS CREDIT, 'D' AS [TYPE] FROM APRC WHERE DEBTOR = '%s'",customer->dat[1]);
    OpenRs p7(str);
    if(p7.rows) {
         p7.dupcpy(&st1,FALSE);
    }

    memend(&st1);
    arrage_st(&st1);
}

void CViewInv::OnViewDetailTransaction() 
{
    rbopen(&st1,"st1.rep",init_st1);

    SQL_Method();
    ::ComputeOverFourMonthAgeing();

    for(;;) {
        st1.rec = viewidx_ciq(&st1,IDD_VIEWCIQ);
        if(st1.rec ==0) break;
        ViewCIQ();
    }
	
    dclose(&st1);
}
