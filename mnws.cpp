// mnws.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"

enum mnws {DR_DATE=1,DR_TICKET,DR_ACCODE,DR_BNAME,DR_VEHICLE,
           DR_TIMEIN,DR_TIMEOUT,DR_DTIME,DR_PT,DR_Q1,DR_PRICE,DR_AMOUNT};

void save_mnws(DB *df,DB *rep)
{
    long rec;
    int  i,j,k;
    double wt,total;
    char time[10];

    fetch_rec(&report,df);
    dtime(time,df->dat[DP_TIMEIN],df->dat[DP_TIMEOUT]);
    lset(report.dat[DR_DTIME],time);
    ctom(report.dat[DR_DTIME]);
    getmet(m,df);
    qd(df->dat[DP_ACCODE],&accode);
    if(wildcmp(accode.dat[ACCODE_COUNTRY],daily.dat[Da_COUNTRY])) return;
    lset(report.dat[DR_BNAME],accode.dat[2]);
    total = 0;
    for(i=k=0;i<8;i++){
        j = i*4+DP_PT1;
        rec = qd(df->dat[j],&prodtype);
        if(rec>0){
            if(!wildcmp(df->dat[j],daily.dat[Da_PT])){
                wt = atof(df->dat[j+2]) * atof(df->dat[j+3]);
                lset(report.dat[DR_PT],prodtype.dat[1]);
                dtodb(wt,&report,DR_AMOUNT);
                rset(report.dat[DR_Q1],df->dat[j+2]);
                rset(report.dat[DR_PRICE],df->dat[j+3]);
                memwrt(&report);
                k++;
            }
            total += wt;
        }
    }
}

DB *ptrmnws;
void put_mnws()
{
    int  field[12],fo[] = { DR_TICKET,DR_ACCODE,DR_VEHICLE,DR_PT,DR_DATE,0};
    char *ndetail[]={"Ticket No","A/c Code","Vehicle No","Product","Date",""};

    mloop(&daily,ptrmnws,&report,DP_DATE,save_mnws);
    if(report.t->sumrecord>0){
        multisort s2(3,fo,ndetail,&report,field);
        s2.DoModal();
        sort(&report,field);
//        stdplan(&daily,&report);
//        CrystalReport(&daily,&report,field,5);
    }
 }

void MonthlyProductDetailReport(DB *df,char *rep)
{
    dopen(&daily,"daily.scr");
    dopen(&report,rep);

    ptrmnws = df;
    CDaily t1(IDD_DAILY,&daily,0,put_mnws);
    t1.DoModal();
    dclose(&report);
    dclose(&daily);
}
