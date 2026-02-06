// Wac.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wac.h"
#include "Math.h"
#include "Reminder.h"
#include "mgrid.h"
#include "ChequeNo.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DB jb,jbdat,imon,ciq,imondat;
void compute_wacitem_amt(DB *df)
{
    double total=0;

    long rec = dseek(df->dat[Wi_GLCODE],&glcode,1,MF);
    if(rec==0) {
        dtodb(0,df,Wi_AMT);
        dtodb(0,df,Wi_T);
        dtodb(0,df,Wi_GST);
    }
    qd(df->dat[Wi_T],&Gst_type);
    double rate = Gst_type.val(3);
    double Tax  = df->val(Wi_AMT) * rate / 100;
    dtodb(Tax,df,Wi_GST);
}

int tell_wacitem(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    if(fld != Wi_GLCODE) {
        if(isblank(df->dat[Wi_GLCODE])) {
            flag = DO;
            return 1;
        }
    }
    switch(fld) {
    case Wi_GLCODE:
        if(sq){
            lset(df->dat[Wi_NAME],glcode.dat[2]);
            lset(df->dat[Wi_DC],"D");
            if(dr_amt > cr_amt) {
                lset(df->dat[Wi_DC],"C");
                dtodb(dr_amt - cr_amt, df, Wi_AMT);
            }
            else if(dr_amt < cr_amt) {
                dtodb(cr_amt - dr_amt, df, Wi_AMT);
            }
        }
        break;
    case Wi_T:
    case Wi_GST:
        if(!(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T')){
            flag = DO;
        }
        break;
    }
    compute_wacitem_amt(df);
    return 2;
}

int save_wacitem(DB *df,int flag)
{
    long xrec = qd(df->dat[Wi_GLCODE],&glcode);
    if(xrec==0) {
        waitmsg("no g/l code");
        return 0;
    }
    if( df->val(Wi_AMT)<=0) {
        waitmsg("Please key in Amount");
        return 0;
    }
    if(isblank(df->dat[Wi_DC])) {
        waitmsg("Please assign Debit/Credit");
        return 0;
    }
    return 1;
}

int start_wacitem(DB *df)
{
    if(dr_amt > cr_amt) {
        lset(df->dat[Wi_DC],"C");
    }
    else if(dr_amt < cr_amt) {
        lset(df->dat[Wi_DC],"D");
    }
    return 1;
}

int WacItemShowInformation(class CDialog *p,DB *sub,int flag)
{
    CString str;
    if(sub->dat[Wi_DC][0]=='D') {
        str = "Debit Amount";
    }
    else if(sub->dat[Wi_DC][0]=='C') {
        str = "Credit Amount";
    }
    else {
        str = "G/L Amount";
    }
    p->SetDlgItemText(IDC_STATIC4,str);
	return 0;
}

void init_wacitem(DB *df)
{
    df->e->tell = tell_wacitem;
    df->e->save = save_wacitem;                     // part out
    df->e->start = start_wacitem;
    df->e->ShowInformation = WacItemShowInformation;
}

void SetEasyModeData(DB *df)
{
    long irec = qd(df->dat[I_EMODE],&emode);
    if(irec==0) return;

    long Srec = interval(&irec, df->dat[1], &wacitem, 1);
    if(Srec==0) {
        pDlg->PostMessage(WM_COMMAND,IDDETAIL);
    }
}

int tell_wac(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case I_EMODE:
        if(sq) {
            SetEasyModeData(df);
        }
        break;
    case I_CHEQUE:
        if (sq){
            CheckIssuedChequeNo(fld,flag,df);
        }
        break;
    }
    return 1;
}

BOOL IsGlcodeHaveBank(DB *df)
{
    long irec;
    long Srec = interval(&irec,df->dat[1],df->e->chain,1);
    for(long i=0;i<Srec;i++) {
        ridx(irec+i,df->e->chain,1);
        long xrec = qd(df->e->chain->dat[Wi_GLCODE],&bank);
        if(xrec) return TRUE;
    }
    return FALSE;
}

