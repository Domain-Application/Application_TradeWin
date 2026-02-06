// Tap.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTap dialog

enum tap {Tap_PAYTYPE=1,Tap_CHEQUE,Tap_CQ_DATE,Tap_GLCODE,Tap_AMOUNT,Tap_BC};

CTap::CTap(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD, pdf, Idx, p, pParent)
{
	//{{AFX_DATA_INIT(CTap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTap::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTap)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CTap::OnSetWild()
{
    lsetblank(df);
    dtodb(balance,df,Tap_AMOUNT);
    lset(df->dat[Tap_GLCODE],profile.dat[PF_BANK]);
    lset(df->dat[Tap_PAYTYPE],"B");
    dbasedate(df->dat[Tap_CQ_DATE]);
}

void CTap::OnSaveOR()
{
    waitflock(ipay);
    waitflock(paydat);
    long mrec = getdel(ipay);
    fetch(ipay,df);
    lset(ipay->dat[IP_DATE],inv->dat[IV_DATE]);
    lset(ipay->dat[IP_CURRENCY],inv->dat[IV_CURRENCY]);
    lset(ipay->dat[IP_STAFF],inv->dat[IV_STAFF]);
    rset(ipay->dat[IP_RATE],inv->dat[IV_RATE]);
    lset(ipay->dat[IP_CUSTOMER],inv->dat[IV_CUSTOMER]);
    lset(ipay->dat[1],"OR");
    ipay->CodeNoDefault(ipay,1);
    AppendIdx(mrec,ipay,1);

    mrec = getdel(paydat);
    lset(paydat->dat[1],ipay->dat[1]);
    lset(paydat->dat[PY_INVOICE],inv->dat[1]);
    lset(paydat->dat[PY_DATE],inv->dat[IV_DATE]);
    dtodb(balance,paydat,PY_TOTAL);
    AppendIdx(mrec,paydat,1);
    unflock(ipay);
    unflock(paydat);

    if(MessageBox("Print receipt","Cash bill",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)==IDYES) {
        FILE *ptr = fopen("lpt1","w");
        setvbuf(ptr,NULL,_IONBF,0);
        if(ProgramMenuNo == 1) {
            Ppay(0,ptr,ipay);
        }
        else {
            Preceipt(0,ptr,ipay);
        }
        fclose(ptr);
    }
}

void CTap::OnPrint()
{
    ReplaceRecord();
    if(atof(df->dat[Tap_AMOUNT])==balance) {
        OnSaveOR();
        OnOK();
    }
    else {
        MessageBox("amount no tally");
    }
}

BEGIN_MESSAGE_MAP(CTap, CTio)
	//{{AFX_MSG_MAP(CTap)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTap message handlers

int tell_tap(UINT nChar,int &flag,int &fld,DB *df,int sq)
{

    switch(fld) {
    case Tap_PAYTYPE:
        if(sq){
            if(df->dat[Tap_PAYTYPE][0]=='B') {
                lset(df->dat[Tap_GLCODE],profile.dat[PF_BANK]);
                lset(df->dat[Tap_PAYTYPE],"B");
                dbasedate(df->dat[Tap_CQ_DATE]);
            }
            else {
                lset(df->dat[Tap_GLCODE],profile.dat[PF_PC]);
                lset(df->dat[Tap_CQ_DATE],"");
            }
        }
        break;
    case Tap_CHEQUE:
    case Tap_CQ_DATE:
        if(df->dat[Tap_PAYTYPE][0] != 'B') flag = DO;
        break;
    case Tap_BC:
        if(df->dat[Tap_PAYTYPE][0] == 'C') flag = DO;
        break;
    case Tap_AMOUNT:
        flag = DO;
        break;
    }
    return 2;
}

void init_tap(DB *df)
{
    df->e->tell = tell_tap;
}

void AutoPayment(double bal_total)
{
    DB tap;

    rbopen(&tap,"tap.scr",init_tap);
    CTap t1(IDD_TAP, &tap);
    t1.Status = 1;
    t1.balance = bal_total;
    t1.DoModal();
    dclose(&tap);
}
