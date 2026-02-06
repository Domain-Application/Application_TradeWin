// init_pay.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include <math.h>
#include "mgrid.h"
#include "db_ado.h"
#include "Ipay.h"
#include "ChequeNo.h"
#include "ChequeReturnRegister.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ComputeBankCommission(DB *df)
{
    long xrec = qd(df->dat[IP_PAYTYPE],&paytype);
    if(xrec==0) return;
    int  fld     = Pty_COMM;
    int  min_fld = Pty_ARMINBC;
    if( STCR != St_CR){
        fld = Pty_APCOMM;
        min_fld = Pty_APMINBC;
    }
    double comm   = df->val(IP_TOTAL) * paytype.val(fld) / 100;
    double min_bc = paytype.val(min_fld);
    if(comm<min_bc) comm = min_bc;
    if(df->val(IP_BC) == 0) {
        dtodb(comm,df,IP_BC);
    }
}

int tell_Payment(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	/*  do return cheque at payment entry
    if(!isblank(df->dat[IP_RETURN])) {
        CString msg;
        char d1[20],d2[20],d3[20];
        strcpy(d1,mdate(df->dat[IP_DATE]));
        strcpy(d2,mdate(df->dat[IP_CQ_DATE]));
        strcpy(d3,mdate(df->dat[IP_RETURN]));
        msg.Format("Can not modify, Reason\n\nVoucher No:%s  Voucher Date:%s\nCheque No:%s  Cheque Date:%s\n\nThis cheque already return (%s)",
        df->dat[1],d1,df->dat[IP_CHEQUE],d2,d3);
        AfxMessageBox(msg,MB_ICONHAND);
        flag = DO;
        return ESC;
    }
	*/
    double total=0;
    int i;

    switch(fld) {
    case IP_CUSTOMER:
        if(dseek(df->dat[1],&arpaydat,1,MF)>0){
            flag = DO;
            break;
        }
        if (sq){
            //lset(df->dat[IP_CURR],profile.dat[PF_CURRENCY]);
			lset(df->dat[IP_CURR],customer->dat[CU_CURRENCY]);
            qd(df->dat[IP_CURR],&currency);
            rset(df->dat[IP_RATE],currency.dat[CY_RATE]);
            lset(df->dat[IP_STAFF],customer->dat[CU_STAFF]);
            return 2;
        }
        break;
    case IP_PAYTYPE:
        if(sq) {
            if(paytype.dat[1][0]=='L' || paytype.dat[1][0]=='O') {         //Bank Cheque
                dbasedate(df->dat[IP_CQ_DATE]);
                lset(df->dat[IP_GLCODE],profile.dat[PF_BANK]);
            }
            else if(paytype.dat[1][0]=='C') {                  //cash
                lset(df->dat[IP_CHEQUE],paytype.dat[Pty_NAME]);
                lset(df->dat[IP_CQ_DATE],df->dat[IP_DATE]);
                lset(df->dat[IP_GLCODE],profile.dat[PF_PC]);
            }
        }
        break;
    case IP_CHEQUE:
        if(isblank(df->dat[fld])) break;
        if((df->dat[IP_PAYTYPE][0] == 'C')){
            flag = DO;
            break;
        }
        if (sq){
            i = CheckIssuedChequeNo(fld,flag,df);
        }
        break;
    case IP_TOTAL:
        if (sq){
            pDlg->PostMessage(WM_COMMAND,IDDETAIL);
        }
        break;
    case IP_CQ_DATE:
        if(isblank(df->dat[fld])) break;
        if(!(df->dat[IP_PAYTYPE][0] == 'L' || df->dat[IP_PAYTYPE][0] == 'O' || df->dat[IP_PAYTYPE][0] == ' ')){
            flag = DO;
            break;
        }
        break;
    case IP_BC:
        ComputeBankCommission(df);
        if(df->dat[IP_PAYTYPE][0] == 'C') flag = DO;
        break;
    case IP_CURR:
        flag = DO;
        break;
    case IP_RATE:
        if(!strcmp(df->dat[IP_CURR],profile.dat[PF_CURRENCY])) {
            flag = DO;
            dtodb(1,df,fld);
        }
        break;
    case IP_CGL:
    case IP_VATNO:
        flag = DO;
        break;
    }
    return 1;
}

