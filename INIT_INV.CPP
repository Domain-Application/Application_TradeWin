// Init_Inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Arinv.h"
#include "mgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArinv dialog

int IsArinvStable(DB *df)
{
    long xrec = paydat->Search(df->dat[1],PY_INVOICE);
    long yrec = crdat->Search(df->dat[1],CN_INVOICE);

    if(xrec>0 || yrec>0) {
        return 1;
    }
    return 0;
}

void compute_inv_amt(DB *df)
{
    long irec;

    long Srec = interval(&irec, df->dat[1], oinv, 1);
    double total=0;
    for(long rec=0; rec<Srec; rec++) {
        ridx(rec+irec, oinv, 1);
        total += oinv->val(Oiv_AMT) + oinv->val(Oiv_VAT);
    }
    dtodb(total,df,IV_TOTAL);
}

int Arinv_save(DB *df,int flag)
{
    compute_inv_amt(df);
    long xrec = qd(df->dat[IV_TYPE],&tt);
    if(xrec==0) {
        lset(df->dat[IV_TYPE],"IV");
    }
    xrec = qd(df->dat[IV_CUSTOMER],customer);
    if(xrec==0) {
        CString msg;
        msg.Format("Error message\n\nPlease key in %s code",customer->name);
        waitmsg(msg);
        return 0;
    }
    if(!strcmp(df->dat[IV_TYPE],"RF")) {                //REFUND
        xrec = qd(df->dat[IV_BANK],&bank);
        if(xrec==0) {
            AfxMessageBox("Error Message\n\nPlease key in bank Code\ncheque no\ncheque date");
            return 0;
        }
    }
    xrec = qd(df->dat[IV_PREFIX],&prefix);
    if(xrec==0) {
        if(AfxMessageBox("Error Message\n\nPlease key in terms\n\nAssume this Arinv is 'cash terms'",MB_ICONQUESTION | MB_YESNO)==IDNO) {
            return 0;
        }
        lset(df->dat[IV_PREFIX],"CSH");             //CASH TERMS
    }
    int tdays = atoi(df->dat[IV_TERMS]);
    if(prefix.dat[3][0]=='1') {
        if(tdays==0) {
            AfxMessageBox("Error Message\n\nPlease key in 'no of credit days'");
            return 0;
        }
    }
    else {
        dtodb(0,df,IV_TERMS);
    }
    if(!strcmp(df->dat[IV_CURR],profile.dat[PF_CURRENCY])) {
        flag = DO;
        dtodb(1,df,IV_RATE);
    }
    long irec = qd(df->dat[IV_CURR],&currency);
    if(irec==0) {
        lset(df->dat[IV_CURR],profile.dat[PF_CURRENCY]);
        dtodb(1,df,IV_RATE);
    }
    if(df->val(IV_RATE)==0) {
        AfxMessageBox("Error Message\n\nPlease key in 'Currency exchange rate'");
        return 0;
    }
    return 1;
}

int tell_idy (UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case IV_CUSTOMER:
        if(IsArinvStable(df)) {
            flag = DO;
            break;
        }
        if (sq) {
            lset(df->dat[IV_CURR],customer->dat[CUR]);
            qd(df->dat[IV_CURR],&currency);
            rset(df->dat[IV_RATE],currency.dat[CY_RATE]);
//            lset(df->dat[IV_TYPE],customer->dat[CU_TYPE]);
            lset(df->dat[IV_PREFIX],customer->dat[CU_PREFIX]);
            lset(df->dat[IV_TERMS],customer->dat[CU_TERMS]);
            lset(df->dat[IV_STAFF],customer->dat[CU_STAFF]);
            qd(df->dat[IV_PREFIX],&prefix);
            if(prefix.dat[3][0]=='0') {
                lset(df->dat[IV_TYPE],"CB");
            }
            else {
                lset(df->dat[IV_TYPE],"IV");
            }
            pDlg->PostMessage(WM_COMMAND,IDDETAIL);
        }
        break;
    case IV_CURR:
        if (sq) {
            rset(df->dat[IV_RATE],currency.dat[CY_RATE]);
            if(!strcmp(df->dat[IV_CURR],profile.dat[PF_CURRENCY])) {
                flag = DO;
                dtodb(1,df,IV_RATE);
            }
        }
        break;
    case IV_RATE:
        if(!strcmp(df->dat[IV_CURR],profile.dat[PF_CURRENCY])) {
            flag = DO;
            dtodb(1,df,IV_RATE);
        }
        break;
    case IV_TERMS:
        if(prefix.dat[3][0]=='0') flag = DO;
        break; 
    case IV_TOTAL:
        flag = DO;
        break; 
    }
    compute_inv_amt(df);
    return 2;
}

