#include "stdafx.h"
#include "menu.h"


enum ppay {PV_REFNO=1,PV_RECEIPT,PV_STAFF,PV_SALESMAN,PV_DATE,PV_CREDITOR,PV_NAME,PV_ENAME,PV_ADD1,PV_ADD2,PV_ADD3,
           PV_CHEQUE,PV_AMOUNT,PV_CASH1,PV_CASH2,PV_REM1,PV_REM2,
           PV_N1,PV_D1,PV_P1,PV_N2,PV_D2,PV_P2,PV_N3,PV_D3,PV_P3,
           PV_N4,PV_D4,PV_P4,PV_N5,PV_D5,PV_P5,PV_N6,PV_D6,PV_P6,
           PV_N7,PV_D7,PV_P7,PV_N8,PV_D8,PV_P8,PV_N9,PV_D9,PV_P9,
           PV_N10,PV_D10,PV_P10,PV_N11,PV_D11,PV_P11,PV_N12,PV_D12,PV_P12,
           PV_N13,PV_D13,PV_P13,PV_N14,PV_D14,PV_P14,
           PV_N15,PV_D15,PV_P15,PV_N16,PV_D16,PV_P16,
           PV_N17,PV_D17,PV_P17,PV_N18,PV_D18,PV_P18,
           PV_CQ_DATE};

void split_ppay_line(double amount)
{
    char text[200];

    strcpy(text,std_nw(profile.dat[PF_CURRENCY],amount));
    split(&report,PV_CASH1,2,text);
    int i = (isblank(report.dat[PV_CASH2])) ? PV_CASH1 : PV_CASH2;
    for(int j=report.f[i].len-1; j>=0 ;j--){
        if(report.dat[i][j]!=' '){
            report.dat[i][j+1]=')';
            break;
        }
    }
}

long FindPaymentInvoice(long Srec,long irec,long rec,DB *df,int print_no)
{
    for(long i=0; rec<Srec; rec++, i++) {
        if(i>=print_no) break;
        ridx(irec+rec,paydat,1);
        qd(paydat->dat[PY_INVOICE],inv);
        double total = paydat->val(PY_TOTAL);
        lset(report.dat[PV_N1+i*3],paydat->dat[PY_INVOICE]);
        lset(report.dat[PV_D1+i*3],inv->dat[IV_DATE]);
        dtodb(total,&report,(int)(PV_P1+i*3));
    }
    return rec;
}

void PrepaprePaymentInfo(DB *df)
{
    lsetblank(&report);
    fetch(&report,df);
    qd(df->dat[IP_CUSTOMER],customer);
    lset(report.dat[PV_CREDITOR],customer->dat[1]);
    lset(report.dat[PV_NAME],customer->dat[2]);
    lset(report.dat[PV_ENAME],customer->dat[2]);
    lset(report.dat[PV_STAFF],df->dat[IP_STAFF]);
    qd(df->dat[IP_STAFF],&staff);
    lset(report.dat[PV_SALESMAN],staff.dat[2]);
    lsets(&report,customer,PV_ADD1,CU_ADD1,3);
    double amt = df->val(IP_TOTAL);
    split_ppay_line(amt);
}

void GetInvoiceFigure(FILE *ptr,DB *df,int print_no)
{
    long Srec,irec,rec;

    Srec = interval(&irec,df->dat[1],paydat,1);
    for(rec=0;rec<Srec;) {
        PrepaprePaymentInfo(df);
        SetPaperSize(ptr);
        PrintLogo(ptr,title);
        rec = FindPaymentInvoice(Srec,irec,rec,df,print_no);
        mprint(ptr,&report);
        fprintf(ptr,"");
    }
    if(Srec==0) {
        PrepaprePaymentInfo(df);
        SetPaperSize(ptr);
        PrintLogo(ptr,title);
        mprint(ptr,&report);
        fprintf(ptr,"");
    }
}

void Ppay(long irec,FILE *ptr,DB *df)
{
    ropen(&report,"ppay.rep");
    GetInvoiceFigure(ptr,df,12);
    dclose(&report);
}

void Preceipt(long irec,FILE *ptr,DB *df)
{
    ropen(&report,"preceipt.rep");
    GetInvoiceFigure(ptr,df,12);
    dclose(&report);
}
