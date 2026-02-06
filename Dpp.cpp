// Dpp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"

DB   dpp;
enum dpp {DPP_ACCODE=1,DPP_NAME,DPP_PT,DPP_PNAME,DPP_QTY,DPP_AMOUNT,DPP_PRICE};
void save_dpp(DB *df,DB *rep)
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
                append(&dpp);
                lset(dpp.dat[DPP_PT],prodtype.dat[1]);
                lset(dpp.dat[DPP_PNAME],prodtype.dat[2]);
                wt = atof(df->dat[j+2]) * atof(df->dat[j+3]);
                dtodb(wt,&dpp,DPP_AMOUNT);
                rset(dpp.dat[DPP_QTY],df->dat[j+2]);
                lset(dpp.dat[DPP_ACCODE],df->dat[DP_ACCODE]);
                memwrt(&dpp);
            }
        }
    }
}

void fill_squ()
{
    long rec;
    double qty,price;

    for(rec=1; rec<=dpp.t->sumrecord; rec++){
        readrec(rec, &dpp);
        if(!isblank(dpp.dat[1])){
            qd(dpp.dat[1],&accode);
            lset(dpp.dat[DPP_NAME],accode.dat[2]);
            qty = atof(dpp.dat[DPP_QTY]);
            if(qty) {
                price = atof(dpp.dat[DPP_AMOUNT]) / qty;
                dtodb(price,&dpp,DPP_PRICE);
            }
            fastwrt(rec,&dpp);
        }
    }
}

DB   *ptrdpp;
void put_dpp()
{
    int  field[] = {DPP_ACCODE,DPP_PT,0};

    if(mloop(&daily,ptrdpp,&dpp,DP_DATE,save_dpp)){
        sort(&dpp,field);
//        squ(&dpp,field);
        fill_squ();
//        stdplan(&daily,&dpp);
//        CrystalReport(&daily,&dpp,field,5);
    }
}

void DPProductSummary(DB *df,char *rep)
{
    dopen(&daily,"daily.scr");
    dopen(&dpp, rep);

    ptrdpp = df;
    CDaily t1(IDD_DAILY,&daily,0,put_dpp);
    t1.DoModal();

    dclose(&dpp);
    dclose(&daily);
}
