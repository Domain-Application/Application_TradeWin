// PayAllocate.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "mgrid.h"
#include "PayAllocate.h"
#include "OffsetAmt.h"
#include "DiscAmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPayAllocate dialog


CPayAllocate::CPayAllocate(UINT IDD,CWnd* pParent /*=NULL*/)
    : CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CPayAllocate)
	m_BalanceAllocated = _T("");
	//}}AFX_DATA_INIT
    bScheme = FALSE;
}


void CPayAllocate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPayAllocate)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	DDX_Text(pDX, IDC_EDIT1, m_BalanceAllocated);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPayAllocate, CDialog)
	//{{AFX_MSG_MAP(CPayAllocate)
	ON_BN_CLICKED(IDC_BUTTON1, OnAutoMatch)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnActivePaymentScheme)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPayAllocate message handlers

BEGIN_EVENTSINK_MAP(CPayAllocate, CDialog)
    //{{AFX_EVENTSINK_MAP(CPayAllocate)
	ON_EVENT(CPayAllocate, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPayAllocate::ReplaceGrid(long rec)
{
    MGRID g(&m_grid,df,rec);

    if(bScheme==FALSE) {
        g.SetText(df,0,OFI_INVOICE,OFI_DATE,OFI_DAY,
             OFI_TOTAL,OFI_BAL,OFI_ACC,OFI_PAY,-1);
    }
    else {
        g.SetText(df,0,OFI_INVOICE,OFI_DATE,OFI_DAY,
             OFI_TOTAL,OFI_DISCOUNT,OFI_BAL,OFI_ACC,OFI_PAY,-1);
    }
    if(df->val(OFI_BAL)==0) {
        m_grid.SetCellBackColor(RGB(255,255,0));
		return;
    }
    double pamt = df->val(OFI_PAY);
    if(pamt>0) {
        m_grid.SetCellBackColor(RGB(255,0,255));
		return;
    }
    else {
        m_grid.SetCellBackColor(RGB(255,255,255));
    }
}

void CPayAllocate::SetGridTitle()
{
    if(bScheme==FALSE) {
        MGRID g(&m_grid,df,0,8);
        g.ratio = float(1.1);
        g.SetTitle(df,0,OFI_INVOICE,OFI_DATE,OFI_DAY,OFI_TOTAL,OFI_BAL,OFI_ACC,OFI_PAY,-1);
    }
    else {
        MGRID g(&m_grid,df,0,9);

        g.SetTitle(df,0,OFI_INVOICE,OFI_DATE,OFI_DAY,OFI_TOTAL,OFI_DISCOUNT,OFI_BAL,OFI_ACC,OFI_PAY,-1);
    }
}

void CPayAllocate::Refresh()
{
    long last = df->t->sumrecord;
    double itotal,ibal,ptotal,dtotal,iacc;

    itotal=ibal=ptotal=dtotal=iacc=0;
    m_grid.SetRedraw(FALSE);
    m_grid.SetRows(last+1);
    m_progress.SetRange32(1,last);
    for(long rec=1; rec<=last ;rec++) {
        readrec(rec,df);
        itotal += df->val(OFI_TOTAL);
        iacc   += df->val(OFI_BAL);
        ibal   += df->val(OFI_BAL);
        ptotal += df->val(OFI_PAY);
        dtotal += df->val(OFI_DISCOUNT);
        dtodb(iacc,df,OFI_ACC);
        wrtfield(rec,OFI_ACC,df);
        m_progress.SetPos(rec);
        m_grid.SetRow(rec);
        ReplaceGrid(rec);
    }
	m_grid.SetRedraw(TRUE);

    char tmp[20];
    dtoa(ptotal,tmp,17,2);
    SetDlgItemText(IDC_PTOTAL,putnum0(tmp));
    dtoa(BalanceAllocated,tmp,17,2);
    SetDlgItemText(IDC_EDIT1,putnum0(tmp));
}

BOOL CPayAllocate::OnInitDialog() 
{
	CDialog::OnInitDialog();
    obal = BalanceAllocated;
    int field[] = {OFI_INVOICE,OFI_DATE,OFI_DAY,0};
    sort(df,field);

    bClearFlag = FALSE;
	m_list.AddString("Invoice No");
	m_list.AddString("Invoice Date");
	m_list.AddString("Over due days");
	m_list.SetSel(0);
    if(df->t->sumrecord==1) {
        GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
    }

    long xrec = qd(customer_code,customer);
    if(xrec==0) {
        bScheme=FALSE;
    }
    xrec = qd(customer->dat[CU_SCHEME],&scheme);
    if(xrec==0 || STCR != St_CR) {           //payment scheme only for ar
        bScheme=FALSE;
    }
    SetGridTitle();
    if(bScheme==TRUE) {
        OnInitialPaymentScheme();
    }
    GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
    bClearFlag = TRUE;
    OnAutoMatch();
    OnAutoMatch();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPayAllocate::OnAutoAdjust(long rec,long last)
{
    if(BalanceAllocated>0) {
        m_progress.SetRange32(1,last);
        for(rec=rec; rec<=last && BalanceAllocated; rec++) {
            readrec(rec,df);
            m_progress.SetPos(rec);
            double ibal = df->val(OFI_BAL);
            if(ibal==0) continue;
            if(BalanceAllocated>=ibal) {
                dtodb(0,df,OFI_BAL);
                dtodb(ibal,df,OFI_PAY);
                BalanceAllocated -= ibal;
            }
            else {
                if(df->val(OFI_IBAL)==df->val(OFI_BAL)) {
                    dtodb(ibal - BalanceAllocated,df,OFI_BAL);
                    dtodb(BalanceAllocated,df,OFI_PAY);
                }
                else {
                    decfld(df,OFI_BAL,BalanceAllocated);
                    incfld(df,OFI_PAY,BalanceAllocated);
                }
                BalanceAllocated = 0;
            }
            fastwrt(rec,df);
        }
    }
}

void CPayAllocate::OnAutoMatch() 
{
    long last = df->t->sumrecord;
    double ibal;

    BalanceAllocated = obal;
    if(bClearFlag==FALSE) {
        m_progress.SetRange32(1,last);
        for(long rec=1; rec<=last && BalanceAllocated>0;rec++) {
            readrec(rec,df);
            m_progress.SetPos(rec);
            ibal = df->val(OFI_BAL);
            if(BalanceAllocated>=ibal) {
                dtodb(0,df,OFI_BAL);
                dtodb(ibal,df,OFI_PAY);
                BalanceAllocated -= ibal;
            }
            else {
                dtodb(ibal - BalanceAllocated,df,OFI_BAL);
                dtodb(BalanceAllocated,df,OFI_PAY);
                BalanceAllocated = 0;
            }
            fastwrt(rec,df);
        }
    }
    else {
        m_progress.SetRange32(1,last);
        for(long rec=1; rec<=last; rec++) {
            readrec(rec,df);
            m_progress.SetPos(rec);
            ibal = df->val(OFI_IBAL) - df->val(OFI_DISCOUNT);
            dtodb(ibal,df,OFI_BAL);
            dtodb(0,df,OFI_PAY);
            fastwrt(rec,df);
        }
    }
    if(bClearFlag==FALSE) {
        SetDlgItemText(IDC_BUTTON1,"&Clear");
    }
    else {
        SetDlgItemText(IDC_BUTTON1,"&Auto Match");
    }
    bClearFlag = !bClearFlag;
    Refresh();
}

void CPayAllocate::OnDblclkList1() 
{
    int fld = m_list.GetCurSel()+1;
	TRACE("FLD: %i\n",fld);
    if(fld==1) {
        int field[] = {OFI_DATE,OFI_INVOICE,OFI_DAY,0};
        sort(df,field);
	}
    else if(fld==2) {
        int field[] = {OFI_INVOICE,OFI_DATE,OFI_DAY,0};
        sort(df,field);
	}
    else {
        int field[] = {OFI_DAY*-1,OFI_INVOICE,OFI_DATE,0};
        sort(df,field);
	}
    Refresh();
}

double CPayAllocate::FindPaymentSchemeDiscount()
{
    long xrec = qd(ipay->dat[IP_CUSTOMER],customer);
    if(xrec==0) return 0;
    xrec = qd(customer->dat[CU_SCHEME],&scheme);
    if(xrec==0) return 0;

    double total = df->val(OFI_IBAL);
    double days = df->val(OFI_DAY);
    for(int i=Sch_D10; i>=Sch_D1; i-=2) {
        if( scheme.val(i)==0) continue;
        if(days<= scheme.val(i)) {
            return total * scheme.val(i+1) / 100;
        }
    }
	return 0;
}

void CPayAllocate::OnInitialPaymentScheme()
{
    long last = df->t->sumrecord;
	
    m_progress.SetRange32(1,last);
    for(long rec=1; rec<=last ;rec++) {
        readrec(rec,df);
        m_progress.SetPos(rec);
        double discount = FindPaymentSchemeDiscount();
        dtodb(discount,df,OFI_DISCOUNT);
        double ibal = df->val(OFI_IBAL) - discount;
        dtodb(ibal,df,OFI_BAL);
        fastwrt(rec,df);
    }
    Refresh();
}

void CPayAllocate::OnActivePaymentScheme() 
{
    long last = df->t->sumrecord;

    m_progress.SetRange32(1,last);
    for(long rec=1; rec<=last ;rec++) {
        readrec(rec,df);
        m_progress.SetPos(rec);
        if(df->val(OFI_PAY) + df->val(OFI_DISCOUNT) < df->val(OFI_IBAL)) {
            dtodb(0,df,OFI_DISCOUNT);
        }
        double ibal = df->val(OFI_IBAL) - df->val(OFI_DISCOUNT) - df->val(OFI_PAY);
        dtodb(ibal,df,OFI_BAL);
        fastwrt(rec,df);
    }
    Refresh();
}

void CPayAllocate::OnOK() 
{
    OnActivePaymentScheme();
    if(MessageBox("Accept It","Offset Invoice",MB_ICONQUESTION | MB_YESNO)==IDNO) {
        CDialog::OnCancel();
        return;
    }
	CDialog::OnOK();
}

void CPayAllocate::OnCompute(long row,long last)
{
    long rec;
	double ibal;

    BalanceAllocated = obal;
    if(BalanceAllocated>0) {
        m_progress.SetRange32(1,last);
        for(rec=1; rec<=row; rec++) {
            readrec(rec,df);
            m_progress.SetPos(rec);
            ibal = df->val(OFI_PAY);
            if(ibal==0) continue;
            BalanceAllocated -= ibal;
        }
        for(rec=row+1; rec<=last; rec++) {
            readrec(rec,df);
            m_progress.SetPos(rec);
            ibal = df->val(OFI_IBAL);
            dtodb(ibal,df,OFI_BAL);
            fastwrt(rec,df);
            if(ibal==0) continue;

            if(BalanceAllocated>=ibal) {
                dtodb(0,df,OFI_BAL);
                dtodb(ibal,df,OFI_PAY);
                BalanceAllocated -= ibal;
            }
            else {
                if(df->val(OFI_IBAL)==df->val(OFI_BAL)) {
                    dtodb(ibal - BalanceAllocated,df,OFI_BAL);
                    dtodb(BalanceAllocated,df,OFI_PAY);
                }
                else {
                    decfld(df,OFI_BAL,BalanceAllocated);
                    incfld(df,OFI_PAY,BalanceAllocated);
                }
                BalanceAllocated = 0;
            }
            fastwrt(rec,df);
            if(BalanceAllocated == 0) break;
        }
    }
}

void CPayAllocate::OnDblClickMsflexgrid1() 
{
    int  row1 = m_grid.GetRow();
    int  col1 = m_grid.GetCol();
    long last = df->t->sumrecord;
    if(row1 <= last) {
        readrec(row1,df);
		backup(df);
        if(col1==6 && bScheme==TRUE) {
            CDiscAmt h1;
            h1.df = df;
            if(h1.DoModal()==IDOK) {
                double ibal = df->val(OFI_IBAL);
                dtodb(h1.oamt,df,OFI_DISCOUNT);
                double bal = ibal - h1.oamt - df->val(OFI_PAY);
                if(bal>=0) {
                    dtodb(bal,df,OFI_BAL);
                }
                else {
                    dtodb(0,df,OFI_BAL);
                }
                fastwrt(row1,df);
                OnAutoAdjust(row1,last);
                Refresh();
            }
        }
        else {
            COffsetAmt h1;
            h1.df = df;
            if(h1.DoModal()==IDOK) {
                double ibal = df->val(OFI_IBAL);
                dtodb(h1.oamt,df,OFI_PAY);
                dtodb(ibal - h1.oamt,df,OFI_BAL);
                fastwrt(row1,df);
                OnCompute(row1,last);
//              BalanceAllocated = atof(df->bak[OFI_PAY]) - h1.oamt;
//              OnAutoAdjust(row1+1,last);
                Refresh();
            }
        }
    }
}
