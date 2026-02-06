#include "stdafx.h"
#include "menu.h"
#include "two.h"
#include "wac.h"
#include "ipay.h"
#include "blist.h"
#include "state.h"
#include "arinv.h"

extern DB blist;
DB gl_bank;
int tell_WacBankr(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case I_BANKIN:
        if(isblank(df->dat[fld])) {
            dbasedate(df->dat[fld]);
        }
        break;
    default:
        flag = DO;
        break;
    }
    return 2;
}

int initial_bankwac(DB *df,int *fld)                //initial setting
{
    int j = 0;
    for (uint i=1; i<= df->sumfield; i++) {
        if(i==I_BANKIN) continue;
        fld[j++] = i;
    }
	return 1;
}

void init_WacBank(DB *df)
{
    df->e->tell  = tell_WacBankr;
    df->e->initial = initial_bankwac;
}

int tell_cash(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case IV_BANKIN:
        if(isblank(df->dat[fld])) {
            dbasedate(df->dat[fld]);
        }
        break;
    default:
        flag = DO;
        break;
    }
    return 2;
}

void init_cash(DB *df)
{
    df->e->tell = tell_cash;
}

int tell_ipay(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case IP_BANKIN:
        if(isblank(df->dat[fld])) {
            dbasedate(df->dat[fld]);
        }
        break;
    default:
        flag = DO;
        break;
    }
    return 2;
}


int wac_bankr_initial(DB *df,int *fld)                //initial setting
{
    int j=0;
    for(uint i=1; i<=df->sumfield; i++) {
        if(i==I_BANKIN) continue;
        fld[j++] = i;
    }
	return 1;
}

int WacModule(DB *df,DB *wacitem,int id)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return 0;
    }
    CWac p1(id,df,df->f[1].idx & 0x01);
    p1.sub     = wacitem;
    p1.IDD_SUB = IDD_WACITEM;
    init_WacBank(df);
    wac.e->initial = wac_bankr_initial;
    p1.OneRecordOnly = TRUE;
    p1.grid_ratio = float(1.00);
    p1.child_grid_ratio = float(0.67);
    return p1.DoModal();
}

void ViewWacBankr(DB *wac,DB *wacitem,int type)
{
    long irec = wac->Search(blist.dat[Bl_REFNO],1,TRUE);
    if(irec>0) {
        lset(wac->dat[I_BANKIN],wac->dat[I_DATE]);
        int flag = WacModule(wac,wacitem,IDD_WAC);
        init_wac(wac);
        if ( flag == IDSAVE ){
            wac->Update();
            lset(blist.dat[Bl_BANKIN],wac->dat[I_BANKIN]);
            fastwrt(blist.rec,&blist);
        }
    }
}

int cash_bankr_initial(DB *df,int *fld)                //initial setting
{
    uint j=0;
    for(uint i=1; i<=df->sumfield; i++) {
        if(i==IV_BANKIN) continue;
        fld[j++] = i;
    }
	return 1;
}

void ViewInvoice()
{
    long irec = inv->Search(blist.dat[Bl_REFNO],1,TRUE);
    if(irec>0) {
       inv->rec = qd(blist.dat[Bl_REFNO],inv);
       lset(inv->dat[IV_BANKIN],inv->dat[IV_CQ_DATE]);
       CArinv p1(IDD_INVOICE,inv,1);
       p1.m_customer = AcStr[ProgramMenuNo-1];
       p1.m_salesman = "SALESMAN";
       p1.IDD_SUB = IDD_OINV;
       init_cash(inv);
       inv->e->initial = cash_bankr_initial;
       p1.OneRecordOnly = TRUE;
       int flag = p1.DoModal();
       if ( flag == IDSAVE ){
           inv->Update();
           lset(blist.dat[Bl_BANKIN],inv->dat[IV_BANKIN]);
           fastwrt(blist.rec,&blist);
       }
   }
}

int ipay_bankr_initial(DB *df,int *fld)                //initial setting
{
    int j=0;
    for(uint i=1; i<=df->sumfield; i++) {
        if(i==IP_BANKIN) continue;
        fld[j++] = i;
    }
    fld[j++] = 0;
	return 1;
}

void init_bankr_ipay(DB *df)
{
    df->e->initial = ipay_bankr_initial;
    df->e->chain   = paydat;
}

void ViewBankr()
{
    if(!strcmp(blist.dat[Bl_PATH],"AR")) {
        OpenARData();
    }
    else if(!strcmp(blist.dat[Bl_PATH],"AP")) {
        OpenAPData();
    }
    init_bankr_ipay(ipay);

    long irec = ipay->Search(blist.dat[Bl_REFNO],1,TRUE);
    if(irec>0) {
        lset(ipay->dat[IP_BANKIN],ipay->dat[IP_CQ_DATE]);
        CIpay p1(IDD_IPAY,ipay,1);
        ipay->e->initial  = ipay_bankr_initial;
        p1.OneRecordOnly = TRUE;
        p1.sub           = paydat;
        p1.IDD_SUB       = IDD_PAYDAT;
        p1.m_customer    = "Debtor";
        p1.m_AutoDetail  = FALSE;
        int flag = p1.DoModal();
        init_payment(ipay);
        if (flag == IDSAVE){
            ipay->Update();
            lset(blist.dat[Bl_BANKIN],ipay->dat[IP_BANKIN]);
            fastwrt(blist.rec,&blist);
        }
    }
    else {
        ViewInvoice();
    }
}

long viewidx_blist(DB *df,UINT ID)
{
    UINT IDD=ID;

    if(IDD==0) IDD=df->BROWSE_ID;
    CBlist b1(IDD);
    b1.df       = df;
    b1.IdxItem  = df->f[1].idx & 0x01;
    b1.EditFlag = 0;

    b1.DoModal();
    df->rec = b1.no;
    ridx(df->rec,df,b1.IdxItem);
    return df->rec;
}

void ViewEditBankReconciliation()
{
    int field[] = { Bl_CHEQUE,Bl_DATE,Bl_REFNO,0 };

    sort(&blist, field);
    blist.f[Bl_DATE].idx = 0;
    for(;;) {
        blist.rec = viewidx_blist(&blist,IDD_BLIST);
        if(blist.rec==0) break;
        if(!strcmp(blist.dat[Bl_PATH],"GL")) {
            OpenGLData();
            if(blist.dat[Bl_OB][0]=='Y') {
                ViewWacBankr(&bcwac,&bcitem,0);
            }
            else {
                ViewWacBankr(&wac,&wacitem,1);
            }
        }
        else {
            ViewBankr();
        }
    }
    OpenGLData();
}
