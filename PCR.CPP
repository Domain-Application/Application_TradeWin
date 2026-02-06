#include "stdafx.h"
#include "menu.h"

enum pcr {PV_BILL=1,PV_DATE,PV_CUSTOMER,PV_NAME,PV_ENAME,PV_ADD1,PV_ADD2,PV_ADD3,
           PV_AMOUNT,PV_CASH1,PV_CASH2,PV_REM1,PV_REM2,
           PV_N1,PV_F1,PV_V1,PV_P1,PV_D1,PV_N2,PV_F2,PV_V2,PV_P2,PV_D2,
           PV_N3,PV_F3,PV_V3,PV_P3,PV_D3,PV_N4,PV_F4,PV_V4,PV_P4,PV_D4,
           PV_N5,PV_F5,PV_V5,PV_P5,PV_D5,PV_N6,PV_F6,PV_V6,PV_P6,PV_D6,
           PV_N7,PV_F7,PV_V7,PV_P7,PV_D7,PV_N8,PV_F8,PV_V8,PV_P8,PV_D8,
           PV_N9,PV_F9,PV_V9,PV_P9,PV_D9,PV_N10,PV_F10,PV_V10,PV_P10,PV_D10,
           PV_N11,PV_F11,PV_V11,PV_P11,PV_D11,
           PV_N12,PV_F12,PV_V12,PV_P12,PV_D12,
           PV_N13,PV_F13,PV_V13,PV_P13,PV_D13};

void split_pcr_line(double amount)
{
    char text[200];
    int  i,j;

    strcpy(text,std_nw("RM",amount));
    split(&report,PV_CASH1,2,text);
    i = (isblank(report.dat[PV_CASH2])) ? PV_CASH1 : PV_CASH2;
    for(j=report.f[i].len-1; j>=0 ;j--){
        if(report.dat[i][j]!=' '){
            report.dat[i][j+1]=')';
            break;
        }
    }
}

long FindPaymentCr(long Srec,long irec,long rec,DB *df,int print_no)
{
    double total;
    int    i;

    for(i=0; rec<Srec; rec++, i++) {
        if(i>=print_no) break;
        ridx(irec+rec, crdat,1);
        qd(crdat->dat[CN_INVOICE],inv);
        total = atof(crdat->dat[CN_TOTAL]);
        lset(report.dat[PV_N1+i*5],crdat->dat[CN_INVOICE]);
        lset(report.dat[PV_D1+i*5],inv->dat[IV_DATE]);
        dtodb(total,&report,(int)(PV_P1+i*5));
    }
    return rec;
}

void GetCrFigure(FILE *ptr,DB *df,int print_no)
{
    long Srec,irec,rec;
    double amt;

    ropen(&report,"pcr.rep");
    fetch(&report,df);
    qd(df->dat[Cr_CUSTOMER],customer);
    lset(report.dat[PV_CUSTOMER],customer->dat[1]);
    lset(report.dat[PV_NAME],customer->dat[ENAME]);
    lset(report.dat[PV_ENAME],customer->dat[ENAME]);
    lsets(&report,customer,PV_ADD1,ADD1,3);
    lset(report.dat[PV_REM1],df->dat[Cr_REMARKS]);

    qd(df->dat[1], gldat);
    lset(report.dat[PV_REM2], gldat->dat[CG_DESCRIPT]);

    amt = atof(df->dat[Cr_TOTAL]);
    split_pcr_line(amt);
    Srec = interval(&irec,df->dat[1], crdat,1);
    for(rec=0;rec<Srec;) {
        rec = FindPaymentCr(Srec,irec,rec,df,print_no);
        PrintLogo(ptr,title);
        mprint(ptr,&report);
        fprintf(ptr,"");
    }
    if(Srec==0) {
        PrintLogo(ptr,title);
        mprint(ptr,&report);
        fprintf(ptr,"");
    }
    dclose(&report);
}

void PCreditNote(long irec,FILE *ptr,DB *df)
{
//    ptr = fopen("lpt1","w");
    fprintf(ptr,"2C!");
    GetCrFigure(ptr,df,12);
//    fclose(ptr);
}
