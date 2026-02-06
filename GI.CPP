// gi.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include "state.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int NoOfMonthsOutstanding(char *date,char *date2)
{
    int days = diffmon(date, date2);
    int j = days;
    if(j>11) j = 11;
    return j;
}

void GetInvOutstandingBalance(char *date,char *date2,double offset)
{
    int j = NoOfMonthsOutstanding(date,date2);
    Over_Balance[j] += offset;
}

void CollectReceipt(DB *data,char *cust, char *cur,char *paydate)
{
    memset(Over_Balance,0,sizeof(Over_Balance));
    char szTmp[40];
    double accbal =0;

    DB adoTmp;
    CString strTable = "ibal";
    if(STCR == St_DR) strTable = "ipbal";
    CString strSQL;
    strSQL.Format("select * from [%s] where [debtor]='%s' and [balance] <> 0 order by [REFNO]",
        strTable,cust);

    BOOL flag = ado(&adoTmp,"",NULL,strSQL);
    if(flag==FALSE) {
        strSQL = "Error Message\n\n" + strSQL;
        AfxMessageBox(strSQL);
        return;
    }

    memstart(data);
    lsetblank(data);

    long last = read_sumindex(1,&adoTmp);
    if(m_p1) m_p1->SetRange32(1,last);
    for(long rec=1; rec<= last; rec++) {
        adoFastRead(rec,&adoTmp);
        if(m_p1) m_p1->SetPos(rec);
        double bal = adoTmp.val(Ibal_AMOUNT) - adoTmp.val(Ibal_PAYSUM) -
                     adoTmp.val(Ibal_CNSUM) + adoTmp.val(Ibal_RCSUM);
        dtoa(bal, szTmp, 15, 2);
        bal = atof(szTmp);
        if(bal) {
            lset(data->dat[RE_INVOICE],adoTmp.dat[1]);
            lset(data->dat[RE_DATE],adoTmp.dat[Ibal_DATE]);
            double iamt = adoTmp.val(Ibal_AMOUNT);
            dtodb(iamt,data,RE_TOTAL);
            dtodb(bal,data,RE_IBAL);
            dtodb(bal,data,RE_BAL);
            accbal += data->val(RE_BAL);
            dtodb(accbal,&receipt,RE_ACC);
            int Noofmons = diffmon(adoTmp.dat[Ibal_DATE],paydate);
            ltodb(Noofmons,data,RE_MONTH);

            int NoofDays = diffday(adoTmp.dat[Ibal_DATE],paydate);
            ltodb(NoofDays,data,RE_DAY);

            if(NoofDays > atoi(adoTmp.dat[Ibal_TERMS])) {
                lset(data->dat[RE_OVERDUE],"*");
            }
            else {
                lset(data->dat[RE_OVERDUE],"");
            }
            memwrt(data);
            GetInvOutstandingBalance(adoTmp.dat[Ibal_DATE],paydate,bal);
        }
    }
    if(m_p1) m_p1 = NULL;
    dclose(&adoTmp);

    memend(data);
}

void GetInvoiceInformation(DB* df)
{
    char paydate[210];

    if(df->sumfield == IP_VATNO) {
        if(strcmp(df->dat[IP_DATE],df->dat[IP_CQ_DATE])>0) {
            strcpy(paydate,df->dat[IP_DATE]);
        }
        else {
            strcpy(paydate,df->dat[IP_CQ_DATE]);
        }
        CollectReceipt(&receipt,df->dat[IP_CUSTOMER],df->dat[IP_CURR],today);
    }
    else {
        strcpy(paydate,df->dat[Cr_DATE]);
        CollectReceipt(&receipt,df->dat[Cr_CUSTOMER],df->dat[Cr_CURR],today);
    }
}

void ResetReceiptBalance()
{
    long last = receipt.t->sumrecord;
    double bal = 0;

    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&receipt);
        bal += receipt.val(RE_BAL);
        dtodb(bal,&receipt,RE_ACC);
        fastwrt(rec,&receipt);
    }
    INV_bal = bal;
}