int Receive_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = IV_REASON;
    fld[1] = IV_BANKIN;
    fld[2] = 0;
    return 1;
}

int start_Arinv(DB *df)
{
    dbasedate(df->dat[IV_DATE]);
    lset(df->dat[IV_TYPE],"IV");
    lset(df->dat[IV_REM],"SALES");
    return 1;
}

int start_purchase(DB *df)
{
    dbasedate(df->dat[IV_DATE]);
    lset(df->dat[IV_TYPE],"IV");
    lset(df->dat[IV_REM],"PURCHASE");
    return 1;
}

void ReplaceACINVGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);

    g.SetText(df,NULL,IV_BILLNO,IV_DATE,IV_CUSTOMER,IV_CUSTOMER | 0x101,-1);

    g.SetText(df,IV_CURRENCY,IV_TOTAL,-1);

    double bal = InvoiceBalance(df,df->dat[1]);
    g.SetText(bal,12,2);
}

void SetACINVGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,8);

    g.SetTitle(df,0,IV_BILLNO,IV_DATE,IV_CUSTOMER,-1);
    g.SetTitle("Name",2000);
    g.SetTitle("$",500,7);
    g.SetTitle("Inv. Amt",1500,7);
    g.SetTitle("Inv. Balance",1500,7);
}

int DeleteArinv(long irec,DB *df,int IdxItem)
{
    if(IsArinvStable(df)) {
        CString str;
        str.Format("Error Message\n\nCannot delete Arinv No: %s\nbecause this invoice already make payment.\n",df->dat[1]);
        AfxMessageBox(str);
		return 0;
    }
    else {
        return Delete(irec,df,IdxItem);
    }
}

void init_arinv(DB *df)
{
    df->e->save      = Arinv_save;
    df->e->start     = start_Arinv;
    df->e->rep       = ReplaceACINVGrid;
    df->e->GridTitle = SetACINVGridTitle;
    df->e->initial   = Receive_initial;
    df->BROWSE_ID    = IDD_BROWSE_INV;
    df->e->tell      = tell_idy;
//    df->e->prec      = unprint;
    df->e->Delete    = DeleteArinv;
}

void init_purchase(DB *df)
{
    df->e->save      = Arinv_save;
    df->e->start     = start_purchase;
    df->e->rep       = ReplaceACINVGrid;
    df->e->initial   = Receive_initial;
    df->e->GridTitle = SetACINVGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_INV;
    df->e->tell      = tell_idy;
//    df->e->prec      = unprint;
    df->e->Delete    = DeleteArinv;
}

void SalesInvoice()
{
    OpenARData();

    UINT IDD = IDD_ARINV;
    CArinv p1(IDD,inv,1);
    p1.m_customer = "Debtor";
    p1.m_salesman = "Salesman";
    p1.m_invno    = "Customer P/O No";
    p1.period = &period_acc;
    p1.fldPeriod      = IV_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    p1.sub        = oinv;
    p1.IDD_SUB    = IDD_OINV;
    p1.LastField  = IV_REASON;
    p1.DoModal();
}

void PurchaseInvoice()
{
    OpenAPData();

    UINT IDD = IDD_ARINV;
    CArinv p1(IDD,inv,1);
    p1.m_customer = "Creditor";
    p1.m_salesman = "Received";
    p1.m_invno    = "Supplier Inv. No";
    p1.period = &period_acc;
    p1.fldPeriod      = IV_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    p1.sub     = oinv;
    p1.IDD_SUB = IDD_OINV;
    p1.LastField = IV_REASON;
    p1.DoModal();
}
