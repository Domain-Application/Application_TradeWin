// Sdp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"

enum sdp {DPP_PT=1,DPP_PNAME,DPP_TL,DPP_QTY,DPP_WT,DPP_PRICE};
void save_sdp(DB *df,DB *rep)
{
    long rec;
    int  i,j;
    double wt;

    qd(df->dat[DP_ACCODE],&accode);
    if(wildcmp(accode.dat[ACCODE_COUNTRY],daily.dat[Da_COUNTRY])) return;
    for(i=0;i<8;i++){
        j = i*4+DP_PT1;
        rec = qd(df->dat[j],&prodtype);
        if(rec>0){
            if ( !wildcmp(df->dat[j],daily.dat[Da_PT])) {
                    append(&report);
                    lset(report.dat[DPP_PT],df->dat[j]);
                    wt = atof(df->dat[j+2]) * atof(df->dat[j+3]);
                    dtodb(wt,&report,DPP_WT);
                    rset(report.dat[DPP_QTY],df->dat[j+2]);
                    memwrt(&report);
            }
        }
    }
}

void fill_sdp()
{
    long rec;
    double qty,price;

    for(rec=1; rec<=report.t->sumrecord; rec++){
        readrec(rec, &report);
        if(!isblank(report.dat[1])){
            qd(report.dat[DPP_PT],&prodtype);
            lset(report.dat[DPP_PNAME],prodtype.dat[2]);
            qty = atof(report.dat[DPP_QTY]);
            if(qty) {
                price = atof(report.dat[DPP_WT]) / qty;
                dtodb(price,&report,DPP_PRICE);
            }
            fastwrt(rec,&report);
        }
    }
}
//DB *ttt;
void put_sdp()
{
    int  field[] = {DPP_PT,0};

    if(mloop(&daily,ttt,&report,DP_DATE,save_sdp)){
        sort(&report,field);
//        squ(&report,field);
        fill_sdp();
//        stdplan(&daily,&report);
//        CrystalReport(&daily,&report,field,5);
    }
}

void summary_dp(DB *df,char *rep)
{
    dopen(&daily,"daily.scr");
    dopen(&report,rep);
    ttt = df;
    CDaily t1(IDD_DAILY,&daily,0,put_sdp);
    t1.DoModal();

    dclose(&report);
    dclose(&daily);
}