int payment_save(DB *df,int flag)
{
    long xrec = glcode.Search(df->dat[IP_GLCODE]);

    if(xrec==0){
        waitmsg("Please keyin 'Bank A/c Code'");
        return 0;
    }
    xrec = customer->Search(df->dat[IP_CUSTOMER]);
    if(xrec==0){
        waitmsg("Please keyin 'A/c Code'");
        return 0;
    }
    xrec = qd(df->dat[IP_CURR],&currency);
    if(xrec==0){
        lset(df->dat[IP_CURR],customer->dat[CUR]);
        qd(df->dat[IP_CURR],&currency);
        rset(df->dat[IP_RATE],currency.dat[CY_RATE]);
    }
    if(df->val(IP_RATE)==0) {
        dtodb(1,df,IP_RATE);
    }
    xrec = qd(df->dat[IP_PAYTYPE],&paytype);
    if(xrec>0) {
        if(paytype.dat[1][0]=='L' || paytype.dat[1][0]=='O') {                        //Bank Cheque
            if(isblank(df->dat[IP_CQ_DATE])) {
                dbasedate(df->dat[IP_CQ_DATE]);
            }
            if(isblank(df->dat[IP_CHEQUE])) {
                lset(one.dat[1],df->dat[IP_CQ_DATE]);
                CChequeNo h1;
                int flag = h1.DoModal();
                if(flag==IDOK) {
                    lset(df->dat[IP_CHEQUE],h1.m_cheque);
                    lset(df->dat[IP_CQ_DATE],one.dat[1]);
                }
                else {
                    if(AfxMessageBox("Error Message\n\nPlease keyin 'Cheque No'\nSkip It",MB_ICONQUESTION | MB_YESNO)==IDNO) {
                        return 0;
                    }
                }
            }
        }
/*        else if(paytype.dat[1][0]=='C') {                  //cash
            if(!isblank(df->dat[IP_CHEQUE])) {
                if(AfxMessageBox("Are you sure is pay by cash",MB_ICONQUESTION | MB_YESNO)==IDYES) {
                    lset(df->dat[IP_CHEQUE],"CASH");
                    lset(df->dat[IP_CQ_DATE],"");
                }
            }
        }*/
    }
    return 1;

    double total = df->val(IP_TOTAL);
    CString str;
    if(STCR == St_CR) {
        str.Format("select sum(total) from [arpaydat] where [refno] = %s",
            DbText(df,1));
    }
    else {
        str.Format("select sum(total) from [appaydat] where [refno] = %s",
            DbText(df,1));
    }
    ItemAmt = SqlValue(str);

    if(fabs(total-ItemAmt)<0.001) return 1;
    if(total>ItemAmt){
        if ( choice_menu("Over Pay",FAIL) == OK ) return 1;
    }
    else {
        if ( choice_menu("Under Pay",FAIL) == OK ) return 1;
    }
    return 0;
}

void ReplaceIPAYGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,0,IP_BILLNO,IP_DATE,IP_CUSTOMER,IP_CUSTOMER | 0x102,-1);
    g.SetText(df,IP_CHEQUE,IP_GLCODE,IP_CURRENCY,IP_TOTAL,-1);
}

void SetIPAYGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,9);

    g.SetTitle("No",500,0);
    g.SetTitle(df,IP_BILL,IP_DATE,IP_CUSTOMER,-1);
    g.SetTitle("Name",2000);
    g.SetTitle("Cheque No",1000);
    g.SetTitle("Bank A/C",800);
    g.SetTitle("$",500);
    g.SetTitle("Paid Amt.",1000,7);
}

int start_payment(DB *df)
{
    dbasedate(df->dat[IP_DATE]);
    lset(df->dat[IP_PAYTYPE],"C");
    lset(df->dat[IP_REMARKS],"PAYMENT");
    lset(df->dat[IP_GLCODE],profile.dat[PF_BANK]);
    return 1;
}