char *not_bal_msg = "Do not tally";
int wac_save(DB *df,int flag)
{
    if(fabs(dr_amt-cr_amt)>0.001){
        waitmsg(not_bal_msg);
        return 0;
    }
    if(!isblank(df->dat[I_CHEQUE])) {
        if(isblank(df->dat[I_CQ_DATE])) {
            dbasedate(df->dat[I_CQ_DATE]);
        }
    }
    else {
        if(IsGlcodeHaveBank(df)==FALSE) return 1;
        if(isblank(df->dat[I_CQ_DATE])) {
            dbasedate(df->dat[I_CQ_DATE]);
        }
        lset(one.dat[1],df->dat[I_CQ_DATE]);
        CChequeNo h1;
        int flag = h1.DoModal();
        if(flag==IDOK) {
            lset(df->dat[I_CHEQUE],h1.m_cheque);
            lset(df->dat[I_CQ_DATE],one.dat[1]);
        }
        else {
            if(AfxMessageBox("Error Message\n\nPlease keyin 'Cheque No'\nSkip It",MB_ICONQUESTION | MB_YESNO)==IDNO) {
                return 0;
            }
        }
    }
    return 1;
}

int initial_wac(DB *df,int *fld)                //initial setting
{
    fld[0] = I_BANKIN;
    fld[1] = 0;
	return 1;
}

int start_wac(DB *df)                //initial setting
{
    dbasedate(df->dat[I_DATE]);
    lset(df->dat[I_POSTING],"T");
	return 1;
}

void ReplaceWACGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);

    g.SetText(df,0,1,I_DATE,I_PARTICULAR,I_CHEQUE,I_CQ_DATE,-1);
    CString str;
    str.Format("select sum(amt) from wacitem where [refno] = %s and [dc] = 'D'",
       DbText(df,1));
    double total = SqlValue(str);
    g.SetText(total, 12, 2);
}

void SetWACGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,7);
    g.SetTitle(df,0,1,I_DATE);
    g.SetTitle("Particular",3200);
    g.SetTitle("Cheque No",1300);
    g.SetTitle("Cheque Date",1200);
    g.SetTitle("Amount",1000,7);
}

int wac_quarry(DB *df)
{
    dbasedate(df->dat[I_DATE]);
    return 1;
}

void init_wac(DB *df)
{
    df->e->initial   = initial_wac;
    df->e->tell      = tell_wac;
    df->e->save      = wac_save;
    df->e->rep       = ReplaceWACGrid;
    df->e->GridTitle = SetWACGridTitle;
    df->e->start     = start_wac;
//    df->e->prec      = PrintVoucher;
    df->BROWSE_ID    = IDD_BROWSE_INV;
}

BOOL IsReminderOn()
{
    long last = emode.t->sumrecord;

    for(long rec=1; rec<=last; rec++) {
        readrec(rec, &emode);
        if(emode.dat[Em_REMINDER][0]=='Y' || emode.dat[Em_REMINDER][0]=='T'){
            if(!strcmp(emode.dat[Em_DATE],today)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void Reminder()
{
    long last = emode.t->sumrecord;

    ropen(&ciq,"reminder.scr");
    for(long rec=1; rec<=last; rec++) {
        readrec(rec, &emode);
        if(emode.dat[Em_REMINDER][0]=='Y' || emode.dat[Em_REMINDER][0]=='T'){
            if(!strcmp(emode.dat[Em_DATE],today)) {
                fetch(&ciq,&emode);
                CReminder p1;
                p1.df = &ciq;
                p1.DoModal();
            }
        }
    }
    dclose(&ciq);
}

void DoWAC(DB *wac,DB *wacitem,int id1,int id2)
{
    if(emode.t->sumrecord>0) {
        if(IsReminderOn()==TRUE) {
            if(AfxMessageBox("Use reminder wizard",MB_ICONQUESTION | MB_YESNO)==IDYES) {
                Reminder();
                return;
            }
        }
    }
    CWac p1(id1,wac,1);
    wac->e->chain = wacitem;
    p1.sub     = wacitem;
    p1.IDD_SUB = id2;
    p1.period = &period_acc;
    p1.fldPeriod       = I_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    p1.grid_ratio = float(1.00);
    p1.child_grid_ratio = float(0.67);
    if(profile.dat[PF_PREVIEW][0]=='Y') {
        p1.LptNo = 0;
    }

    p1.DoModal();
}

void BankReconciliationOpeningData()
{
    tado(&bcwac,"bcwac",init_wac);
    tado(&bcitem,"bcitem",init_wacitem);
    ASSERT(bcwac.t->reclen  == wac.t->reclen);
    ASSERT(bcitem.t->reclen == wacitem.t->reclen);

    DoWAC(&bcwac,&bcitem,IDD_WAC,IDD_WACITEM);

    dclose(&bcwac);
    dclose(&bcitem);
}
