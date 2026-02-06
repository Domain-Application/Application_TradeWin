#include "stdafx.h"
#include "menu.h"
#include "Tso.h"
#include "ShowProgress.h"

/**
    Full Detail
**/
void PrintAuditorStatement(DB *df)
{
    double amount = df->val(IV_TOTAL);
    if(tso.dat[Ts_CONVERT][0]=='Y') {
		double rate = df->val(IV_RATE);
		if(rate==0) rate = 1;
        amount = df->val(IV_TOTAL) * rate;
    }
    if(!amount) return;
    SetStatementInvoiceData(df,&st);

    double balance = InvoiceRealBalance(df,df->dat[1],tso.dat[2]);       //Analysis Balance
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        double rate = df->val(IV_RATE);
        if(rate==0) rate =1;
        balance *= rate;
    }
    if(balance==0) {
        lset(st.dat[St_RECEIVABLE],"@");             //Receivable
        st.dat[St_FLAG][0] = 'F';
    }
    else {
        int receiveable_days = diffday(df->dat[IV_DATE],tso.dat[2]) - atoi(df->dat[IV_TERMS]);
        if(receiveable_days>0) {
            lset(st.dat[St_RECEIVABLE],"*");             //Receivable
        }
        else {
            Over_Balance[RCA] += amount;
        }
    }

    if(amount>0) {
        dtodb(amount,&st,STDR);
    }
    else if(amount<0){
        dtodb(amount * -1,&st,STCR);
    }
    memwrt(&st);
    FindInvoiceOutstandingBalance(df->dat[IV_DATE],balance);
}

