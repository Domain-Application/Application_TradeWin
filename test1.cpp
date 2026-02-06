// Detail.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "month.h"
#include "multisort.h"


void MonthRep(DB *df,DB *rep)
{
    fetch_rec(rep,df);
    qd(df->dat[CUST],&accode);
    lset(rep->dat[DI_NAME],accode.dat[2]);
    qd(df->dat[PT],&pt);
    lset(rep->dat[DI_PNAME],pt.dat[2]);
    memwrt(rep);
}

void put_detail()
{
    int  field[12],fo[] = { DI_TICKET,DI_CUSTOMER,DI_VEHICLE,DI_PT,0};
    char *ndetail[]={"Ticket No","A/c Code","Vehicle No","Product",""};

    mloop(&monthly,&wb,&report,TDATE,MonthRep);
    if(report.t->sumrecord>0) {
//      sort(&report,field);
        stdplan(&monthly,&report);
    }
}

void month_report(DB *df)
{
    dopen(&monthly,"month.scr");
    dopen(&report,"detail.rep");

    CMonth t2(IDD_MONTH,&monthly,0,put_detail);
    t2.DoModal();

    dclose(&report);
    dclose(&monthly);
}