int payment_initial(DB *df,int *fld)                //initial setting
{	
	return 1;
    /*
	fld[0] = IP_RETURN;
    fld[1] = 0;
	*/

}

void init_payment(DB *df)
{
    df->e->start     = start_payment;
    df->e->save      = payment_save;
    df->e->tell      = tell_Payment;
//    df->e->prec      = Preceipt;
    df->e->initial   = payment_initial;
    df->e->chain     = &arpaydat;
    df->e->rep       = ReplaceIPAYGrid;
    df->e->GridTitle = SetIPAYGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_INV;
}

int start_receipt(DB *df)
{
    dbasedate(df->dat[IP_DATE]);
    lset(df->dat[IP_PAYTYPE],"C");
    lset(df->dat[IP_REMARKS],"PAYMENT RECEIPT");
    lset(df->dat[IP_GLCODE],profile.dat[PF_BANK]);
    return 1;
}

void init_ipay(DB *df)
{
    df->e->start     = start_receipt;
    df->e->save      = payment_save;
    df->e->tell      = tell_Payment;
//    df->e->prec      = Ppay;
    df->e->initial   = payment_initial;
    df->e->chain     = &arpaydat;
    df->e->rep       = ReplaceIPAYGrid;
    df->e->GridTitle = SetIPAYGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_INV;
    df->t->set = 2;     //self auto
}

void Payment_Entry()
{
    OpenARData();
    dbopen(&receipt,"receipt.rep",init_receipt);
    CIpay p1(IDD_IPAY,&aripay,1);
    p1.m_customer = "Debtor";
    p1.m_salesman = "Collected";
    p1.sub     = &arpaydat;
    p1.IDD_SUB = IDD_PAYDAT;
    p1.period = &period_acc;
    p1.fldPeriod       = IP_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    if(profile.dat[PF_PREVIEW][0]=='Y') {
        p1.LptNo = 0;
    }
    p1.DoModal();
    dclose(&receipt);
}

void Receipt_Entry()
{
    OpenAPData();
    dbopen(&receipt,"receipt.rep",init_receipt);
    CIpay p1(IDD_IPAY,&apipay,1);
    p1.m_customer = "Creditor";
    p1.m_salesman = "Issued";
    p1.sub     = &appaydat;
    p1.IDD_SUB = IDD_PAYDAT;
    p1.period = &period_acc;
    p1.fldPeriod       = IP_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    if(profile.dat[PF_PREVIEW][0]=='Y') {
        p1.LptNo = 0;
    }
    p1.DoModal();
    dclose(&receipt);
}

void ViewAR_ChequeReturn()
{
	OpenARData();
    DB main,sub;

    tado(&main,"arrc",init_ipay);
    tado(&sub,"arrcdat");

    CIpay p1(IDD_IPAY,&main,1);
    p1.m_customer = "Debtor";
    p1.m_salesman = "Collected";
    p1.sub        = &sub;
    p1.IDD_SUB    = IDD_PAYDAT;
    p1.Status     = 2;           //view only
    p1.DoModal();

    dclose(&main);
    dclose(&sub);
}

void ViewAP_ChequeReturn()
{
	OpenAPData();
    DB main,sub;

    tado(&main,"aprc",init_ipay);
    tado(&sub,"aprcdat");

    CIpay p1(IDD_IPAY,&main,1);
    p1.m_customer = "Creditor";
    p1.m_salesman = "Issued";
    p1.sub        = &sub;
    p1.IDD_SUB    = IDD_PAYDAT;
    p1.Status     = 2;           //view only
    p1.DoModal();

    dclose(&main);
    dclose(&sub);
}

void AP_ChequeReturn()
{
    OpenAPData();
    CChequeReturnRegister h1;

    h1.df = &apipay;
    h1.sub = &appaydat;
    h1.DoModal();
}

void AR_ChequeReturn()
{
    OpenARData();
    CChequeReturnRegister h1;

    h1.df = &aripay;
    h1.sub = &appaydat;
    h1.DoModal();
}
