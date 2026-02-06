// init_Cr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Cr.h"
#include "AdjOffset.h"
#include "mgrid.h"

int tell_cr(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    double total=0;

    switch(fld) {
    case Cr_CUSTOMER:
        if(dseek(df->dat[1],crdat,1,MF)>0){
             flag = DO;
             break;
        }
        if (sq){
            lset(df->dat[Cr_CURR],customer->dat[CUR]);
            lset(df->dat[Cr_STAFF],customer->dat[STAFF]);
            qd(df->dat[Cr_CURR],&currency);
            rset(df->dat[Cr_RATE],currency.dat[CY_RATE]);
        }
        return 2;
        break;
    case Cr_RATE:
        if(!strcmp(df->dat[Cr_CURR],profile.dat[PF_CURRENCY])) {
            flag = DO;
            dtodb(1,df,fld);
        }
        break;
    case Cr_CURRENCY:
    case Cr_CGL:
    case Cr_TOTAL:
        flag = DO;
        break;
    }
    return 1;
}

int cr_save(DB *df,int flag)
{
    long xrec = dseek(df->dat[Cr_CUSTOMER],customer,1,MF);
    if(xrec==0){
        waitmsg("Please keyin 'A/c Code'");
        return 0;
    }
    long irec = qd(df->dat[Cr_CURR],&currency);
    if(irec==0) {
        lset(df->dat[Cr_CURR],profile.dat[PF_CURRENCY]);
        dtodb(1,df,Cr_RATE);
    }
    if(df->val(Cr_RATE)==0) {
        dtodb(1,df,Cr_RATE);
    }
    if(fabs(ItemAmt - cr_amt)<0.01) return 1;

    if(ItemAmt > cr_amt) {
        GldatChildControl(IDD_GLDAT,df,gldat,1);
    }
    return 1;
}

void ReplaceCRGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,0,1,Cr_DATE,Cr_CUSTOMER,Cr_CUSTOMER | 0x101,-1);
    g.SetText(df,Cr_CURRENCY,Cr_TOTAL,-1);
}

void SetCRGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,7);

    g.SetTitle(df,0,Cr_BILL,Cr_DATE,Cr_CUSTOMER,-1);
    g.SetTitle("Name",3000);
    g.SetTitle("$",500);
    g.SetTitle("Amount",1000,7);
}

int DeleteCR(long irec,DB *df,int IdxItem)
{
    return Delete(irec,df,IdxItem);
    /* 
    long crec = dseek(df->dat[1],crdat,1,MF);
    if(crec) {
        CString str;
        str.Format("Error Message\n\nCannot delete cr/dr No: %s\nbecause this cr/dr already offset invoice.\n",df->dat[1]);
        AfxMessageBox(str);
    }
    else {
        Delete(irec,df,IdxItem);
    }
	*/
}

int start_cr(DB *df)
{
    dbasedate(df->dat[Cr_DATE]);
    lset(df->dat[Cr_CURRENCY],profile.dat[PF_CURRENCY]);
    dtodb(1,cr,Cr_RATE);
    if(STCR == St_CR) {
        lset(df->dat[Cr_REMARKS],"CREDIT NOTE");
    }
    else {
        lset(df->dat[Cr_REMARKS],"DEBIT NOTE");
    }
    return 1;
}
 
void init_cr(DB *df)
{
    df->e->start = start_cr;
    df->e->save  = cr_save;
    df->e->tell  = tell_cr;
//    df->e->prec2  = PCreditNote;
//    df->e->prec  = PCreditNote;
    df->e->chain = crdat;
    df->e->rep       = ReplaceCRGrid;
    df->e->GridTitle = SetCRGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_INV;
    df->e->Delete    = DeleteCR;
    df->t->set = 2;       //self auto
}
 
void init_apcr(DB *df)
{
    df->e->start = start_cr;
    df->e->save  = cr_save;
    df->e->tell  = tell_cr;
//    df->e->prec2  = PDebitNote;
//    df->e->prec  = PDebitNote;
    df->e->chain = crdat;
    df->e->rep       = ReplaceCRGrid;
    df->e->GridTitle = SetCRGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_INV;
    df->e->Delete    = DeleteCR;
    df->t->set = 2;       //self auto
}

void CreditNote()
{
    OpenARData();
    dbopen(&receipt,"receipt.rep",init_receipt);
    CCr p1(IDD_CR,cr,1);
    p1.sub     = crdat;
    p1.sub1    = gldat;
    p1.IDD_SUB = IDD_CRDAT;
    p1.m_customer = "Debtor";
    p1.m_salesman = "Salesman";
    p1.period = &period_acc;
    p1.fldPeriod       = Cr_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    if(profile.dat[PF_PREVIEW][0]=='Y') {
        p1.LptNo = 0;
    }
    p1.DoModal();
    dclose(&receipt);
}

void SupplierCreditNote()
{
    OpenAPData();
    dbopen(&receipt,"receipt.rep",init_receipt);
    CCr p1(IDD_CR,cr,1);
    p1.sub     = crdat;
    p1.sub1    = gldat;
    p1.IDD_SUB = IDD_CRDAT;
    p1.m_customer = "Creditor";
    p1.m_salesman = "Received";
    p1.period = &period_acc;
    p1.fldPeriod       = Cr_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    if(profile.dat[PF_PREVIEW][0]=='Y') {
        p1.LptNo = 0;
    }
//    p1.m_PrintOneFormOnly = TRUE;
    p1.DoModal();
    dclose(&receipt);
}
