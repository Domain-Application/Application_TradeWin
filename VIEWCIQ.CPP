// ViewCiq.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ViewCiq.h"
#include "Ciq.h"
#include "mgrid.h"
#include "tso.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewCiq dialog


CViewCiq::CViewCiq(UINT IDD,CWnd* pParent /*=NULL*/)
    : CBrowse(IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewCiq)
	m_m4 = _T("");
	m_due = _T("");
	m_m1 = _T("");
	m_m2 = _T("");
	m_m3 = _T("");
	m_current = _T("");
	//}}AFX_DATA_INIT
    bToolBarBottom = FALSE;
}


void CViewCiq::DoDataExchange(CDataExchange* pDX)
{
    CBrowse::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCiq)
	DDX_Text(pDX, IDC_M4, m_m4);
	DDX_Text(pDX, IDC_DUE, m_due);
	DDX_Text(pDX, IDC_M1, m_m1);
	DDX_Text(pDX, IDC_M2, m_m2);
	DDX_Text(pDX, IDC_M3, m_m3);
	DDX_Text(pDX, IDC_CURRENT, m_current);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCiq, CBrowse)
	//{{AFX_MSG_MAP(CViewCiq)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCiq message handlers
void CViewCiq::SetBrowseText()
{
    int idx  = IdxItem ? IdxItem : 1;
    CString text;
    if(df->dat[St_TYPE][0]=='2') {
        if(STCR == St_CR)
           text.Format("Sales Invoice No: %s",df->dat[St_BILLNO]);
        else
           text.Format("Purchase Invoice No: %s",df->dat[St_BILLNO]);
    }
    else if(st1.dat[St_TYPE][0]=='3') {
        text.Format("OR No: %s",df->dat[St_BILLNO]);
    }
    else if(st1.dat[St_TYPE][0]=='4') {
        if(STCR == St_CR)
            text.Format("Credit Note No: %s",df->dat[St_BILLNO]);
        else
            text.Format("Debit Note No: %s",df->dat[St_BILLNO]);
    }
    SetDlgItemText(IDC_BROWSE_TEXT,text);
    SetDlgItemText(IDC_BROWSE_TEXT1,mdate(df->dat[idx]));
    CBrowse::SetBrowseText();
}

HBRUSH CViewCiq::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

BOOL CViewCiq::OnInitDialog() 
{
    //CDialog::OnInitDialog();
	CBrowse::OnInitDialog();
    //bakIdxItem = IdxItem;
    //OnInitFrame();
    this->SetWindowText(customer->dat[2]);
    FM fm;
    DbSum(&fm,df);
    DlgAmount(IDC_DEBIT, fm.acc[St_DR], 15);
    DlgAmount(IDC_CREDIT, fm.acc[St_CR], 15);
    DlgAmount(IDC_BALANCE, fm.acc[St_DR] - fm.acc[St_CR], 15);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern void ShowRCInformation();
void ViewCIQ()
{
    char chType = st1.dat[St_TYPE][0];
/*
    if(chType>='2' && chType<='5') {
        OpenARData();
    }
    if(chType>='A' && chType<='D') {
        OpenAPData();
    }  */

    if(chType=='2' || chType=='A') {
        if(STCR != St_CR) {         //ap module
            lset(inv->dat[1],st1.dat[St_INVNO]);
        }
        else {
            lset(inv->dat[1],st1.dat[St_BILLNO]);
        }
		lset(inv->dat[1],st1.dat[St_BILLNO]);
        ShowInvoiceInformation();
    }
    else if(chType=='3' || chType=='B') {
        lset(ipay->dat[1],st1.dat[St_BILLNO]);
        ShowIpayInformation();
    }
    else if(chType=='4' || chType=='C') {
        lset(cr->dat[1],st1.dat[St_BILLNO]);
        ShowCRInformation();
    }
    else if(chType=='5' || chType=='D') {
        lset(rc->dat[1],st1.dat[St_BILLNO]);
        ShowRCInformation();
    }
}

void ComputeOverFourMonthAgeing()
{
    double o4 = 0;
    char   tmp[20];

    for(int i=4;i<12;i++) {
        o4 += Over_Balance[i];
    }
    dtoa(o4,tmp,12,2);
    strcpy(Ob[4],putnum(tmp));

    for(i=0;i<4;i++) {
        dtoa(Over_Balance[i],tmp,11,2);
        strcpy(Ob[i],putnum(tmp));
        o4 += Over_Balance[i];
    }
    dtoa(o4,tmp,12,2);
    strcpy(Ob[RCA],putnum(tmp));
}

long viewidx_ciq(DB *df,UINT ID)
{
    UINT IDD=ID;

    if(IDD==0) IDD=df->BROWSE_ID;
    CViewCiq b1(IDD);
    b1.df       = df;
    b1.IdxItem  = df->f[1].idx & 0x01;
    b1.EditFlag = 0;
    b1.m_SetTitleFlag = TRUE;
    b1.title    = "INQUIRY";
    b1.m_due = Ob[RCA];
    b1.m_current = Ob[0];
    b1.m_m1 = Ob[1];
    b1.m_m2 = Ob[2];
    b1.m_m3 = Ob[3];
    b1.m_m4 = Ob[4];
    b1.DoModal();

    df->rec = b1.no;
    ridx(df->rec,df,b1.IdxItem);
    return df->rec;
}

void SetCiqGridRows(CMSFlexGrid *grid,DB *df)
{
    grid->SetRows(21);
}

void ReplaceCIQGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,0,St_DATE,St_NAME,St_BILLNO,St_FLAG,St_DR,St_CR,St_BALANCE,-1);
}

void SetCIQGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,8);
    g.SetTitle(df,0,St_DATE,-1);
    g.SetTitle("Description",2400,1);
    g.SetTitle("Ref. No",1100,1);
    g.SetTitle("",200,1);
    g.SetTitle("Debit",1200,7);
    g.SetTitle("Credit",1200,7);
    g.SetTitle("Balance",1500,7);
}

void init_st1(DB *df)
{
    df->e->SetGridRows = SetCiqGridRows;
    df->e->rep         = ReplaceCIQGrid;
    df->e->GridTitle   = SetCIQGridTitle;
}

void CustomerInquired()
{
    FLASH = 0;
    rbopen(&st1,"st1.rep",init_st1);
    ropen(&st,"st.rep");
    tado(&ciq,"ciq");
    rbopen(&tso,"tso.scr",init_tso);
    ropen(&report,"satb.rep");
    setwild(&tso);
    setwild(&one);
	
    CCiq b1(IDD_CIQ);

    b1.df       = customer;
    b1.IdxItem  = 1;
    b1.EditFlag = 0;
    b1.DoModal();

    dclose(&st1);
    dclose(&st);
    dclose(&tso);
    dclose(&ciq);
    dclose(&report);
    FLASH = 1;
}