void FindInvoiceStatement(DB *df)
{
    CString str;

    if(STCR == St_CR) {
        str.Format("select * from [arinv] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    else {
        str.Format("select * from [apinv] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetRange32(1,7);
    m_p2->SetPos(1);
    int fld = GetDbFieldNo(&adoTmp,"DATE");

    pDlg->SetDlgItemText(IDC_TYPE,"Invoice No");
    for(long rec=1;rec<=last && rec>0; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        pDlg->SetDlgItemText(IDC_MSG,adoTmp.dat[1]);
        pDlg->SetDlgItemText(IDC_INVDATE,mdate(adoTmp.dat[fld]));
        PrintAuditorStatement(&adoTmp);
    }

    dclose(&adoTmp);
}

/******************************************************************************
*   CHEQUE RETURN                                                             *
******************************************************************************/
void check_ipay_cheque_return(DB *ipay)
{
    if(strcmp(ipay->dat[IP_RETURN],ipay->dat[IP_DATE])>=0) {
         lsetblank(&st);
         double ret_amt = ipay->val(IP_TOTAL);
         if(!ret_amt) return;
         if(tso.dat[Ts_CONVERT][0]=='Y') {
             double rate = ipay->val(IP_RATE);
             if(rate==0) rate = 1;
             ret_amt = ipay->val(IP_TOTAL) * rate;
         }
         lset(st.dat[St_BILLNO],ipay->dat[1]);
         lset(st.dat[St_DATE],ipay->dat[IP_RETURN]);
         lset(st.dat[St_IDATE],ipay->dat[IP_RETURN]);
         lset(st.dat[St_NAME],"DISHONOURNED CHEQUE");
         lset(st.dat[St_TYPE],"3");
         if(ret_amt>0) {
             dtodb(ret_amt,&st,STDR);
         }
         else{
             dtodb(ret_amt * -1,&st,STCR);
         }
         memwrt(&st);
    }
}

void save_a_state_ipay(DB *ipay)
{
    char tmp[180];

    lsetblank(&st);
    double ret_amt = ipay->val(IP_TOTAL);
    if(!ret_amt) return;
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        double rate = ipay->val(IP_RATE);
        if(rate==0) rate = 1;
        ret_amt = ipay->val(IP_TOTAL) * rate;
    }
    lset(st.dat[St_BILLNO],ipay->dat[1]);
    if(isblank(ipay->dat[IP_CQ_DATE])) {
        lset(st.dat[St_IDATE],ipay->dat[IP_DATE]);
        lset(st.dat[St_DATE],ipay->dat[IP_DATE]);
    }
    else {
        lset(st.dat[St_DATE], ipay->dat[IP_DATE] );
        lset(st.dat[St_IDATE], ipay->dat[IP_DATE] );
    }
    if(strcmp(st.dat[St_IDATE],tso.dat[2])<=0) {
        sprintf(tmp,"Payment (%s",ipay->dat[IP_CHEQUE]);
    }
    else {
        return;
        sprintf(tmp,"PD Chq (%s",ipay->dat[IP_CHEQUE]);
    }

    rtrim(tmp);
    strcat(tmp,")");
    lset(st.dat[St_NAME],tmp);
    lset(st.dat[St_TYPE],"3");
    if(ret_amt>0) {
        dtodb(ret_amt,&st,STCR);
    }
    else if(ret_amt<0){
        dtodb(ret_amt * -1,&st,STDR);
    }
    memwrt(&st);

    if(tso.dat[Ts_CONVERT][0]=='Y') {
        ret_amt = ipay->val(IP_CGL);
        if(ret_amt) {
            lsetblank(&st);
            lset(st.dat[St_BILLNO],ipay->dat[1]);
            lset(st.dat[St_DATE],ipay->dat[IP_DATE]);
            lset(st.dat[St_IDATE],ipay->dat[IP_DATE]);
            lset(st.dat[St_NAME],"Currency Gain/Loss");
            lset(st.dat[St_TYPE],"3");
            if(ret_amt>0) {
                dtodb(ret_amt,&st,STDR);
            }
            else if(ret_amt<0){
                dtodb(ret_amt * -1,&st,STCR);
            }
            if(ret_amt) memwrt(&st);
        }
    }
    check_ipay_cheque_return(ipay);
}

void FindPaymentStatement()
{
    CString str;
    if(STCR == St_CR) {
        str.Format("select * from [aripay] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    else {
        str.Format("select * from [apipay] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetPos(2);
    pDlg->SetDlgItemText(IDC_TYPE,"Payment Receipt No");
    int fld = GetDbFieldNo(&adoTmp,"DATE");

    for(long rec=1;rec<=last && rec>0; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        pDlg->SetDlgItemText(IDC_MSG,adoTmp.dat[1]);
        pDlg->SetDlgItemText(IDC_INVDATE,mdate(adoTmp.dat[fld]));
        save_a_state_ipay(&adoTmp);
    }

    dclose(&adoTmp);
}

void save_cn_statement(DB *cr)
{
    lsetblank(&st);
    double ret_amt = cr->val(Cr_TOTAL);
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        double rate = ipay->val(Cr_RATE);
        if(rate==0) rate = 1;
        ret_amt = cr->val(Cr_TOTAL) * rate;
    }
    lset(st.dat[St_BILLNO],cr->dat[1]);
    lset(st.dat[St_DATE],cr->dat[Cr_DATE]);
    lset(st.dat[St_IDATE],cr->dat[Cr_DATE]);
    lset(st.dat[St_NAME],cr->dat[Cr_REMARKS]);
    lset(st.dat[St_TYPE],"4");                  //Return
    if(ret_amt>0) {
        dtodb(ret_amt,&st,STCR);
    }
    else if(ret_amt<0){
        dtodb(ret_amt * -1,&st,STDR);
    }
    if(ret_amt) memwrt(&st);

    if(tso.dat[Ts_CONVERT][0]=='Y') {
        lsetblank(&st);
        ret_amt = cr->val(Cr_CGL);
        lset(st.dat[St_BILLNO],cr->dat[1]);
        lset(st.dat[St_DATE],cr->dat[Cr_DATE]);
        lset(st.dat[St_IDATE],cr->dat[Cr_DATE]);
        lset(st.dat[St_NAME],"Currency Gain/Loss");
        lset(st.dat[St_TYPE],"4");
        if(ret_amt>0) {
            dtodb(ret_amt,&st,STDR);
        }
        else if(ret_amt<0){
            dtodb(ret_amt * -1,&st,STCR);
        }
        if(ret_amt) memwrt(&st);
    }
}

void FindCNStatement()
{
    CString str;
    if(STCR == St_CR) {
        str.Format("select * from [arcr] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    else {
        str.Format("select * from [apcr] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(tso.dat[2]));
    }
    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetPos(3);
    int fld = GetDbFieldNo(&adoTmp,"DATE");

    for(long rec=1; rec<=last; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        pDlg->SetDlgItemText(IDC_MSG,adoTmp.dat[1]);
        pDlg->SetDlgItemText(IDC_INVDATE,mdate(adoTmp.dat[fld]));
        save_cn_statement(&adoTmp);
    }

    dclose(&adoTmp);
}

void print_adj(DB *cr)
{
    double amount = cr->val(Cr_TOTAL);
    if(!amount) return;
    double cr_amt = tally(cr->dat[1],crdat,1,CN_TOTAL);
    double bal = amount - cr_amt;

    char tmp[180];
    dtoa(bal,tmp,17,2);
    bal = atof(tmp);
    if(bal<=0) return;

    if(tso.dat[Ts_CONVERT][0]=='Y') {
        bal *= cr->val(Cr_RATE);
    }
    int j = NoOfDaysOutstanding(cr->dat[Cr_DATE]);
    for(;j>=0;j--){
        Over_Balance[j] -= bal;
        if(Over_Balance[j]>=0) break;
        bal = 0 - Over_Balance[j];
        Over_Balance[j] = 0;
        if(j==0) {
             Over_Balance[11] -= bal;
             break;
        }
    }
}

void rep_adj()
{
    CString str;
    char d1[20],d2[20];

    strcpy(d1,DateStr(tso.dat[1]));
    strcpy(d2,DateStr(tso.dat[2]));
    if(STCR == St_CR) {
        str.Format("select * from [arcr] where [date] >= '%s' and [date] <= '%s' and [debtor] = '%s'",
            d1,d2,customer->dat[1]);
    }
    else {
        str.Format("select * from [apcr] where [date] >= '%s' and [date] <= '%s' and [debtor] = '%s'",
            d1,d2,customer->dat[1]);
    }

    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetPos(4);
    pDlg->SetDlgItemText(IDC_TYPE,"Credit Note No");
    int fld = GetDbFieldNo(&adoTmp,"DATE");

    for(long rec=1;rec<=last && rec>0; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        pDlg->SetDlgItemText(IDC_MSG,adoTmp.dat[1]);
        pDlg->SetDlgItemText(IDC_INVDATE,mdate(adoTmp.dat[fld]));
        print_adj(&adoTmp);
    }

    dclose(&adoTmp);
}

void GetAuditorStateData(void)
{
    lsetblank(&two);
    lset(two.dat[2],tso.dat[1]);
    plusday(two.dat[2],-1);
    GetSalesmanStateData();

    memset(Over_Balance,0,sizeof(Over_Balance));
    memstart(&st);
    FindInvoiceStatement(&arinv);
    FindPaymentStatement();
    FindCNStatement();
    rep_adj();
    memend(&st);
    ComputeStateBalance();
}
