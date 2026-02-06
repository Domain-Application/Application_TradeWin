// sv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"

enum sv {Vr_ACCODE=1,Vr_NAME,Vr_TL,Vr_NET,Vr_AMOUNT,Vr_PRICE,Vr_COUNTRY};

void rep_sv(double total,DB *df,DB *rep)
{
    fetch_rec(rep,df);
    lset(rep->dat[Vr_NAME],accode.dat[2]);
    dtodb(total,rep,Vr_AMOUNT);
    getmet(m,df);
    ltodb(m[11],rep,Vr_NET);
    memwrt(rep);
}

void save_sv(DB *df,DB *rep)
{
    int i,j;
    double total,wt;
	long rec;

    qd(df->dat[DP_ACCODE],&accode);
    if(wildcmp(accode.dat[CU_COUNTRY],daily.dat[Da_COUNTRY])) return;
    total = 0;
    for(i=0;i<8;i++){
        j = i*4+DP_PT1;
        rec = qd(df->dat[j],&prodtype);
        if(rec>0){
            if ( !wildcmp(df->dat[j],daily.dat[Da_PT])) {
                wt = atof(df->dat[j+2]) * atof(df->dat[j+3]);
                total += wt;
                if (!strcmp(df->dat[j],daily.dat[Da_PT])){
                    rep_sv(total,df,rep);
                    return;
                }
            }
        }
    }
    if(daily.dat[Da_PT][0]=='*') rep_sv(total,df,rep);
}

void fill_sv()
{
    long rec;
    double qty,price;

    for(rec=1; rec<=report.t->sumrecord; rec++){
        readrec(rec, &report);
        if(!isblank(report.dat[1])){
            qty = atof(report.dat[Vr_NET]);
            if(qty) {
                price = atof(report.dat[Vr_AMOUNT]) / qty;
                dtodb(price,&report,Vr_PRICE);
            }
            fastwrt(rec,&report);
        }
    }
}

DB *ptrsv;
void put_sv()
{
    int  field[] = {1,0};

    if(mloop(&daily,ptrsv,&report,DP_DATE,save_sv)){
       sort(&report,field);
//       squ(&report,field);
       fill_sv();
//       stdplan(&daily,&report);
//         CrystalReport(&daily,&report,field,5);
        }
}

void AcTotalWeight(DB *df,char *rep)
{

    dopen(&daily,"daily.scr");
    dopen(&report,rep);
    ptrsv = df;
    CDaily t1(IDD_DAILY,&daily,0,put_sv);
    t1.DoModal();
    dclose(&report);
    dclose(&daily);
}
