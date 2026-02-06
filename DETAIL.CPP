// Detail.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"


enum detail {DE_TICKET=1,DE_DATE,DE_VEHICLE,DE_DO,DE_CONTRACT,
     DE_ACCODE,DE_IC,DE_TIMEIN,DE_TIMEOUT,
     DE_SGROSS,DE_STARE,DE_NET,DE_THEORY,DE_PTYPE};

void rep_detail(double total,DB *df,DB *rep)
{
    fetch_rec(rep,df);
    dtodb(total,rep,DE_THEORY);
    getmet(m,df);
    ltodb(m[11],rep,DE_NET);
    qd(df->dat[DP_PTYPE],&ptype);
    lset(rep->dat[DE_PTYPE],ptype.dat[2]);
    memwrt(rep);
}

void save_detail(DB *df,DB *rep)
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
                    rep_detail(total,df,rep);
                    return;
                }
            }
        }
    }
    if(daily.dat[Da_PT][0]=='*') rep_detail(total,df,rep);
}

DB *ptrd;

void put_detail()
{
    int  field[] = {DE_TICKET,0};

        if(mloop(&daily,ptrd,&report,DP_DATE,save_detail)){
            sort(&report,field);
//            stdplan(&daily,&report);
//            CrystalReport(&daily,&report,field,5);
        }
}

void DetailReport(DB *df,char *rep)
{
    dopen(&daily,"daily.scr");
    dopen(&report,rep);

    ptrd = df;
    CDaily t1(IDD_DAILY,&daily,0,put_detail);
    t1.DoModal();
    dclose(&report);
    dclose(&daily);
}
