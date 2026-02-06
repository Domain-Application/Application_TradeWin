#include "stdafx.h"
#include "menu.h"
#include "Tso.h"

void print_salesman_state(DB *df)
{
    double rate;

    double amount = InvoiceRealBalance(df,df->dat[1],two.dat[2]);             //Analysis Balance
    if(!amount) return;
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        rate     = atof(df->dat[IV_RATE]);
        if(rate==0) rate =1;
        amount *= rate;
    }
    SetStatementInvoiceData(df,&st1);
    if(amount>0) {
        dtodb(amount,&st1,STDR);
    }
    else if(amount<0){
        dtodb(amount * -1,&st1,STCR);
    }

//    FindInvoiceOutstandingBalance(df->dat[IV_DATE],amount);
    double lamt = InvoiceRealBalance(df,df->dat[1],tso.dat[2]);             //Analysis Balance
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        rate     = atof(df->dat[IV_RATE]);
        if(rate==0) rate =1;
        lamt *= rate;
    }
    if(lamt==0) {
        st1.dat[St_FLAG][0] = 'F';
    }
    else if(lamt<amount) {
        st1.dat[St_FLAG][0] = 'P';
    }
    memwrt(&st1);
}

void rep_salesman_state()
{
    CString str;

    if(STCR == St_CR) {
        str.Format("select * from [arinv] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(two.dat[2]));
    }
    else {
        str.Format("select * from [apinv] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(two.dat[2]));
    }
    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetPos(5);

    for(long rec=1;rec<=last && rec>0; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        print_salesman_state(&adoTmp);
    }

    dclose(&adoTmp);
}

void print_cr(DB *cr)
{
    double amount = atof(cr->dat[Cr_TOTAL]);
    if(!amount) return;
    double cr_amt = tally(cr->dat[1],crdat,1,CN_TOTAL);
    double bal = amount - cr_amt;

    char tmp[180];
    dtoa(bal,tmp,12,2);
    bal = atof(tmp);
    if(bal<=0) return;

    if(tso.dat[Ts_CONVERT][0]=='Y') {
        double rate = atof(cr->dat[Cr_RATE]);
        if(rate==0) rate =1;
        bal *= rate;
    }

    lsetblank(&st1);
    lset(st1.dat[St_INVNO],cr->dat[1]);
    lset(st1.dat[St_BILLNO],cr->dat[Cr_BILL]);
    lset(st1.dat[St_DATE],cr->dat[Cr_DATE]);
    lset(st1.dat[St_IDATE],cr->dat[Cr_DATE]);
    lset(st1.dat[St_NAME],cr->dat[Cr_REMARKS]);
    lset(st1.dat[St_TYPE],"9");                  //Purchase
    if(bal>0) {
        dtodb(bal,&st1,STCR);
    }
    else if(bal<0){
        dtodb(bal*-1,&st1,STDR);
    }
    memwrt(&st1);
    int j = NoOfDaysOutstanding(cr->dat[Cr_DATE]);
    for(;j>=0;j--){
        Over_Balance[j] -= bal;
        if(Over_Balance[j]>=0) break;
        bal = 0 - Over_Balance[j];
        Over_Balance[j] = 0;
    }
}

void rep_cr()
{
    CString str;
    char d1[20],d2[20];

    strcpy(d1,DateStr(two.dat[1]));
    strcpy(d2,DateStr(two.dat[2]));
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
    m_p2->SetPos(6);

    for(long rec=1; rec<=last; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        print_cr(&adoTmp);
    }

    dclose(&adoTmp);
}

void print_ipay(DB *ipay)
{
    double amount,ipay_amt,bal,rate;
    int    j;
    char   tmp[180];

    amount = atof(ipay->dat[IP_TOTAL]);
    if(!amount) return;
    ipay_amt = tally(ipay->dat[1],paydat,1,PY_TOTAL);
    bal = amount - ipay_amt;
    dtoa(bal,tmp,12,2);
    if(atof(tmp)==0) return;
    if(tso.dat[Ts_CONVERT][0]=='Y') {
        rate     = atof(ipay->dat[IP_RATE]);
        if(rate==0) rate =1;
        bal *= rate;
    }

    lsetblank(&st1);
    lset(st1.dat[St_INVNO],ipay->dat[1]);
    lset(st1.dat[St_BILLNO],ipay->dat[IP_BILL]);
    lset(st1.dat[St_DATE],ipay->dat[IP_DATE]);
    lset(st1.dat[St_IDATE],ipay->dat[IP_DATE]);

    if(!isblank(ipay->dat[IP_CQ_DATE])) {
        lset(st1.dat[St_IDATE], ipay->dat[IP_DATE] );
    }

    if(realm(tso.dat[1],st1.dat[St_IDATE],tso.dat[2])) {
        if(amount>ipay_amt) return;
    }
    sprintf(tmp,"Payment(%s",ipay->dat[IP_CHEQUE]);
    rtrim(tmp);
    strcat(tmp,")");
    lset(st1.dat[St_NAME],tmp);
    lset(st1.dat[St_TYPE],"8");                  //Purchase
    if(bal>0) {
        dtodb(bal,&st1,STCR);
    }
    else if(bal<0){
        dtodb(bal*-1,&st1,STDR);
    }
    memwrt(&st1);
    j = NoOfDaysOutstanding(ipay->dat[IP_DATE]);
    for(;j>=0;j--){
        Over_Balance[j] -= bal;
        if(Over_Balance[j]>=0) break;
        bal = 0 - Over_Balance[j];
        Over_Balance[j] = 0;
    }
}

void rep_ipay()
{
    CString str;
    if(STCR == St_CR) {
        str.Format("select * from [aripay] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(two.dat[2]));
    }
    else {
        str.Format("select * from [apipay] where [debtor] = '%s' and [date] <= '%s' order by [date]",
            customer->dat[1],DateStr(two.dat[2]));
    }
    DB adoTmp;
    ado(&adoTmp,"",NULL,str);

    long last = read_sumindex(1,&adoTmp);
    m_p1->SetRange32(1,last);
    m_p2->SetPos(7);

    for(long rec=1;rec<=last && rec>0; rec++) {
        adoFastRead(rec,&adoTmp);
        m_p1->SetPos(rec);
        if(strcmp(adoTmp.dat[IP_RETURN],adoTmp.dat[IP_DATE])>0) continue;
        print_ipay(&adoTmp);
    }

    dclose(&adoTmp);
}

void GetSalesmanStateData(void)
{
    memstart(&st1);
    rep_salesman_state();
    rep_cr();
    rep_ipay();
    memend(&st1);
    arrage_st(&st1);
}
